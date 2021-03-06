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
#include <random>

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,star_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  initializeGeometry();
  initializeShaderPrograms();
  initializeSolarSystem();
  initializeStars();
  initilizeFramebuffer(initial_resolution.x, initial_resolution.y);
  initializeFullScreenQuad();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

void ApplicationSolar::initializeSolarSystem(){
  // root node
  Node root_node = Node();
  std::shared_ptr<Node> root_node_pointer = std::make_shared<Node>(root_node);

  // create scene graph
  solarSystem_ = SceneGraph("Solar System", root_node_pointer);

  // sun
  makeSun("sun", root_node_pointer, 0.5f, 0.0f, 0.0f, glm::fvec3{255, 215, 0}, 1.0f, glm::fvec3{255, 255, 150}, "sunmap.png", 1);

  // planets
  makePlanet("mercury", root_node_pointer, 0.09f, 0.5f, 1.0f, glm::fvec3{139, 69, 19}, "mercurymap.png", 2);
  makePlanet("venus", root_node_pointer, 0.2f, 0.4f, 1.5f, glm::fvec3{245, 222, 179}, "venusmap.png", 3);
  makePlanet("earth", root_node_pointer, 0.2f, 0.3f, 2.5f, glm::fvec3{34, 139, 34}, "earthmap1k.png", 4);
  makePlanet("mars", root_node_pointer, 0.1f, 0.2f, 3.5f, glm::fvec3{255, 0, 0}, "mars_1k_color.png", 5);
  makePlanet("jupiter", root_node_pointer, 0.4f, 0.09f, 5.0f, glm::fvec3{255, 140, 0}, "jupitermap.png", 6);
  makePlanet("saturn", root_node_pointer, 0.4f, 0.10f, 7.0f, glm::fvec3{255, 165, 0}, "saturnmap.png", 7);
  makePlanet("uranus", root_node_pointer, 0.3f, 0.05f, 9.0f, glm::fvec3{65, 105, 225}, "uranusmap.png", 8);
  makePlanet("neptune", root_node_pointer, 0.3f, 0.04f, 10.0f, glm::fvec3{0, 0, 139}, "neptunemap.png", 9);
  makePlanet("pluto", root_node_pointer, 0.04f, 0.06f, 10.5f, glm::fvec3{255, 255, 255}, "plutomap1k.png", 10);
  
  // moons
  std::shared_ptr<Node> earth_holder_pointer = root_node_pointer->getChild("earth holder");
  makePlanet("moon", earth_holder_pointer, 1.0f, 1.3f, 0.6f, glm::fvec3{128, 128, 128}, "moonmap1k.png", 11);

  // camera
  CameraNode camera = CameraNode("camera", root_node_pointer, glm::fmat4(1));
  std::shared_ptr<CameraNode> camera_pointer = std::make_shared<CameraNode>(camera);
  root_node_pointer->addChild(camera_pointer);

  std::cout << solarSystem_.printGraph() << std::endl;
}

void ApplicationSolar::makeSun(std::string const& name, std::shared_ptr<Node> const& parent, float size, float speed, float distance, glm::fvec3 color, float light_intensity, glm::fvec3 light_color, std::string texture, int index){
  // set up local transform matrix
  glm::fmat4 localTransform = parent->getWorldTransform();
  localTransform = glm::translate(localTransform, glm::fvec3{0.0f, 0.0f, distance});
  localTransform = glm::rotate(localTransform, float(glfwGetTime()) * speed, glm::fvec3{0.0f, 1.0f, 0.0f});
  localTransform = glm::scale(localTransform, glm::fvec3{size, size, size});

  // create holder node
  PointLightNode sun_light = PointLightNode(name + " light", parent, localTransform, light_intensity, light_color);
  std::shared_ptr<PointLightNode> sun_light_pointer = std::make_shared<PointLightNode>(sun_light);
  parent->addChild(sun_light_pointer);

  // create geometry node
  GeometryNode sun_geometry = GeometryNode(name + " geometry", sun_light_pointer, glm::fmat4(1), size, speed, distance, color, texture, index);
  std::shared_ptr<GeometryNode> sun_geometry_pointer = std::make_shared<GeometryNode>(sun_geometry);
  sun_light_pointer->addChild(sun_geometry_pointer);

  solarSystem_.addPlanet(sun_geometry_pointer);
  solarSystem_.addLightNode(sun_light_pointer);

  makeTexture(sun_geometry_pointer);
}

void ApplicationSolar::makePlanet(std::string const& name, std::shared_ptr<Node> const& parent, float size, float speed, float distance, glm::fvec3 color, std::string texture, int index){
  // set up local transform matrix
  glm::fmat4 localTransform = parent->getWorldTransform();
  localTransform = glm::translate(localTransform, glm::fvec3{0.0f, 0.0f, distance});
  localTransform = glm::rotate(localTransform, float(glfwGetTime()) * speed, glm::fvec3{0.0f, 1.0f, 0.0f});
  localTransform = glm::scale(localTransform, glm::fvec3{size, size, size});

  // create holder node
  Node planet_holder = Node(name + " holder", parent, localTransform);
  std::shared_ptr<Node> planet_holder_pointer = std::make_shared<Node>(planet_holder);
  parent->addChild(planet_holder_pointer);

  // create geometry node
  GeometryNode planet = GeometryNode(name, planet_holder_pointer, glm::fmat4(1), size, speed, distance, color, texture, index);
  std::shared_ptr<GeometryNode> planet_pointer = std::make_shared<GeometryNode>(planet);
  planet_holder_pointer->addChild(planet_pointer);

  solarSystem_.addPlanet(planet_pointer);

  makeTexture(planet_pointer);
}

void ApplicationSolar::makeTexture(std::shared_ptr<GeometryNode> const& object){
  // load texture
  pixel_data planetTexture = texture_loader::file(m_resource_path + "textures/" + object->getTexture());

  // select active texture unit
  glActiveTexture(GL_TEXTURE0 + object->getIndex());

  // create texture object
  texture_object t;
  t.target = GL_TEXTURE_2D;
  // generate texture names
  glGenTextures(1, &t.handle);
  glBindTexture(t.target, t.handle);
  object->setTextureObject(t);

  // define texture sampling parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // define texture data and format
  glTexImage2D(GL_TEXTURE_2D, 0, planetTexture.channels, planetTexture.width, planetTexture.height, 0, planetTexture.channels, planetTexture.channel_type, planetTexture.ptr());
}

void ApplicationSolar::initializeStars(){
  int number_stars = 1000;
  for(int i = 0; i < number_stars; i++){
    // generate position
    stars_.push_back(float(rand() % 100 - 50));
    stars_.push_back(float(rand() % 100 - 50));
    stars_.push_back(- float(rand() % 100));
    // generate color
    stars_.push_back(float(rand() % 256) / 255);
    stars_.push_back(float(rand() % 256) / 255);
    stars_.push_back(float(rand() % 256) / 255);
  }

  // generate vertex array object
  glGenVertexArrays(1, &star_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(star_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &star_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * number_stars * 6, stars_.data(), GL_STATIC_DRAW);

  // first attribute (position)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GLsizei(sizeof(float) * 6), 0);

  // second attribute (color)
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, GLsizei(sizeof(float) * 6), (void*) (sizeof(float)*3));

  // store type of primitive to draw
  star_object.draw_mode = GL_POINTS;
  // transfer number of indices to model object 
  star_object.num_elements = GLsizei(number_stars);
}

void ApplicationSolar::initilizeFramebuffer(int width, int height){
  // create framebuffer object
  glGenFramebuffers(1, &framebuffer.handle);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.handle);
  
  // select active texture unit
  glActiveTexture(GL_TEXTURE0 + 12);
  
  // create texture object
  texture_object color_buffer = texture_object{12, GL_TEXTURE_2D};

  // generate texture names
  glGenTextures(1, &color_buffer.handle);
  glBindTexture(color_buffer.target, color_buffer.handle);

  // define texture sampling parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // define texture data and format
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

  // set color attachment
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_buffer.handle, 0);
  framebuffer.color_buffer = color_buffer;
  
  // create depth attachment as renderbuffer
  glGenRenderbuffers(1, &framebuffer.depth_handle);
  glBindRenderbuffer(GL_RENDERBUFFER, framebuffer.depth_handle);
  // ERROR? - GL_DEBUG_SEVERITY_LOW - GL_DEBUG_TYPE_OTHER: Framebuffer detailed info: The driver allocated storage for renderbuffer 1.
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GLsizei(width), GLsizei(height));
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebuffer.depth_handle);
  
  // define which buffers to write
  GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, draw_buffers);
  
  // check whether the framebuffer can be written
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
    std::cout << "Oh no! Framebuffer died." << std::endl;
  }else{
    std::cout << "Good newz! Framebuffer lives." << std::endl;
  }
}

void ApplicationSolar::initializeFullScreenQuad(){
  // triangles
  std::vector<GLfloat> triangles = {
    // v1
    -1.0, -1.0, 0.0, 0.0, 0.0,
    // v2
    1.0, -1.0, 0.0, 1.0, 0.0,
    // v3
    -1.0, 1.0, 0.0, 0.0, 1.0,
    // v4
    1.0, 1.0, 0.0, 1.0, 1.0
  };

  // generate vertex array object
  glGenVertexArrays(1, &full_screen_quad.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(full_screen_quad.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &full_screen_quad.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, full_screen_quad.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * triangles.size(), triangles.data(), GL_STATIC_DRAW);

  // first attribute (position)
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GLsizei(sizeof(float) * 5), 0);

  // second attribute (uv)
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, GLsizei(sizeof(float) * 5), (void*) (sizeof(float)*3));

  // store type of primitive to draw
  full_screen_quad.draw_mode = GL_TRIANGLE_STRIP;
  // transfer number of indices to model object 
  full_screen_quad.num_elements = GLsizei(triangles.size() / 5);
}

void ApplicationSolar::render() const {
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.handle);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  // render stars
  renderStars();

  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);

  // upload light uniforms
  auto lightNodes = solarSystem_.getLightNodes();
  for(auto lightNode : lightNodes){
    // upload light intensity
    auto temp_intensity = glGetUniformLocation(m_shaders.at("planet").handle, "light_intensity");
    glUniform1f(temp_intensity, lightNode->getIntensity());

    // upload light color
    auto temp_color = glGetUniformLocation(m_shaders.at("planet").handle, "light_color");
    glUniform3f(temp_color, lightNode->getColor()[0], lightNode->getColor()[1], lightNode->getColor()[2]);

    // calculate position
    glm::fvec4 light_position = lightNode->getWorldTransform() * glm::fvec4{0, 0, 0, 1};
    // upload position
    auto temp_position = glGetUniformLocation(m_shaders.at("planet").handle, "light_position");
    glUniform3f(temp_position, light_position[0] / light_position[3], light_position[1] / light_position[3], light_position[2] / light_position[3]);
  }

  // render planets
  auto planets = solarSystem_.getPlanets();
  for (auto planet : planets){
    renderPlanet(planet);
  }

  renderPostProcessing();
}

void ApplicationSolar::renderPostProcessing()const{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  // full-screen quad
  glUseProgram(m_shaders.at("fullScreenQuad").handle);

  // extract texture from framebuffer
  glActiveTexture(GL_TEXTURE0 + 12);
  glBindTexture(framebuffer.color_buffer.target, framebuffer.color_buffer.handle);

  // upload texture to shader
  auto temp_texture = glGetUniformLocation(m_shaders.at("fullScreenQuad").handle, "fullScreenQuad_texture");
  glUniform1i(temp_texture, framebuffer.color_buffer.handle);

  // render quad
  glBindVertexArray(full_screen_quad.vertex_AO);
  glDrawArrays(full_screen_quad.draw_mode, 0, full_screen_quad.num_elements);
}

void ApplicationSolar::renderStars()const{
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("star").handle);

  glm::fmat4 matrix = glm::fmat4();
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(matrix));

  // bind the VAO to draw
  glBindVertexArray(star_object.vertex_AO);

  // draw stars
  glDrawArrays(star_object.draw_mode, GLint(0), star_object.num_elements);
}

void ApplicationSolar::renderPlanet(std::shared_ptr<GeometryNode> planet)const{
  glm::fmat4 planetWorldTransform = planet->getParent()->getWorldTransform();
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(planetWorldTransform));
  
  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 planet_normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * planetWorldTransform);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(planet_normal_matrix));

  // set color
  auto temp_color = glGetUniformLocation(m_shaders.at("planet").handle, "planet_color");
  glm::fvec3 color = planet->getColor();
  glUniform3f(temp_color, color[0], color[1], color[2]);

  // set ambient intensity
  auto temp_ambient =  glGetUniformLocation(m_shaders.at("planet").handle, "ambient_intensity");

  if(planet->getParent()->getIsLight() == true){
    glUniform3f(temp_ambient, 2.0f, 2.0f, 2.0f);
  }else{
    glUniform3f(temp_ambient, 0.4f, 0.4f, 0.4f);
  }

  // access texture
  glActiveTexture(GL_TEXTURE0 + planet->getIndex());
  texture_object textureObject = planet->getTextureObject();
  glBindTexture(textureObject.target, textureObject.handle);

  // upload texture to shader
  auto temp_texture = glGetUniformLocation(m_shaders.at("planet").handle, "planet_texture");
  glUseProgram(m_shaders.at("planet").handle);
  glUniform1i(temp_texture, textureObject.handle);
  
  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);

  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

  // calculate next step
  planetWorldTransform = glm::rotate(planet->getParent()->getParent()->getWorldTransform(), float(glfwGetTime()) * planet->getSpeed(), glm::fvec3{0.0f, 1.0f, 0.0f});
  planet->getParent()->setWorldTransform(planetWorldTransform);
}

void ApplicationSolar::uploadView(std::string shader_name) {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at(shader_name).u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection(std::string shader_name) {
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at(shader_name).u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
  // bind shader to which to upload unforms
  glUseProgram(m_shaders.at("planet").handle);
  // upload uniform values to new locations
  uploadView("planet");
  uploadProjection("planet");

  // bind shader to which to upload unforms
  glUseProgram(m_shaders.at("star").handle);
  // upload uniform values to new locations
  uploadView("star");
  uploadProjection("star");
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER, m_resource_path + "shaders/simple.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;

  // store shader program objects in container
  m_shaders.emplace("star", shader_program{{{GL_VERTEX_SHADER, m_resource_path + "shaders/vao.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("star").u_locs["ModelMatrix"] = -1;
  m_shaders.at("star").u_locs["ViewMatrix"] = -1;
  m_shaders.at("star").u_locs["ProjectionMatrix"] = -1;

  //  store shader program objects in container
  m_shaders.emplace("fullScreenQuad", shader_program{{{GL_VERTEX_SHADER, m_resource_path + "shaders/post-processing.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/post-processing.frag"}}});
}

// load models
void ApplicationSolar::initializeGeometry() {
  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL | model::TEXCOORD);

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
  // activate third attribute on gpu
  glEnableVertexAttribArray(2);
  // second attribute is 2 floats with no offset & stride
  glVertexAttribPointer(2, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::TEXCOORD]);

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
  }
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
  }
  else if (key == GLFW_KEY_A  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-0.1f, 0.0f, 0.0f});
  }
  else if (key == GLFW_KEY_D  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.1f, 0.0f, 0.0f});
  }
  else if (key == GLFW_KEY_SPACE  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.1f, 0.0f});
  }
  else if (key == GLFW_KEY_LEFT_SHIFT  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, -0.1f, 0.0f});
  }
  glUseProgram(m_shaders.at("planet").handle);
  uploadView("planet");
  glUseProgram(m_shaders.at("star").handle);
  uploadView("star");
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrices
  glUseProgram(m_shaders.at("planet").handle);
  uploadProjection("planet");
  glUseProgram(m_shaders.at("star").handle);
  uploadProjection("star");
}


// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}