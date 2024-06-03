/*
 * Implicite.h
 *
 *  Created on: 22 sept. 2010
 *      Author: aubert
 */

#pragma once

#include <vector>

#include "Vector2.h"
#include "Blob.h"

class Implicit {

  std::vector<Blob> blob_;

public:
  Implicit();

  int nbBlob() {return blob_.size();}
  std::vector<p3d::Vector2> allCenter();
  std::vector<float> allRadius();


  float distance(int index,const p3d::Vector2 &p);
  void addBlob(const p3d::Vector2 &center,double radius);
  void set(int index,const p3d::Vector2 &center,double radius);
  void computeForce(unsigned int index);

  double eval(const p3d::Vector2 &p) const;
  void move(double dt);
  void computeAllForce();
};


