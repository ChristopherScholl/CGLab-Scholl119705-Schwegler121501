#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include <string>
#include <memory>
#include "node.hpp"

class GeometryNode : public Node {
 public:
  // constructors
  GeometryNode();
  GeometryNode(model const& geometry);
  GeometryNode(
  std::string const& name,
  std::shared_ptr<Node> const& parent,
  glm::fmat4 const& localTansform,
  float size,
  float speed,
  float distance,
  glm::fvec3 color
  );
  
  // get attribute methods
  model getGeometry() const;
  float getSize() const;
  float getSpeed() const;
  float getDistance() const;
  glm::fvec3 getColor() const;

  // set attribute methods
  void setGeometry(model const& geometry);
  void setColor(glm::fvec3 const& color);
 
 private:
  // attributes
  model geometry_;
  float size_;
  float speed_;
  float distance_;
  glm::fvec3 color_;
};

#endif