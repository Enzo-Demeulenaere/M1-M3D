#pragma once

class IntersectionArray;

#include "Primitive.h"

class Cone:public Primitive {
  typedef enum {FULL_IN, FULL_OUT, INTERSECT_IN,INTERSECT_OUT} ESpaceIntersect;

    public:
    Cone();
    //Cone(const p3d::Vector4 a,const p3d::Vector3 d,p3d::Vector3 &s);
    p3d::Vector3 computeNormal(const p3d::Vector3 &p) override;
    void drawGL() override;
    IntersectionArray intersection(const Ray &ray);

private:
    IntersectionArray intersectCap(const Ray &r);

};

