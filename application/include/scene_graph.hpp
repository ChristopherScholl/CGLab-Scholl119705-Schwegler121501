#ifndef APPLICATION_SCENE_GRAPH_HPP
#define APPLICATION_SCENE_GRAPH_HPP

#include "application.hpp"

// gpu representation of model
class SceneGraph : public Application {
 public:
  // allocate and initialize objects
  // ApplicationVbo(std::string const& resource_path);
  // ~ApplicationVbo();
  SceneGraph(std::string const& resource_path);
  ~SceneGraph();

  // draw all objects
  // void render() const;

 
 private:
  // void initializeGeometry();
  // vertex buffer object
  // GLuint m_vertex_bo;
};

#endif