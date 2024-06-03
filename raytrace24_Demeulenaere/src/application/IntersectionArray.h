#pragma once

#include "IntersectionInfo.h"
#include "CsgTree.h"
#include <vector>


class IntersectionArray : public std::vector<IntersectionInfo> {
  public:
  /// set this to the fusion of left and right with operation op (union, intersection or difference)^
    static IntersectionArray fusion(const IntersectionArray &left, const IntersectionArray &right, CsgTree::ENode op);

  ///
    static IntersectionArray fromInterval(double lambda1,double lambda2);

  /// set this to the intersection of i1 and i2. i1 and i2 should be intervals (particular case of fusion)
    static IntersectionArray intersectInterval(const IntersectionArray &i1,const IntersectionArray &i2);

  /// set this to the intersection ray/plane where plane has the normal (1,0,0) ( or (0,1,0) or (0,0,1) )
  /// a is the x coordinate (or y or z) of the ray origin, and u the x coordinate (or y or z) of the ray direction
    static IntersectionArray intersectPlane(double a,double u);

  /// set this to the intersection with the leaf t (t should have a primitive)
    static IntersectionArray intersectLeaf(CsgTree *t,const Ray &r);


  protected:
  private:
};


