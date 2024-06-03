#include "WVertex.h"
#include "Tools.h"
#include "GLDraw.h"

#include "WEdge.h"
#include "Winged.h"
#include "WFace.h"


/**
@file
@author F. Aubert
@brief  Winged-Edges : opérations sur les sommets

*/


using namespace p3d;
using namespace std;



/** ************************************************************************************
** A COMPLETER : WVertex::computeNormal() => calculer la moyenne des normales aux faces incidentes au sommet. Cette normale est affectée au sommet à la fin
** On parcourt toutes les faces du sommet dans l'ordre direct
** - this->edge() : donne l'arête affectée à la face (début du parcours)
** - si e de type WEdge * :
**    - e->begin(), e->end() : donne les sommets de l'arête (de type WVertex *)
**    - e->right(), e->left() : donne les faces gauche et droite (de type WFace *)
**    - e->succLeft(), e->succRight(), e->predLeft(), e->predRight() donnent les arêtes incidentes à droite et à gauche (de type WEdge *)
** - si f de type WFace * :
**    - f->normal() donne la normale à f (de type Vector3).
** - sur les Vector3 on peut faire les opérations v1+v2,v1/k, v.normalize(), ...
** *********************************************************************************** */


void WVertex::computeNormal() {
  Vector3 average(0,0,0); // à calculer

  // TODO : compléter
  vector<Vector3> normals;
  WEdge *e, *start;
  e = start = this->edge();
  // premiere implementation pour les boundaries (certaines normales manquantes)
//  do{
//    if (e->begin()==this){
//        if (e->left()){
//            normals.push_back(e->left()->normal());
//            e = e->predLeft();
//        }
//        else {
//            e = start;
//        }
//    }
//    else{
//        if (e->right()){
//            normals.push_back(e->right()->normal());
//            e = e->predRight();
//        }
//        else e = start;
//    }
//  }while(e != start);

  // seconde implémentation qui essaie de récuperer toutes les normales
  bool borderFound = false;
  do{
    if (!borderFound) {
        if (e->begin()==this){
            if (e->left()){
                normals.push_back(e->left()->normal());
                e = e->predLeft();
            }
            else {
                borderFound = true;
                e = start;
            }
        }
        else{
            if (e->right()){
                normals.push_back(e->right()->normal());
                e = e->predRight();
            }
            else {
                borderFound = true;
                e = start;
            }
        }
    }
    if (borderFound){
        if (e->begin()==this){
            if (e->right()){
                normals.push_back(e->right()->normal());
                e = e->succRight();
            }
            else {
                e = start;
            }
        }
        else{
            if (e->left()){
                normals.push_back(e->left()->normal());
                e = e->succLeft();
            }
            else e = start;
        }
    }

  }while(e != start);


  // Calcul de la moyenne
  for (Vector3 n:normals){
      average.x(average.x()+n.x());
      average.y(average.y()+n.y());
      average.z(average.z()+n.z());
  }
  average.x(average.x()/normals.size());
  average.y(average.y()/normals.size());
  average.z(average.z()/normals.size());

  // A LAISSER à la fin
  average.normalize();
  this->normal(average);
}


/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/
/** ************************************************************************************ **/



WVertex::~WVertex() {
  //dtor
}

WVertex::WVertex(const Vector3 &p,Winged *owner) {
  position_=p;
  owner_=owner;
  edge_=NULL;
}


WEdge *WVertex::indirectBoundary() {
  WEdge *e=edge();
  bool found=false;
  do {
    if (this==e->begin()) {
      if (e->right()==NULL) found=true;
      else e=e->succRight();
    }
    else {
      if (e->left()==NULL) found=true;
      else e=e->succLeft();
    }
  } while (!found && (e!=edge()));

  if (found) return e; else return edge();
}


Winged *WVertex::owner() {
  return owner_;
}

void WVertex::checkVertexBound() {

  unsigned int nbBound=0;
  Winged *w=owner_;
  WEdge *e;
  for(unsigned int i=0;i<w->nbEdge();i++) {
    e=w->edge(i);
    if (((e->begin()==this) || (e->end()==this)) && ((e->left()==NULL) || (e->right()==NULL))) {
      nbBound++;
    }
  }
  if (nbBound>2) {
    cout << "WARNING !!!! vertex check : nbBound =" << nbBound << endl;
  }

}

WEdge *WVertex::directBoundary() {
  WEdge *e=edge();
  bool found=false;
  do {
    if (this==e->begin()) {
      if (e->left()==NULL) found=true;
      else e=e->predLeft();
    }
    else {
      if (e->right()==NULL) found=true;
      else e=e->predRight();
    }
  } while (!found && (e!=edge()));

  if (found) return e; else return edge();
}


const Vector3 &WVertex::normal() const {
  return normal_;
}

void WVertex::normal(const Vector3 &n) {
  normal_=n;
}

WEdge *WVertex::edge() {
  return edge_;
}

void WVertex::edge(WEdge *e) {
  edge_=e;
}

const Vector3 &WVertex::position() const {
  return position_;
}

void WVertex::position(const Vector3 &p) {
  position_=p;
}

void WVertex::index(size_t i) {
  index_=i;
}

size_t WVertex::index() {
  return index_;
}



