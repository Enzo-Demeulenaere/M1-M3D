#pragma once

#include <cmath>

#include "Vector3.h"

/**
@file
@author F. Aubert
@brief AngleAxis class (only container : see Quaternion/Matrix4 for usual operations)
*/

namespace p3d {

class Quaternion;
class Matrix4;

class AngleAxis {
public:
  explicit AngleAxis(double angle=0.0,const Vector3 &axis=Vector3(0,0,1)):angle_{angle},axis_{axis} {}
  explicit AngleAxis(double angle=0.0,double x=0,double y=0,double z=1):angle_{angle},axis_{x,y,z} {}


  double angleDegree() const {return angle_;}
  double angleRadian() const {return M_PI*angle_/180.0;}

  const Vector3 &axis() const {return axis_;}

  Quaternion toQuaternion() const;
  Matrix4 toMatrix4() const;
  Vector3 rotate(const Vector3 &u);

private:
  double angle_;
  Vector3 axis_;
};

double interpolateAngle(double angle0, double angle1, double lambda);
double wrap360(double angle);
}


