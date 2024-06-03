#include "IntersectionInfo.h"
#include "Line.h"
#include "CsgTree.h"
#include <math.h>
#include "Primitive.h"
#include "Ray.h"

#include <limits>


/**
@file
@author Fabrice Aubert
*/

using namespace p3d;
using namespace std;

const double MaxInfinity = numeric_limits<double>::max();
const double MinInfinity = numeric_limits<double>::lowest();

double IntersectionInfo::lambda() const {
    return lambda_;
}

IntersectionInfo::IntersectionInfo(double lambda) {
  lambda_=lambda;
  isIntersect_=true;
}

const Material &IntersectionInfo::material() const {
    return node_->primitive()->material();
}


void IntersectionInfo::lambda(double k) {
  isIntersect_=true;
    lambda_=k;
}

void IntersectionInfo::node(CsgTree *n) {
    node_=n;
}

const CsgTree *IntersectionInfo::node() const {
    return node_;
}


Ray IntersectionInfo::computeReflectRay(const Ray &r) const {
    Ray res;
    Vector3 P=r.point(lambda());
    res.point(P);

    Vector3 N=normal();

    N.normalize();
    Vector3 D=r.direction();
    D.normalize();

    // N = normale, D = rayon incident : R=?

    if (dot(N,D)>0) N=-N;

    Vector3 R=-2.0*(dot(D,N))*N+D;
    res.direction(R);
    res.inside(r.inside());
    return res;
}

Ray IntersectionInfo::computeRefractRay(const Ray &r) const {
  Ray res;
  /* TODO : inside/outside
    Ray res;
    double m1=1.0,m2=1.0; // coefficients des milieux pour descartes

    if (incident().inside()) {
      m1=node_->primitive()->material().refractionIndex();
    }
    else {
      m2=node_->primitive()->material().refractionIndex();
    }

    //M1=1.0;M2=1.0;
    Vector3 P=incident().point(lambda());
    res.point(P);
    node_->pointLocal(&P);
    Vector3 N=normal();
    N.normalize();
    Vector3 D=incident().direction();
    D.normalize();
    Vector3 T;
    double n=m1/m2;
    double ND=N.dot(D);
    if (ND<0) {N=-N;ND=-ND;}
    double sin22=1.0-n*n*(1.0-ND*ND);
    if (sin22 <0) { // angle critique => le rayon transmis est réfléchi
      T=-2.0*ND*N+D;
      res.inside(incident().inside());
    }
    else {
      T=n*D+(n*ND-sqrt(sin22))*N;
      res.inside(!incident().inside());
    }
    res.direction(T);
    return res;
    */
  return res;
}


void IntersectionInfo::computePointNormal(const Ray &r) {
    Vector3 P=r.point(lambda());
    point_=P;
    P=node_->pointLocal(P);
    normal_=node_->normalPrimitive(P);
    normal_=node_->normalWorld(normal_);
    normal_.normalize();
}




