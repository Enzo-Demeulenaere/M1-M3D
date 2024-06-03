#pragma once

#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4.h"

class BasicCamera
{
public:
  BasicCamera();
  p3d::Vector3 position;
  p3d::Quaternion orientation;

private:
  /* TODO : manipulate frustum, viewport
  float xView_{0},yView_{0},wView_{100},hView_{100}; // viewport
  float left_{-1},right_{1},bottom_{-1},top_{1},zNear_{0.1},zFar_{100};


  p3d::Matrix4 projection();
  p3d::Matrix4 worldEye();
  */
};


