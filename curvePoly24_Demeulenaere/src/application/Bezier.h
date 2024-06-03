#pragma once

#include "Curve.h"

class Bezier : public Curve
{
public:
  /// De Casteljau evaluation
  p3d::Vector3 eval(double t);

  void draw();
};


