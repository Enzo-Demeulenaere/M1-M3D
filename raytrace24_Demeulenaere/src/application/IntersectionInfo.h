#pragma once

class CsgTree;
#include "Ray.h"
#include "Material.h"


/***
@file
@author Fabrice Aubert
@brief Un point d'intersection avec la scène
**/

extern const double MaxInfinity;
extern const double MinInfinity;

class IntersectionInfo {
    public:
    IntersectionInfo(double lambda);
    IntersectionInfo() {}


    Ray computeReflectRay(const Ray &r) const;
    Ray computeRefractRay(const Ray &r) const;
    bool isIntersection() {return isIntersect_;}
    const p3d::Material &material() const;
    double lambda() const;
    void lambda(double k);
    void node(CsgTree *n);
    const CsgTree *node() const;

    void normal(const p3d::Vector3 &n) {normal_=n;}
    const p3d::Vector3 &normal() const {return normal_;}

    void point(const p3d::Vector3 &p) {point_=p;}
    const p3d::Vector3 &point() const {return point_;}

    void computePointNormal(const Ray &r);

private:
    bool isIntersect_{false}; // set to true when lambda is set

    CsgTree *node_{nullptr}; // référence sur primitive intersectée dans le CSG (node_ doit correspondre à une feuille de l'arbre CSG)

    p3d::Vector3 point_{}; // le point d'intersection (world)
    p3d::Vector3 normal_{}; // la normale à la primitive au point d'intersection (world)

    double lambda_{0}; // le point d'intersection est donné par point_ = A + lambda_ * u (lambda_ indépendant du repère)

};


