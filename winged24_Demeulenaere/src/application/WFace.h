#pragma once

/**
@author F. Aubert
*/


#include "WEdge.h"
#include "Winged.h"
#include "Vector3.h"

class WFace
{
  public:
    WFace(Winged *owner);
    virtual ~WFace();

    void draw(bool withNormal);
    void drawLineCatmull();

    size_t index();
    void index(size_t i);

    Winged *owner();

    WEdge *edge();
    void edge(WEdge *e);

    const p3d::Vector3 &normal() const;
    void normal(const p3d::Vector3 &n);

    void computeNormal();

  protected:
  private:
  WEdge *edge_;

  p3d::Vector3 normal_;

  Winged *owner_;

  size_t index_;
};


