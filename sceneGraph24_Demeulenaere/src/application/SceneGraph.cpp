#include <QXmlStreamReader>
#include <QDebug>
#include <iostream>

#include "SceneGraph.h"
#include "Tools.h"

#include "Quaternion.h"
#include "Vector3.h"
#include "BasicVAO.h"
#include "BasicRender.h"

using namespace p3d;
using namespace std;


void Node::draw() {

  tp::modelview.push();  // On garde le repere parent
  tp::modelview.translate(this->position);
  tp::modelview.rotate(this->orientation);
  tp::modelview.scale(this->scale);
  if (mesh) {
    tp::setupShader();
    mesh->draw();
  }


  for(Node *n:child) {
      n->draw();          // On descends dans l'arbre
  }
  tp::modelview.pop();    // Une fois plus d'enfants, on remonte

}


/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/



Node *Node::find(const std::string &name) {
  if (this->name==name) return this;

  for(Node *n:child) {
    Node *res=n->find(name);
    if (res) return res;
  }
  return nullptr;
}


SceneGraph::~SceneGraph() {
  // clean/delete all nodes from root
  // TODO
}

SceneGraph::SceneGraph() {
}


void SceneGraph::draw() {
  if (!root) return;

  tp::modelview.push();

  root->draw();

  tp::modelview.pop();
}


Node *SceneGraph::readNode(QXmlStreamReader *doc,p3d::Vector3 currentColor) {
   Node *res=new Node();

   QXmlStreamAttributes attribute;
   QStringRef value;

   attribute=doc->attributes();
   value=attribute.value("name");
   res->name=value.toString().toStdString();
   res->color=currentColor;

   doc->readNext();

   double x,y,z,angle;

   while (!doc->isEndElement()) {
     if (doc->isStartElement()) {
       attribute=doc->attributes();
       cout << "NAME = " << doc->name().toString().toStdString() << endl;
       if (doc->name()=="mesh") {
         value=attribute.value("type");
         if (value=="cube") res->mesh=&tp::cube;
         if (value=="torus") res->mesh=&tp::torus;
         if (value=="cylinder") res->mesh=&tp::cylinder;
         doc->readNext();
       }
       if (doc->name()=="position") {
         x=attribute.value("x").toString().toDouble();
         y=attribute.value("y").toString().toDouble();
         z=attribute.value("z").toString().toDouble();
         res->position.set(x,y,z);
         doc->readNext();
       }
       if (doc->name()=="orientation") {
         angle=attribute.value("angle").toString().toDouble();
         x=attribute.value("x").toString().toDouble();
         y=attribute.value("y").toString().toDouble();
         z=attribute.value("z").toString().toDouble();
         res->orientation=Quaternion::fromAngleAxis(angle,x,y,z);
         doc->readNext();
       }
       if (doc->name()=="scale") {
         x=attribute.value("x").toString().toDouble();
         y=attribute.value("y").toString().toDouble();
         z=attribute.value("z").toString().toDouble();
         res->scale.set(x,y,z);
         doc->readNext();
       }
       if (doc->name()=="node") {
         Node *child=readNode(doc,currentColor);
         res->child.push_back(child);
         child->parent=res;
       }
     }
     doc->readNext();
   }
   return res;
}


void SceneGraph::read(const std::string &filename) {
  QFileInfo qfile=resourceFile(filename);
  QFile file(qfile.absoluteFilePath());
  file.open(QIODevice::ReadOnly);
  QXmlStreamReader doc(&file);
  if (doc.hasError()) {
    qDebug() << doc.errorString() << Qt::endl;
  }
  doc.readNext();

  while (!doc.isEndDocument()) {
    if (doc.isStartElement()) {
      if (doc.name()=="node") {
        root=readNode(&doc,Vector3(1,0,0));
      }
    }
    doc.readNext();
  }
  file.close();
}

Node *SceneGraph::find(const std::string &name) {
  if (!root) return nullptr;

  return root->find(name);
}


