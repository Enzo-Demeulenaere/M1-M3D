#pragma once

#include <vector>
#include "Ray.h"
#include "Vector3.h"
#include "Material.h"
#include "p3helpers.h"
//#include "IntersectionRay.h"
class IntersectionArray;


class Primitive {
    p3d::Material material_;
    public:

    virtual ~Primitive() {}

    void material(const p3d::Material &m) {material_=m;}
    const p3d::Material &material() const {return material_;}

    virtual IntersectionArray intersection(const Ray &r)=0;

    void materialGL();

    virtual p3d::Vector3 computeNormal(const p3d::Vector3 &p)=0;

    virtual void drawGL()=0;
};



