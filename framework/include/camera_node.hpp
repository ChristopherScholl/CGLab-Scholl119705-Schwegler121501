#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include <string>
#include <memory>
#include "node.hpp"

class CameraNode : Node {
public:
    // constructors
    CameraNode();
    CameraNode(bool isPerspective, bool isEnabled, glm::mat4 const& projectionMatrix);

    // get attribute methods
    bool getPerspective() const;
    bool getEnabled() const;
    glm::mat4 getProjectionMatrix() const;

    // set attribute methods
    void setEnabled(bool isEnabled);
    void setProjectionMatrix(glm::mat4 const& projectionMatrix);

private:
    bool isPerspective_;
    bool isEnabled_;
    glm::mat4 projectionMatrix_;
};

#endif