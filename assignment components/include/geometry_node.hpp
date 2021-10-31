#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include <string>
#include <memory>
#include "node.hpp"
#include <../../framework/include/model.hpp>

// 
class GeometryNode : public Node {
 public:
  model getGeometry() const;
  void setGeometry(model const& geometry);
 
 private:
  // attributes
  model geometry_
};

#endif