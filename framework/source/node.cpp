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
    worldTransform_(world_transform),
    localTransform_(local_tansform)
{}
Node::Node(
    glm::mat4 const& world_transform,
    glm::mat4 const& local_transform
) : name_("root"),
    parent_(nullptr),
    children_(),
    path_(""),
    depth_(0),
    worldTransform_(world_transform),
    localTransform_(local_transform)
{}

// get attribute methods
std::string Node::getName(){
    return name_;
}
std::shared_ptr<Node> Node::getParent(){
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
    return worldTransform_;
}
glm::mat4 Node::getLocalTransform(){
    return localTransform_;
}

// set methods
void Node::setParent(std::shared_ptr<Node> const& parent){
    parent_ = parent;
}
void Node::setWorldTransform(glm::mat4 const& worldTransform){
    worldTransform_ = worldTransform;
}
void Node::setLocalTransform(glm::mat4 const& localTransform){
    localTransform_ = localTransform;
}

// get one specific child
std::shared_ptr<Node> Node::getChild(std::string const& childName){
    for (std::shared_ptr<Node> node : children_){
        if(node->getName() == childName){
            return node;
        }
    }
}