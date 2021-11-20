#include "point_light_node.hpp"

// constructors
PointLightNode::PointLightNode(){}
PointLightNode::PointLightNode(float lightIntensity, glm::fvec3 lightColor) : lightIntensity_(lightIntensity), lightColor_(lightColor){}
  
  // get attribute methods
  float PointLightNode::getIntensity() const{
      return lightIntensity_;
  }
  glm::fvec3 PointLightNode::getColor() const{
      return lightColor_;
  }

  // set attribute methods
  void PointLightNode::setIntensity(float lightIntensity){
      lightIntensity_ = lightIntensity;
  }
  void PointLightNode::setColor(glm::fvec3 lightColor){
      lightColor_ = lightColor;
  }