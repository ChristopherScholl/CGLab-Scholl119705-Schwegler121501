#ifndef POINT_LIGHT_NODE_HPP
#define POINT_LIGHT_NODE_HPP

#include <string>
#include <memory>
#include "node.hpp"
#include "structs.hpp"

class PointLightNode : public Node {
 public:
  // constructors
  PointLightNode();
  PointLightNode(float lightIntensity, color lightColor);
  
  // get attribute methods
  float getIntensity() const;
  color getColor() const;

  // set attribute methods
  void setIntensity(float lightIntensity);
  void setColor(color lightColor);
 
 private:
  // attributes
  float lightIntensity_;
  color lightColor_;
};

#endif