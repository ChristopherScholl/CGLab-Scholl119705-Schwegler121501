#include "geometry_node.hpp"

// constructors
GeometryNode::GeometryNode(){}
GeometryNode::GeometryNode(model const& geometry) : geometry_(geometry) {}
GeometryNode::GeometryNode(std::string const& name, std::shared_ptr<Node> const& parent) : Node(name, parent){}

// get attribute methods
model GeometryNode::getGeometry() const {
  return geometry_;
}

// set attribute methods
void GeometryNode::setGeometry(model const& geometry) {
  geometry_ = geometry;
}