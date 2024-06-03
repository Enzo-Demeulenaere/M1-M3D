#include "Cone.h"
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

Cone::Cone() : Primitive() {}



// donne la liste des réels k issus de l'intersection de l=A+kv et du cone x^2+y^2=z^2 (attention à la base : plan z=1)
IntersectionArray Cone::intersection(const Ray &ray) {

   /**
   */

  IntersectionArray res;

  Vector3 a=ray.point();
  Vector3 u=ray.direction();

  IntersectionArray cone; // for infinite cone
  IntersectionArray cap1=IntersectionArray::intersectPlane(a.z()-1,u.z()); // for cap (at z=1)
  IntersectionArray cap2=IntersectionArray::intersectPlane(-a.z(),-u.z()); // for cap (at z=0)
  IntersectionArray cap=IntersectionArray::intersectInterval(cap1,cap2);
  if (!cap.empty()) {
    double lambda1,lambda2;
    double A=u.x()*u.x()+u.y()*u.y()-u.z()*u.z();
    double B=2.0*(u.x()*a.x()+u.y()*a.y()-u.z()*a.z());
    double C=a.x()*a.x()+a.y()*a.y()-a.z()*a.z();
    if (fabs(A)<p3d::epsilon) { // only one intersection
      if (fabs(B)>p3d::epsilon) {
        lambda1=-C/B;
        lambda2=(u.z()>0)?MinInfinity:MaxInfinity;
        cone=IntersectionArray::fromInterval(lambda1,lambda2);
      }
    }
    else {
      double delta=B*B-4.0*A*C;
      if (delta>0) {
        lambda1=(-B+sqrt(delta))/2.0/A;
        lambda2=(-B-sqrt(delta))/2.0/A;
        swapIfMin(lambda1,lambda2);
        if (A<0) {
          if (u.z()<0) {
            lambda2=lambda1;lambda1=MinInfinity;
          }
          else {
            lambda1=lambda2;lambda2=MaxInfinity;
          }
        }
        cone=IntersectionArray::fromInterval(lambda1,lambda2);
      }
    }
  }
  res=IntersectionArray::intersectInterval(cap,cone);

  return res;
}

Vector3 Cone::computeNormal(const Vector3 &p) {
  if (fabs(p.z()-1.0)<0.001) {
    return Vector3(0,0,1);
  }
  else if (fabs(p.z())<0.0001) {
    return Vector3(0,0,-1);
  }
  else
    return Vector3(p.x(),p.y(),-p.z());
}


void Cone::drawGL() {
    materialGL();
    p3d::modelviewMatrix.push();
    p3d::shaderLightPhong();
    p3d::drawCone();
    p3d::modelviewMatrix.pop();
}

