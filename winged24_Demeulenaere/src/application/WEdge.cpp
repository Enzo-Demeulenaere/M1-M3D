
#include "WEdge.h"

#include <vector>

#include "glsupport.h"
#include "Tools.h"

#include "WFace.h"
#include "WVertex.h"
#include "GLDraw.h"


/**
@file
@author F. Aubert

@brief Winged-Edges : opérations sur les arêtes
*/

using namespace p3d;
using namespace std;




/** ************************************************************************************ */

Vector3 WEdge::direction() {
  return this->end()->position()-this->begin()->position();
}


// start should be a boundary edge
void WEdge::followBoundary() {
  WEdge *e;
  WEdge *next;
  e=this;
  unsigned int number=0;
  do {
      if (e->left()) next=e->end()->directBoundary();
      else if (e->right()) next=e->begin()->directBoundary();
      else throw ErrorD("WEdge::followBoundary : not a boundary edge");
      e->succLine(next);
      e->mark();
      e=next;
      number++;
  } while (e!=this);
}


WEdge::~WEdge() {
  //dtor
}

WEdge::WEdge(WVertex *b,WVertex *e,Winged *owner) {
  begin_=b;
  end_=e;
  owner_=owner;
}

Winged *WEdge::owner() {
  return owner_;
}

WVertex *WEdge::begin() const {
  return begin_;
}

WVertex *WEdge::end() const {
  return end_;
}

void WEdge::begin(WVertex *b) {
  begin_=b;
}

void WEdge::end(WVertex *e) {
  end_=e;
}

WFace *WEdge::left() const {
  return left_;
}

WFace *WEdge::right() const {
  return right_;
}

void WEdge::left(WFace *f) {
  left_=f;
}

void WEdge::right(WFace *f) {
  right_=f;
}

WEdge *WEdge::succLeft() const {
  return succLeft_;
}

WEdge *WEdge::succRight() const {
  return succRight_;
}

WEdge *WEdge::predLeft() const {
  return predLeft_;
}

WEdge *WEdge::predRight() const {
  return predRight_;
}

void WEdge::succLeft(WEdge *e) {
  succLeft_=e;
}

void WEdge::succRight(WEdge *e) {
  succRight_=e;
}
void WEdge::predLeft(WEdge *e) {
  predLeft_=e;
}
void WEdge::predRight(WEdge *e) {
  predRight_=e;
}


void WEdge::draw() {
}


size_t WEdge::index() {
  return index_;
}

void WEdge::index(size_t i) {
  index_=i;
}



void WEdge::mark() {
  mark_=true;
}

void WEdge::unmark() {
  mark_=false;
}

bool WEdge::isMark() {
  return mark_;
}

void WEdge::drawLine() {
  vector<int> a;
  vector<Vector3> pts;
  pts.clear();
  unsigned int num=0;
  WEdge *e,*start;
  WVertex *v;
  start=this;
  e=start;
  if (start->succLine()) {
    if (begin()==succLine()->begin()) v=end(); else v=begin();
  }
  else{
    return;
  }
  do {
    pts.push_back(e->begin()->position());
    pts.push_back(e->end()->position());
    e=e->succLine();
    if (e) {if (v==e->begin()) v=e->end(); else v=e->begin();}
    num++;
  } while ((e!=start) && e);

  p3d::drawLines(pts);
}


void WEdge::succLine(WEdge *e) {
  succLine_=e;
}

WEdge *WEdge::succLine() {
  return succLine_;
}

ostream& operator <<(std::ostream &s,const WEdge &e) {
    s << "(begin = " << e.begin() << ", end= " << e.end() << "left,right = " <<  e.left() << "," << e.right() << " succLeft = " << e.succLeft() << " succRight= " << e.succRight() << " predLeft = " << e.predLeft() << " predRight=" << e.predRight();
    return s;
}




