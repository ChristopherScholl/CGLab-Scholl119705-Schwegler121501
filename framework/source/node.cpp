#include "node.hpp"

// constructors
Node::Node() : name_("root"),
    parent_(nullptr),
    children_(),
    path_(""),
    depth_(0),
    worldTransform_(glm::mat4(1)),
    localTransform_(glm::mat4(1))
{}
Node::Node(
    std::string const& name,
    std::shared_ptr<Node> const& parent
) : name_(name),
    parent_(parent),
    path_(""),
    depth_(parent->getDepth()+1),
    worldTransform_(glm::mat4(1)),
    localTransform_(glm::mat4(1)){}
Node::Node(
    std::string const& name,
    std::shared_ptr<Node> const& parent,
    std::list<std::shared_ptr<Node>> const& children,
    std::string const& path,
    glm::mat4 const& worldTransform,
    glm::mat4 const& localTansform
) : name_(name),
    parent_(parent),
    children_(children),
    path_(path),
    depth_(parent->getDepth()+1),
    worldTransform_(worldTransform),
    localTransform_(localTansform)
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
    if (depth_ != 0){
        return parent_->getWorldTransform() * localTransform_;
    }else{
        return localTransform_;
    }
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
    return nullptr;
}

// add one child
void Node::addChild(std::shared_ptr<Node> const& node){
    children_.push_back(node);
}

// remove one child
void Node::removeChild(std::string const& childName){
    for(std::shared_ptr<Node> child : children_){
        if(child->getName().compare(childName) == 0){
            children_.remove(child);
        }
    }
}