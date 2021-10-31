#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include <string>
#include <memory>
#include "node.hpp"


class CameraNode : Node {
public:
    // 
    CameraNode(std::shared_ptr<Node> parent, std::string const& name);
    bool getPerspective() const;
    bool getEnabled() const;
    glm::mat4 getProjectionMatrix() const;
    void setEnabled(bool isEnabled);
    void setProjectionMatrix(glm::mat4 const& projectionMatrix);
private:
    bool isPerspective_;
    bool isEnabled_;
    glm::mat4 projectionMatrix_;
};

#endif