#include "Blob.h"
#include <cmath>

using namespace p3d;

/**
 * @brief Blob::Blob
 * @param center
 * @param radius
 */
Blob::Blob(const p3d::Vector2 &center,double radius) {
  center_=center;
  radius_=radius;
}

/**
 * @brief distance from point p to the center of the blob
 * @param p
 * @return distance(p,center_)
 */
double Blob::distance(const p3d::Vector2 &p) const {
  return p3d::distance(center_,p);
}

/**
 * @brief set force to f
 * @param f the force
 */
void Blob::force(const p3d::Vector2 &f) {
  force_=f;
}

/**
 * @brief cumulate the total force with the force f
 * @param f
 */
void Blob::addForce(const p3d::Vector2 &f) {
  force_+=f;
}

/**
 * @brief compute spring force between center and point p
 * @param p
 */
void Blob::attract(const p3d::Vector2 &p) {
  Vector2 f(0,0);
  // TODO
    f.x(p.x()-center_.x());
    f.y(p.y()-center_.y());
  addForce(f);

}

/**
 * @brief move the center of the blob according to the applied force and its velocity
 * @param dt
 */
void Blob::move(double dt) {
  // TODO : euler explicit

}

/**
 * @brief evaluate the height of the blob at point p
 * @param p : the point where the function is evaluate
 * @return the height
 */
double Blob::eval(const p3d::Vector2 &p) const {
  // TODO
//  float a = 2.5;
//  float b = 4.0;
//  float r = sqrtf(pow(p.x()-center_.x(),2)+pow(p.y()-center_.y(),2));
//  float res = a*exp(-b* r/radius_);
  float r = sqrtf(pow(p.x()-center_.x(),2)+pow(p.y()-center_.y(),2));
  if (r<radius_){
      float a = 4/9;
      float b = 17/9;
      float c = 22/9;
      float ratio = r/radius_;
      float res = 1-a*pow(ratio,6)+b*pow(ratio,4)-c*pow(ratio,2);

      return res;
  }
  return 0;


}

