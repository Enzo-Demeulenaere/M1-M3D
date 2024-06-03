#include "Sphere.h"
#include <cmath>
#include <iostream>
#include "p3helpers.h"
#include "GLRender.h"
#include "GLPrimitive.h"
#include "IntersectionArray.h"

/**
@file
@author Fabrice Aubert
*/

using namespace p3d;
using namespace std;

Sphere::Sphere() : Primitive() {}


/**
  donne la liste croissante issus de l'intersection de ray=(A,u)  et de la sphere x^2+y^2+z^2=1
  (2 ou 0 intersection)
*/
IntersectionArray Sphere::intersection(const Ray &ray) {

  /**
   * A COMPLETER : il faut résoudre l'équation en lambda (cf cours), puis ajouter dans le résultat res les intersections correspondantes aux lambda  :
   * Pour ajouter une intersection : res.push_back(IntersectionInfo(lambda))
   * ray est déjà dans le repère local de la sphere unitaire centrée à l'origine :
   *   - ray.point() : donne l'origine du rayon (Vector3)
   *   - ray.direction() : donne le vecteur directeur (Vector3)
   * il faut impérativement renvoyer 0 ou 2 intersections dans res pour assurer une cohérence dans l'intersection (intersection droite/volume)
   * il faut que les intersections soient triées (ordre croissant) pour assurer le fonctionnement avec l'algorithme général d'intersection avec le CSG
   * vous n'avez pas à vous préoccuper de compléter les IntersectionInfo (référence du noeud CSG intersecté par exemple) autres que le lambda  : cela est effectué dans le squelette par l'appelant
   *
   *  fonctions utiles :
   *  - dot(a,b) donne le produit scalaire de a par b
   *  - swapIfMin(a,b) : permute les valeurs de a et b pour que a <= b
   */

  IntersectionArray res;
  res.clear(); // initialisation : liste d'intersection vide

  // résoudre I=A+lambda*u (rayon) et dot(I,I)=1 (sphere unitaire)
  Vector3 A = ray.point();
  Vector3 u = ray.direction();

  double a = dot(u,u);
  double b = 2*dot(A,u);
  double c = dot(A,A)-1;

  double delta = pow(b,2) - (4*a*c);

  if (delta > 0){
      // 2 solutions
      double l1 = (-b + sqrt(delta))/(2*a);
      double l2 = (-b - sqrt(delta))/(2*a);

      swapIfMin(l1,l2);
      res.push_back(IntersectionInfo(l1));
      res.push_back(IntersectionInfo(l2));
  }

  return res;
}


/** ****************************************************** **/
/** ****************************************************** **/
/** ****************************************************** **/
/** ****************************************************** **/

Vector3 Sphere::computeNormal(const Vector3 &p) {
  return p;
}


void Sphere::drawGL() {
  materialGL();
  p3d::shaderLightPhong();
  p3d::drawSphere();
}

