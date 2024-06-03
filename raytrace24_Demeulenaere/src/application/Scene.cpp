#include <QXmlStreamReader>

#include "Scene.h"
#include "IntersectionArray.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cone.h"
#include "Cylinder.h"

#include "Tools.h"
#include "GLRender.h"
#include "GLPrimitive.h"

/**
@file
@author Fabrice Aubert
*/

using namespace std;
using namespace p3d;



/**
Calcul de l'intersection avec la scène : renvoie le point le plus proche parmi toutes les intersections calculées (par csg()->intersection).
Remarque : seuil permet de considérer uniquement les points tels que lambda>seuil
*/
IntersectionInfo Scene::intersection(const Ray &ray,double seuil) {
  IntersectionArray intersectionList;
  IntersectionInfo res;

  if (csg_) {
    intersectionList=csg_->intersection(ray); // calcul de l'intersection entre le rayon et l'arbre csg => au retour interList contient toutes les intersections
  }

  if (intersectionList.empty()) {
    return IntersectionInfo(); // isIntersection()==false for default IntersectionInfo
  }
  else {
  }

  // on sélectionne celle qui est la plus proche et dont le lambda est plus grand que seuil (pour par exemple considérer uniquement ce qui se trouve devant l'écran, et pas derriere avec lambda >0).

  for(auto inter:intersectionList) {
    if (inter.lambda()>seuil) {
      res=inter;
      break;
    }
  }

  if (res.isIntersection()) {
    res.computePointNormal(ray);
  }

  return res;

}

/** ********************************************************************** */


Scene::Scene() {
  //ctor
  csg_=nullptr;
  light_.clear();
  intensity_.clear();
  camera_.position(Vector3(0,0,0));
}

void Scene::init() {
  delete csg_;
  csg_=nullptr;
  light_.clear();
  intensity_.clear();
  camera_.position(Vector3(0,0,0));
}

Scene::~Scene() {
  //dtor
  delete csg_;
}

const Vector3 &Scene::lightPosition(int i) const {
  return light_[i];
}

double Scene::lightIntensity(int i) {
  return intensity_[i];
}

void Scene::addLight(const Vector3 &p,double intensity) {
  light_.push_back(p);
  intensity_.push_back(intensity);
}


CsgTree *Scene::readNode(QXmlStreamReader *doc,Material currentMat) {
  CsgTree *res=nullptr;
  QXmlStreamAttributes attribute;
  QStringRef value;
  res=new CsgTree();
  unsigned int node=0;
  if (doc->name()=="node") {
    attribute=doc->attributes();
    value=attribute.value("operation");
    if (value=="difference") {
      res->nodeType(CsgTree::Op_Diff);
    }
    else if (value=="union") {
      res->nodeType(CsgTree::Op_Union);
    }
    else if (value=="intersection") {
      res->nodeType(CsgTree::Op_Inter);
    }

  }
  else if (doc->name()=="primitive") {
    attribute=doc->attributes();
    value=attribute.value("type");
    if (value=="cube") {
      res->primitive(new Cube());
    }
    else if (value=="sphere") {
      res->primitive(new Sphere());
    }
    else if (value=="cone") {
      res->primitive(new Cone());
    }
    else if (value=="cylinder") {
      res->primitive(new Cylinder());
    }

  }
  Matrix4 translate,rotate,scale;
  translate=Matrix4::identity();
  rotate=Matrix4::identity();
  scale=Matrix4::identity();
  double x,y,z,angle,red,green,blue,shininess,coefficient;
  doc->readNext();
  while(doc->tokenType()!=QXmlStreamReader::EndElement || (doc->name()!="node" && doc->name()!="primitive")) {
    if (doc->tokenType()==QXmlStreamReader::StartElement) {
      attribute=doc->attributes();
      if (doc->name()=="translate") {
        x=attribute.value("x").toString().toDouble();
        y=attribute.value("y").toString().toDouble();
        z=attribute.value("z").toString().toDouble();
        translate=Matrix4::fromTranslation(Vector3(x,y,z));
      }
      else if (doc->name()=="rotate") {
        x=attribute.value("x").toString().toDouble();
        y=attribute.value("y").toString().toDouble();
        z=attribute.value("z").toString().toDouble();
        angle=attribute.value("angle").toString().toDouble();
        rotate=Matrix4::fromAngleAxis(angle,Vector3(x,y,z));
      }
      else if (doc->name()=="scale") {
        x=attribute.value("x").toString().toDouble();
        y=attribute.value("y").toString().toDouble();
        z=attribute.value("z").toString().toDouble();
        angle=attribute.value("angle").toString().toDouble();
        scale=Matrix4::fromScale(Vector3(x,y,z));
      }
      else if (doc->name()=="diffuse") {
        red=attribute.value("red").toString().toDouble();
        green=attribute.value("green").toString().toDouble();
        blue=attribute.value("blue").toString().toDouble();
        currentMat.diffuse(Vector3(red,green,blue));
      }
      else if (doc->name()=="ambient") {
        red=attribute.value("red").toString().toDouble();
        green=attribute.value("green").toString().toDouble();
        blue=attribute.value("blue").toString().toDouble();
        currentMat.ambient(Vector4(red,green,blue,1.0));
      }
      else if (doc->name()=="specular") {
        red=attribute.value("red").toString().toDouble();
        green=attribute.value("green").toString().toDouble();
        blue=attribute.value("blue").toString().toDouble();
        shininess=attribute.value("shininess").toString().toDouble();
        currentMat.specular(Vector3(red,green,blue));
        currentMat.shininess(shininess);
      }
      else if (doc->name()=="reflection") {
        coefficient=attribute.value("coefficient").toString().toDouble();
        currentMat.reflectionCoefficient(coefficient);
      }
      else if (doc->name()=="refraction") {
        coefficient=attribute.value("coefficient").toString().toDouble();
        currentMat.refractionCoefficient(coefficient);
      }
      else if ((doc->name()=="node") || (doc->name()=="primitive")) {
        node++;
        if ((node>2) || res->isLeaf()) throw ErrorD("Reading CSG : node found into primitive or there are more than 2 children");
        if (node==1) res->left(readNode(doc,currentMat));
        else res->right(readNode(doc,currentMat));
      }

    }
    doc->readNext();
  }
  res->matrix(translate*rotate*scale);
  if (res->isLeaf()) {
    res->primitive()->material(currentMat);
  }

  return res;
}

void Scene::read(const string &resource) {
  init();
  QFileInfo qfile=resourceFile(resource);
  QFile file(qfile.absoluteFilePath());
  file.open(QIODevice::ReadOnly);
  QXmlStreamReader doc(&file);
  QXmlStreamAttributes attribute;


  double x,y,z,intensity;

  Material currentMat;
  while (!doc.atEnd() && !doc.hasError()) {
    QXmlStreamReader::TokenType token=doc.readNext();
    if (doc.hasError()) cout << doc.errorString().toStdString() << endl;
    if (token==QXmlStreamReader::StartElement) {
      if (doc.name()=="scene") continue;
      else if ((doc.name()=="node") || (doc.name()=="primitive")) {
        csg_=readNode(&doc,currentMat);
      }
      else if (doc.name()=="light") {
        attribute=doc.attributes();
        x=attribute.value("x").toString().toDouble();
        y=attribute.value("y").toString().toDouble();
        z=attribute.value("z").toString().toDouble();
        intensity=attribute.value("intensity").toString().toDouble();
        addLight(Vector3(x,y,z),intensity);
      }
    }
  }
  file.close();
}


void Scene::drawGL() {


  p3d::modelviewMatrix.push();

  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);




  for(unsigned int i=0; i<min(nbLight(),p3d::nbLight); ++i) {
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.translate(lightPosition(i).x(),lightPosition(i).y(),lightPosition(i).z());
    p3d::modelviewMatrix.scale(0.2,0.2,0.2);
    p3d::ambientColor=Vector4(0,1,0);
    p3d::shaderVertexAmbient();
    p3d::drawSphere();
    p3d::modelviewMatrix.pop();
  }

  for(unsigned int i=0;i<min(nbLight(),p3d::nbLight);++i) {
    p3d::lightPosition[i]=camera_.cameraWorld()*Vector4(lightPosition(i),1.0);
    p3d::lightIntensity[i]=lightIntensity(i);
  }

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);


  if (csg_) csg_->drawParent();
  p3d::modelviewMatrix.pop();
}

void Scene::prepareCsg() {
  if (csg_) csg_->cumulateMatrix();
}







