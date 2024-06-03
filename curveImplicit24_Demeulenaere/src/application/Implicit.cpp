/*
 * Implicite.cpp
 *
 *  Created on: 22 sept. 2010
 *      Author: aubert
 */
#include <cmath>
#include "Implicit.h"

using namespace std;
using namespace p3d;

Implicit::Implicit() {
}

/**
 * @brief Implicit::allCenter
 * @return all center of the blobs
 */
vector<Vector2> Implicit::allCenter() {
  vector<Vector2> res;
  for(Blob &b:blob_) {
    res.push_back(b.center());
  }
  return res;
}

/**
 * @brief Implicit::allRadius
 * @return all radius of the blobs
 */
vector<float> Implicit::allRadius() {
  vector<float> res;
  for(Blob &b:blob_) {
    res.push_back(float(b.radius()));
  }
  return res;
}

/**
 * @brief Implicit::addBlob
 * @param center
 * @param radius
 */
void Implicit::addBlob(const p3d::Vector2 &center, double radius) {
  blob_.push_back(Blob(center,radius));
}


/**
 * @brief compute the forces for all blobs
 */
void Implicit::computeAllForce() {
  for(unsigned int i=0;i<blob_.size();++i) {
    computeForce(i);
  }
}
/**
 * @brief compute force applied to the index-th blob by all other blobs
 * @param index : the index of the blob
 */
void Implicit::computeForce(unsigned int index) {
  blob_[index].force(0);
  // TODO : compute attraction, friction, ...

}

/**
 * @brief Implicit::move : move each blob
 * @param dt : delta time (duration between 2 moves)
 */
void Implicit::move(double dt) {
  for(Blob &b:blob_) {
    b.move(dt);
  }
}


/**
 * @brief Implicit::eval
 * @param p
 * @return the value of the implicit function (the height) of the point p
 */
double Implicit::eval(const p3d::Vector2 &p) const {
// TODO


  //return p.x()*p.x()+p.y()*p.y()-1.0;
   float sum = 0;
   for (int i=0; i<int(blob_.size());i++){
       sum += blob_[i].eval(p);
   }
   return 1-sum;

}


