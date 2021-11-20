#include "point_light_node.hpp"

// constructors
PointLightNode::PointLightNode(){}
PointLightNode::PointLightNode(float lightIntensity, color lightColor) : lightIntensity_(lightIntensity), lightColor_(lightColor){}
  
  // get attribute methods
  float PointLightNode::getIntensity() const{
      return lightIntensity_;
  }
  color PointLightNode::getColor() const{
      return lightColor_;
  }

  // set attribute methods
  void PointLightNode::setIntensity(float lightIntensity){
      lightIntensity_ = lightIntensity;
  }
  void PointLightNode::setColor(color lightColor){
      lightColor_ = lightColor;
  }