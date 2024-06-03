#pragma once

class IntersectionArray;

#include "Primitive.h"

class Sphere:public Primitive {
    public:
    Sphere();
    p3d::Vector3 computeNormal(const p3d::Vector3 &p) override;
    void drawGL() override;
    IntersectionArray intersection(const Ray &ray) override;
};


