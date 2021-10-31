#include "node.hpp"

// constructors
Node::Node(){}
Node::Node(
    std::string const& name,
    std::shared_ptr<Node> const& parent,
    std::list<std::shared_ptr<Node>> const& children,
    std::string const& path,
    int depth,
    glm::mat4 const& world_transform,
    glm::mat4 const& local_tansform
) : name_(name),
    parent_(parent),
    children_(children),
    path_(path),
    depth_(depth),
    world_transform_(world_transform),
    local_tansform_(local_transform)
{}

// get methods
std::string Node::getName(){
    return name_;
}
std::shared_ptr<Node> Node::getParent(){
    return parent_;
}
std::shared_ptr<Node> Node::getChild(std::string const& child_name){
    return parent_;
}
std::list<std::shared_ptr<Node>> Node::getChildren(){
    return children_;
}
std::string Node::getPath(){
    return path_;
}
int Node::getDepth(){
    return depth_;
}
glm::mat4 Node::getWorldTransform(){
    return world_transform_;
}
glm::mat4 Node::getLocalTransform(){
    return local_tansform_;
}

// set methods
void Node::setParent(std::shared_ptr<Node> const& parent){
    parent_ = parent;
}