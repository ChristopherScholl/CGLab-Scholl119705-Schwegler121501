#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include <string>
#include <memory>
#include "node.hpp"

class SceneGraph {
 public:
  // constructors
  SceneGraph();
  SceneGraph(std::string const& name);

  // get attribute methods
  std::string getName();
  std::shared_ptr<Node> getRoot();

  // print method
  std::string printGraph();
 
 private:
  // set attribute methods
  void setName(std::string const& name);
  void setRoot(std::shared_ptr<Node> const& root_node);

  // attributes
  std::string name_;
  std::shared_ptr<Node> root_node_;
};

#endif