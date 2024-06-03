#pragma once

#include "Vector2.h"

class Blob
{
public:
  Blob(const p3d::Vector2 &center, double radius);

  double distance(const p3d::Vector2 &p) const;
  void force(const p3d::Vector2 &f);
  void addForce(const p3d::Vector2 &f);
  void attract(const p3d::Vector2 &p);
  void move(double dt);
  double eval(const p3d::Vector2 &p) const;

  const p3d::Vector2 &center() const {return center_;}
  void center(const p3d::Vector2 &p) {center_=p;}

  double radius() {return radius_;}
  void radius(double radius) {radius_=radius;}

private:
  p3d::Vector2 center_{0,0};
  double radius_{1};

  p3d::Vector2 velocity_{0,0};
  p3d::Vector2 force_{0,0};
};


