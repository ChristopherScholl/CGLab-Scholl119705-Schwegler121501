#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <memory>
#include <list>
#include <../../external/glm-0.9.6.3/glm/glm.hpp>
#include <../../external/glm-0.9.6.3/glm/ext.hpp>
#include <model.hpp>
#include <structs.hpp>

class Node {
 public:
  // constructors
  Node();
  Node(
      std::string const& name,
      std::shared_ptr<Node> const& parent,
      glm::fmat4 const& localTansform
  );
  Node(
      std::string const& name,
      std::shared_ptr<Node> const& parent,
      std::list<std::shared_ptr<Node>> const& children,
      std::string const& path,
      glm::fmat4 const& localTansform
  );

  // get attribute methods
  std::string getName();
  std::shared_ptr<Node> getParent();
  std::list<std::shared_ptr<Node>> getChildren();
  std::string getPath();
  int getDepth();
  glm::fmat4 getWorldTransform();
  glm::fmat4 getLocalTransform();
  bool getIsLight();

  // get methods of derived class
  virtual float getSize() const;
  virtual float getSpeed() const;
  virtual float getDistance() const;

  // set attribute methods
  void setParent(std::shared_ptr<Node> const& parent);
  void setWorldTransform(glm::fmat4 const& worldTransform);
  void setLocalTransform(glm::fmat4 const& localTransform);
  void setIsLight(bool isLight);

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
  glm::fmat4 worldTransform_;
  glm::fmat4 localTransform_;
  bool isLight_ = false;
};

#endif