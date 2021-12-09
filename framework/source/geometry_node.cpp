#include "geometry_node.hpp"

// constructors
GeometryNode::GeometryNode(){}
GeometryNode::GeometryNode(model const& geometry) : geometry_(geometry) {}
GeometryNode::GeometryNode(
  std::string const& name,
  std::shared_ptr<Node> const& parent,
  glm::fmat4 const& localTansform,
  float size,
  float speed,
  float distance,
  glm::fvec3 color,
  std::string texture
) :
  Node(name, parent, localTansform),
  size_(size),
  speed_(speed),
  distance_(distance),
  color_(glm::normalize(color)),
  texture_(texture)
  {}

// get attribute methods
model GeometryNode::getGeometry() const {
  return geometry_;
}
float GeometryNode::getSize() const {
  return size_;
}
float GeometryNode::getSpeed() const {
  return speed_;
}
float GeometryNode::getDistance() const {
  return distance_;
}
glm::fvec3 GeometryNode::getColor() const {
  return color_;
}
std::string GeometryNode::getTexture() const {
  return texture_;
}

// set attribute methods
void GeometryNode::setGeometry(model const& geometry) {
  geometry_ = geometry;
}
void GeometryNode::setColor(glm::fvec3 const& color) {
  color_ = color;
}