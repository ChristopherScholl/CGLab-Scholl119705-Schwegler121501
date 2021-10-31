#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include <string>
#include <memory>
#include "node.hpp"

// 
class CameraNode {
 public:
  // constructors
  SceneGraph(std::string const& name);
  ~SceneGraph();
  // get methods
  std::string getName();
  std::shared_ptr<Node> getRoot();
  // print method
  std::string printGraph();
 
 private:
  // set methods
  void setName(std::string const& name);
  void setRoot(std::shared_ptr<Node> const& root_node);
  // attributes
  std::string name_;
  std::shared_ptr<Node> root_node_;
};

#endif