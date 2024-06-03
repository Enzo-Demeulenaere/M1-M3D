#pragma once

class IntersectionArray;

#include "Primitive.h"

class Cylinder:public Primitive {
  typedef enum {FULL_IN, FULL_OUT, INTERSECT_IN} ESpaceIntersect;

    public:
    Cylinder();
    //Cylinder(const p3d::Vector4 &a,const p3d::Vector3 &d,const p3d::Vector3 &s);
    p3d::Vector3 computeNormal(const p3d::Vector3 &p) override;
    void drawGL() override;
    IntersectionArray intersection(const Ray &ray) override;

private:
    IntersectionArray intersectCap(double a,double u);
};


