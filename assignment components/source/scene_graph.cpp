#include "scene_graph.hpp"

// constructors
SceneGraph::SceneGraph(){}
SceneGraph::SceneGraph(std::string const& name) : name_(name), root_node_(std::make_shared<Node>(Node())){}

// get methods
std::string SceneGraph::getName(){
    return name_;
}
std::shared_ptr<Node> SceneGraph::getRoot(){
    return root_node_;
}

// set methods
void SceneGraph::setName(std::string const& name){
    name_ = name;
}
void SceneGraph::setRoot(std::shared_ptr<Node> const& root_node){
    root_node_ = root_node;
}

// print method
std::string SceneGraph::printGraph(){
    return name_ + " Kekse";
}
