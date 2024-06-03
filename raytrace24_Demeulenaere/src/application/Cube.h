#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED

class IntersectionArray;

#include "Primitive.h"

/**
@file
@author Fabrice Aubert
*/


class Cube: public Primitive {

public:
  Cube();
  virtual p3d::Vector3 computeNormal(const p3d::Vector3 &p);
  void drawGL();

  IntersectionArray intersection(const Ray &ray) override;


};

#endif // CUBE_H_INCLUDED

