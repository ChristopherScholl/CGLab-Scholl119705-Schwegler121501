#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  initializeGeometry();
  initializeShaderPrograms();
  initializeSolarSystem();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

void ApplicationSolar::initializeSolarSystem(){
  // root
  Node root_node = Node();
  std::shared_ptr<Node> root_node_pointer = std::make_shared<Node>(root_node);
  solarSystem_ = SceneGraph("Solar System", root_node_pointer);

  // sun
  glm::fmat4 sun_localTransform = glm::scale(glm::fmat4{}, glm::fvec3{0.7f, 0.7f, 0.7f});
  sun_localTransform = glm::rotate(sun_localTransform, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f});

  Node sun_holder = Node("sun holder", root_node_pointer, sun_localTransform);
  std::shared_ptr<Node> sun_holder_pointer = std::make_shared<Node>(sun_holder);
  root_node_pointer->addChild(sun_holder_pointer);
  GeometryNode sun = GeometryNode("sun", sun_holder_pointer, glm::fmat4(1), 0.7f, 1.0f, 0.0f);
  std::shared_ptr<GeometryNode> sun_pointer = std::make_shared<GeometryNode>(sun);
  sun_holder_pointer->addChild(sun_pointer);

  // planets
  makePlanet("mercury", root_node_pointer, 0.1f, 2.0f, 2.0f);
  makePlanet("venus", root_node_pointer, 0.3f, 1.9f, 3.0f);
  makePlanet("earth", root_node_pointer, 0.3f, 1.7f, 4.0f);
  makePlanet("mars", root_node_pointer, 0.2f, 1.8f, 5.0f);
  makePlanet("jupiter", root_node_pointer, 0.6f, 0.9f, 6.0f);
  makePlanet("saturn", root_node_pointer, 0.5f, 1.0f, 7.0f);
  makePlanet("uranus", root_node_pointer, 0.4f, 0.5f, 8.0f);
  makePlanet("neptune", root_node_pointer, 0.4f, 0.4f, 9.0f);
  
  // moons
  std::shared_ptr<Node> earth_holder_pointer = root_node_pointer->getChild("earth holder");
  makePlanet("moon", earth_holder_pointer, 0.1f, 1.3f, 1.0f);

  // camera
  CameraNode camera = CameraNode("camera", root_node_pointer, glm::fmat4(1));
  std::shared_ptr<CameraNode> camera_pointer = std::make_shared<CameraNode>(camera);
  root_node_pointer->addChild(camera_pointer);

  std::cout << solarSystem_.printGraph() << std::endl;
}

void ApplicationSolar::makePlanet(std::string const& name, std::shared_ptr<Node> const& parent, float size, float speed, float distance){
  glm::fmat4 localTransform = glm::scale(glm::fmat4{}, glm::fvec3{size, size, size});
  localTransform = glm::rotate(localTransform, float(glfwGetTime()), glm::fvec3{0.0f, speed, 0.0f});
  localTransform = glm::translate(localTransform, glm::fvec3{0.0f, 0.0f, distance});

  Node planet_holder = Node(name + " holder", parent, localTransform);
  std::shared_ptr<Node> planet_holder_pointer = std::make_shared<Node>(planet_holder);
  parent->addChild(planet_holder_pointer);
  GeometryNode planet = GeometryNode(name, planet_holder_pointer, glm::fmat4(1), size, speed, distance);
  std::shared_ptr<GeometryNode> planet_pointer = std::make_shared<GeometryNode>(planet);
  planet_holder_pointer->addChild(planet_pointer);

  solarSystem_.addPlanet(planet_pointer);
}

void ApplicationSolar::render() const {
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);

  std::shared_ptr<Node> sun = solarSystem_.getRoot()->getChild("sun holder")->getChild("sun");
  glm::fmat4 sun_model_matrix = sun->getWorldTransform();
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(sun_model_matrix));
  
  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 sun_normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * sun_model_matrix);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(sun_normal_matrix));
  
  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);

  // draw something idk
  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

  sun_model_matrix = glm::rotate(sun_model_matrix, float(glfwGetTime()), glm::fvec3{0.0f, sun->getSpeed(), 0.0f});
  sun_model_matrix = glm::translate(sun_model_matrix, glm::fvec3{0.0f, 0.0f, sun->getDistance()});
  solarSystem_.getRoot()->setLocalTransform(sun_model_matrix);

  auto planets = solarSystem_.getPlanets();
  for (auto planet : planets){
    renderPlanet(planet);
  }
}

void ApplicationSolar::renderPlanet(std::shared_ptr<GeometryNode> planet)const{
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);

  glm::fmat4 planet_model_matrix = planet->getWorldTransform();
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(planet_model_matrix));
  
  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 planet_normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * planet_model_matrix);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(planet_normal_matrix));
  
  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);

  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

  planet_model_matrix = glm::rotate(planet_model_matrix, float(glfwGetTime()), glm::fvec3{0.0f, planet->getSpeed(), 0.0f});
  planet_model_matrix = glm::translate(planet_model_matrix, glm::fvec3{0.0f, 0.0f, planet->getDistance()});
  planet->getParent()->setLocalTransform(planet_model_matrix);
}

void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
  // bind shader to which to upload unforms
  glUseProgram(m_shaders.at("planet").handle);
  // upload uniform values to new locations
  uploadView();
  uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
}

// load models
void ApplicationSolar::initializeGeometry() {
  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object 
  planet_object.num_elements = GLsizei(planet_model.indices.size());
}

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadView();
  }
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    uploadView();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
}


// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}