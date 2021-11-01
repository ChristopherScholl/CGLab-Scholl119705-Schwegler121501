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
  
  // get attribute methods
  model getGeometry() const;

  // set attribute methods
  void setGeometry(model const& geometry);
 
 private:
  // attributes
  model geometry_;
};

#endif