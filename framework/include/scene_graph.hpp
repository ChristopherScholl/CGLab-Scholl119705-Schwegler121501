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
  SceneGraph(std::string const& name, std::shared_ptr<Node> const& rootNode);

  // get attribute methods
  std::string getName();
  std::shared_ptr<Node> getRoot();

  // print method
  std::string printGraph();
 
 private:
  // set attribute methods
  void setName(std::string const& name);
  void setRoot(std::shared_ptr<Node> const& rootNode);

  // recursive print method for individual node
  std::string printNode(std::shared_ptr<Node> const& node);

  // attributes
  std::string name_;
  std::shared_ptr<Node> rootNode_;
};

#endif