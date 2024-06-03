#pragma once

#include <QXmlStreamReader>

#include "Vector3.h"
#include "Quaternion.h"

class BasicVAO;


class Node {
public:

  void draw();

  std::string name{""};

  p3d::Vector3 position;
  p3d::Quaternion orientation;
  p3d::Vector3 scale{p3d::Vector3(1,1,1)};

  p3d::Vector3 color{p3d::Vector3(1,0,0)};

  Node *parent{nullptr}; // SceneGraph is the owner (new/delete delegated to SceneGraph class)
  std::vector<Node *> child{}; // SceneGraph is the owner (new/delete delegated to SceneGraph class; **no sharing** (move only))

  BasicVAO *mesh{nullptr}; // BasicRender is the owner (new/delete delegated to BasicRender)

  Node *find(const std::string &name);
};

class SceneGraph
{
public:
  SceneGraph();
  virtual ~SceneGraph();

  void draw();

  void read(const std::string &filename);
  Node *find(const std::string &name);

  Node *root{nullptr};

protected:
  Node *readNode(QXmlStreamReader *doc,p3d::Vector3 currentColor);

private:
};


