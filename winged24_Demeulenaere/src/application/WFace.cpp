#include "WFace.h"
#include "Tools.h"
#include "GLDraw.h"
#include "WVertex.h"

/**
@file
@author F. Aubert
@brief  Winged-Edges : opérations sur les sommets

*/


using namespace p3d;
using namespace std;

/** ************************************************************************************
** A COMPLETER : WFace::draw()
** On parcourt tous les sommets de la face en remplissant le tableau position (qui est tracé à la fin).
** - this->edge() : donne l'arête affectée à la face
** - si e de type WEdge * :
**    - e->begin(), e->end() : donne les sommets de l'arête (de type WVertex *)
**    - e->right(), e->left() : donne les faces gauche et droite (de type WFace *) (Remarque : e->left()==this est un test valide)
**    - e->succLeft(), e->succRight(), e->predLeft(), e->predRight() donnent les arêtes incidentes à droite et à gauche (de type WEdge *)
**    - v->position() donne les coordonnées du sommet v de type Vector3
**    - v->normal() donne les coordonnées de la normale calculée au sommet v de type Vector3
** *********************************************************************************** */

void WFace::draw(bool withNormal) {
  WEdge *e, *e_new, *start;
  WVertex *v;
  e=start=this->edge();

  vector<Vector3> position;
  position.clear();

  vector<Vector3> normal;
  normal.clear();

  // TODO : compléter
  do {
       if (e->left()==this){
           v = e->end();
           e_new = e->succLeft();
       }
       else{
           v = e->begin();
           e_new = e->succRight();
       }
       position.push_back(v->position());
       normal.push_back(v->normal());
       e = e_new;
  }while(e != start);




  // A Laisser à la fin (effectue un affichage polygone par polygone : lent => uniquement pour vérification/TP)
  if (withNormal)
    p3d::drawPolygon(position,normal,true);
  else
    p3d::drawPolygon(position,false,0.8f);

}

/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/





WFace::WFace(Winged *owner) {
  owner_=owner;
  edge_=nullptr;
  //ctor
}

WFace::~WFace() {
  //dtor
}

void WFace::edge(WEdge *e) {
  edge_=e;
}

WEdge *WFace::edge() {
  return edge_;
}

Winged *WFace::owner() {
  return owner_;
}

void WFace::index(size_t i) {
  index_=i;
}

size_t WFace::index() {
  return index_;
}

void WFace::normal(const Vector3 &n) {
  normal_=n;
}

const Vector3 &WFace::normal() const {
  return normal_;
}


void WFace::computeNormal() {
  WEdge *u,*v,*start;
  Vector3 uvect,vvect;
  Vector3 n;
  start=edge();
  u=edge();
  if (u->left()==this) uvect=u->direction(); else uvect=-u->direction();
  do {
    if (u->left()==this) {
      v=u->succLeft();
    }
    else {
      v=u->succRight();
    }
    vvect=v->direction();
    if (v->right()==this) vvect=-vvect;
    n=cross(uvect,vvect);
    u=v;
    uvect=vvect;
  } while (n.length2()<0.000000001 && (u!=start));
  if (u==start) {
    normal(Vector3(0,0,0)); // cas particulier : face aplatie
  }
  else {
    n.normalize();
    normal(n);
  }
}




