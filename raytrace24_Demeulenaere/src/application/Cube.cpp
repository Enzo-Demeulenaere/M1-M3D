#include <cmath>
#include "Cube.h"
#include "Line.h"
#include "GLTool.h"
#include <iostream>
#include "IntersectionArray.h"


/**
@file
@author Fabrice Aubert
*/

using namespace p3d;
using namespace std;

Cube::Cube() : Primitive() {}



// intersection du cube
IntersectionArray Cube::intersection(const Ray &ray) {

  Vector3 a=ray.point();
  Vector3 u=ray.direction();

  IntersectionArray resX=IntersectionArray::intersectInterval(IntersectionArray::intersectPlane(-a.x()-1,-u.x()),
                                                            IntersectionArray::intersectPlane(a.x()-1,u.x()));
  IntersectionArray resY=IntersectionArray::intersectInterval(IntersectionArray::intersectPlane(-a.y()-1,-u.y()),
                                                            IntersectionArray::intersectPlane(a.y()-1,u.y()));
  IntersectionArray resXY=IntersectionArray::intersectInterval(resX,resY);
  IntersectionArray resZ=IntersectionArray::intersectInterval(IntersectionArray::intersectPlane(-a.z()-1,-u.z()),
                                                            IntersectionArray::intersectPlane(a.z()-1,u.z()));
  IntersectionArray resXYZ=IntersectionArray::intersectInterval(resXY,resZ);

  return resXYZ;
}

Vector3 Cube::computeNormal(const Vector3 &p) {
  Vector3 res(0,0,0);
  if ((fabs(p.x())>=fabs(p.y())) && (fabs(p.x())>=fabs(p.z())))
    res.x(1.0);
  else
  if ((fabs(p.y())>=fabs(p.x())) && (fabs(p.y())>=fabs(p.z())))
    res.y(1.0);
  else
    res.z(1.0);
  return res;
}

void Cube::drawGL() {
  materialGL();
  p3d::shaderLightPhong();
  p3d::drawCube();
}

