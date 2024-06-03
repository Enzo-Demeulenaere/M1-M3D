#include "FaceBSP.h"
#include <math.h>

using namespace p3d;
using namespace std;

/**
* @author Fabrice Aubert
*/

/** ********************************************************
  Trois méthodes à compléter pour le TP : sign, intersection et separe

*/


/// détermine le coté du point p par rapport à la face this (le résultat de type ESign peut être Sign_Minus ou Sign_Plus)
ESign FaceBSP::sign(const Vector3 &p) const {
  /// méthodes utiles :
  /// - vertex_[i].position() : les coordonnées (de type Vector3) du i-ième sommet de la face (i commence à 0)
  /// - normal_ : la normale (de type Vector3) de la face
  /// - dot(v1,v2) : produit scalaire entre 2 Vector3
  /// - Vector3 v=p2-p1 : pour obtenir le vecteur p1p2


  ESign res=Sign_None;

  Vector3 v = p - vertex_[0].position();
  float sign = dot(v,normal_);
  if (sign >= 0){
    res = Sign_Plus;
  }
  else res = Sign_Minus;

  return res;
}


/// Donne le point d'intersection de la face this avec le segment [v1v2] (précondition v1 et v2 de signes distincts par rapport à this)
VertexBSP FaceBSP::intersection(const VertexBSP &v1,const VertexBSP &v2) const {
  /// il suffit de résoudre AI.n=0 (plan (A,n)) et I=v1+k*v1v2 (droite donnée par (v1,v2))
  /// - position(i) : vous donne les coordonnées (type Vector3) du sommet i de la face
  /// - normal() : vous donne la normale de la facette (type Vector3)
  /// - v1.position() et v2.position() pour avoir les positions des sommets v1 et v2
  /// - quelques opérateurs sur les Vector3 peuvent être utiles. Exemple : u=b-a, p=a+k*u, etc (avec a,b,p et u de type Vector3)
  /// - dot(u1,u2) : produit scalaire


//  VertexBSP res;

//  Vector3 u = v2.position()-v1.position();
//  Vector3 ap = position(0)-v1.position();
//  double lambda = dot(ap,normal())/dot(u,normal());

//  res.position(v1.position() + u*lambda);
//  res.normal(v1.normal());


//  return res;


  // second version
  VertexBSP res;

  Vector3 u = v2.position()-v1.position();
  Vector3 ap = position(0)-v1.position();
  double lambda = dot(ap,normal())/dot(u,normal());

  double seuil = 0.01;

  if (fabs(dot(u,normal())) < seuil){
      res.position(v1.position() + u*0.5);
  }
  else{
    res.position(v1.position() + u*lambda);
  }

  // calcul normal
  double k= (v1.position()-res.position()).length() / (v1.position()-v2.position()).length();
  res.normal((1.0-k)*v1.normal()+k*v2.normal());

  //res.normal(v1.normal());


  return res;


}


/// coupe f selon this (i.e. f est découpée par le plan porteur de this) :
/// précondition : f doit être **convexe**
void FaceBSP::separe(const FaceBSP &f,FaceBSP &fPositive,FaceBSP &fNegative) const {
  /// - il faut séparer les sommets de f pour les ajouter dans les faces fPositive et fNegative (algo de séparation vu en cours)
  /// - VertexBSP a=f.vertex(i) vous donne le i-ième sommet de la face f.
  /// - f.size() vous donne le nombre de sommet de f.
  /// - fNegative.add(v) ajoutera le sommet v à fNegative
  /// - lors d'une intersection : il faut ajouter le sommet obtenu à fPositive et fNegative


  fNegative.clear();
  fPositive.clear();

  // start with last Vertex
  VertexBSP vLast = f.vertex(f.size()-1);
  ESign formerSign = sign(vLast.position());
  if (formerSign == Sign_Plus){
      fPositive.add(vLast);
  }
  else fNegative.add(vLast);

  // start for loop
  for (int i = 0; i<int(f.size()); i++){
      VertexBSP v = f.vertex(i);
      ESign vSign = sign(v.position());
      if (vSign == formerSign){
          if (vSign == Sign_Plus){
              fPositive.add(v);
          }
          else fNegative.add(v);
      }
      else{
          VertexBSP middleVertex = intersection(vLast,v);
          fNegative.add(middleVertex);
          fPositive.add(middleVertex);

          if (vSign == Sign_Plus){
              formerSign = Sign_Plus;
              fPositive.add(v);
          }
          else{
              formerSign = Sign_Minus;
              fNegative.add(v);
          }
      }
      vLast = v;
  }


  /** affecte la normale de f aux 2 nouvelles faces **/
  fPositive.normal(f.normal());
  fNegative.normal(f.normal());

}

/** ************************************************************************ */
/** ************************************************************************ */
FaceBSP::~FaceBSP() {
}

FaceBSP::FaceBSP() {
}



void FaceBSP::add(const VertexBSP &v) {
//  if (_tabVertex.empty() || (((_tabVertex[0]->point().distance2(v->point())>0.0000001) && ((_tabVertex[_tabVertex.size()-1]->point().distance2(v->point())>0.000001)))))
  vertex_.push_back(v);
}

Vector3 FaceBSP::normal() const {return normal_;}
void FaceBSP::normal(const Vector3 &n) {normal_=n;}

void FaceBSP::clear() {
  vertex_.clear();
}

