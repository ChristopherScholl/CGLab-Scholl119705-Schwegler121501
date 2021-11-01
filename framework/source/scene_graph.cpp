#include "scene_graph.hpp"

// constructors
SceneGraph::SceneGraph() : name_("DefaultSceneGraph"), rootNode_(std::make_shared<Node>(Node())) {}
SceneGraph::SceneGraph(std::string const& name) : name_(name), rootNode_(std::make_shared<Node>(Node())){}
SceneGraph::SceneGraph(std::string const& name, std::shared_ptr<Node> const& rootNode) : name_(name), rootNode_(rootNode){}

// get attribute methods
std::string SceneGraph::getName(){
    return name_;
}
std::shared_ptr<Node> SceneGraph::getRoot(){
    return rootNode_;
}

// set attribute methods
void SceneGraph::setName(std::string const& name){
    name_ = name;
}
void SceneGraph::setRoot(std::shared_ptr<Node> const& rootNode){
    rootNode_ = rootNode;
}

// print methods
std::string SceneGraph::printGraph(){
    return "Name: " + name_ + ", Nodes: " + printNode(rootNode_);
}
std::string SceneGraph::printNode(std::shared_ptr<Node> const& node){
    // print name
    std::string outputString = node->getName();
    
    // print every child node
    std::list<std::shared_ptr<Node>> children = node->getChildren();
    if (children.size() > 0) {
        outputString.append(" -> (");
        for (auto child : children){
            outputString.append(printNode(node) + ", ");
        } 
        // remove last comma
        outputString = outputString.substr(0, outputString.size()-2);
        outputString.append(")");
    }

    // return string
    return outputString;
}