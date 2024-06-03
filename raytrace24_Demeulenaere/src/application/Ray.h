#ifndef RAY_H
#define RAY_H

#include "Vector3.h"
#include "Line.h"
#include "Matrix4.h"

class Ray : public p3d::Line {
  public:
    Ray();

    Ray(const p3d::Vector3 &p,const p3d::Vector3 &u);
    Ray(const Ray &ray);

    Ray &operator=(const Ray &r) {Line::operator=(r);inside_=r.inside();return *this;}

    void transform(const p3d::Matrix4 &m);

    void inside(bool a) {inside_=a;}
    bool inside() const {return inside_;}

  protected:
  private:
    bool inside_; //
};

#endif // RAY_H

