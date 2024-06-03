#include "Cylinder.h"
#include <cmath>
#include "GLTool.h"
#include <iostream>
#include "IntersectionArray.h"

/**
@file
@author Fabrice Aubert
*/

using namespace p3d;
using namespace std;

Cylinder::Cylinder() : Primitive() {}



// donne la liste des réels k issus de l'intersection de l=A+kv et du Cylindre x^2+y^2=1 (attention aux bases : plan z=1, z=-1)
IntersectionArray Cylinder::intersection(const Ray &ray) {

   /** TODO
   */

  IntersectionArray res;
  return res;
}

Vector3 Cylinder::computeNormal(const Vector3 &p) {
  if (fabs(p.z()-1.0)<0.001) {
    return Vector3(0,0,1);
  }
  else if (fabs(p.z()+1)<0.0001) {
    return Vector3(0,0,-1);
  }
  else
    return Vector3(p.x(),p.y(),0);
}


void Cylinder::drawGL() {
    materialGL();
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix.translate(0,0,-1);
    p3d::modelviewMatrix.scale(1,1,2);
    p3d::shaderLightPhong();
    p3d::drawCylinder();
    p3d::modelviewMatrix.pop();
}

