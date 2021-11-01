#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <memory>
#include <list>
#include <../../external/glm-0.9.6.3/glm/glm.hpp>
#include <model.hpp>
#include "node.hpp"

class Node {
 public:
  // constructors
  Node();
  Node(
      std::string const& name,
      std::shared_ptr<Node> const& parent,
      std::list<std::shared_ptr<Node>> const& children,
      std::string const& path,
      int depth,
      glm::mat4 const& worldTransform,
      glm::mat4 const& localTansform
  );
  Node(
      glm::mat4 const& worldTransform,
      glm::mat4 const& localTansform
  );

  // get attribute methods
  std::string getName();
  std::shared_ptr<Node> getParent();
  std::list<std::shared_ptr<Node>> getChildren();
  std::string getPath();
  int getDepth();
  glm::mat4 getWorldTransform();
  glm::mat4 getLocalTransform();

  // set attribute methods
  void setParent(std::shared_ptr<Node> const& parent);
  void setWorldTransform(glm::mat4 const& worldTransform);
  void setLocalTransform(glm::mat4 const& localTransform);

  // child specific methods
  std::shared_ptr<Node> getChild(std::string const& childName);
  void addChild(std::shared_ptr<Node> const&);
  void removeChild(std::string const& childName);
 
 private:
  // attributes
  std::string name_;
  std::shared_ptr<Node> parent_;
  std::list<std::shared_ptr<Node>> children_;
  std::string path_;
  int depth_;
  glm::mat4 worldTransform_;
  glm::mat4 localTransform_;
};

#endif