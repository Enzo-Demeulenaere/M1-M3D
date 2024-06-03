#include "Collision.h"

#include "Box.h"
#include "DebugTool.h"
#include "Plane.h"

#include <algorithm>
#include <vector>

using namespace p3d;
using namespace std;


bool Collision::visualDebug_=true;

Collision::Collision() {
}

/** détecte la collision entre b1 et b2 et donne les informations nécessaires à la réponse dans *collision
  - principe : il faut déterminer les distances de recouvrement sur chacun des 4 axes possibles (2 axes pour b1 et 2 axes pour b2).
    si on trouve une distance négative, c'est que les boites ne se recouvrent pas sur cet axe => pas de collision (arrêt).
    si toutes les distances sont positives : b1 et b2 sont en intersection, et on renseigne *collision :
    - collision->mtd=un double : distance minimal sur les 4 axes (i.e. distance nécessaire pour séparer les 2 boites : sera utilisé pour la correction en position)
    - collision->normale=un Vector3 : l'axe de la distance minimal qui donne la normale au contact (i.e. direction de séparation)
    - collision->apply=un Vector3 : le point d'application qui sera utilisé pour l'impulsion (utilisé pour le calcul du moment de la force d'impulsion) :
      Le point d'application est déjà calculé (moyenne de tous les sommets intérieurs aux boites).
**/
void Collision::detectCollision(const Box &b1,const Box &b2) {


  vector<Vector3> axis(4); // 4 axes à tester
  Vector3 axis_min; // l'axe qui correspond à la plus petite distance de recouvrement (nécessaire pour la réponse par la suite)
  double dist_min; // plus petite distance de recouvrement (nécessaire pour la réponse par la suite)

  // les 4 axes potentiellement séparateurs
  axis[0]=b1.directionX(); // axe x de b1 (exprimé dans repère global)
  axis[1]=b1.directionY(); // axe y de b1
  axis[2]=b2.directionX(); // axe x de b2
  axis[3]=b2.directionY(); // axe y de b2

  // Completer (le code déjà présent illustre seulement l'instruction à copier-coller pour avoir une visualisation des intervalles sur les axes testés) :
  // - affectez la distance minimale dist_min : c'est la distance de recouvrement entre les 4 axes axis[i] qui sont non séparateurs (Attention : minimale en valeur absolue ! dist_min est négative s'il y a recouvrement)
  // - vous devez affecter correctement axis_min (l'axe correspondant à dist_min) qui est un des axis[i] *mais* en tenant compte du sens de séparation de
  //   b2 par rapport à b1 (i.e. multiplier axis[i] par le signe (-1 ou 1) renvoyé par la méthode distance(b1,b2,...,)).
  // - assurez vous d'avoir affecté correctement isCollide_ à la fin (==true il y a collision, == false pas de collision).
  // DEBUT Compléter

  float intersections = 0;
  dist_min = INFINITY;
  for (int i =0; i <4; i++){
      DistanceInfo d=distance(b1,b2,axis[i]);
      if (d.distance_<0){
          if (visualDebug_) p3d::addDebug(b1.position(),b1.position()-d.direction_*d.distance_*axis[i],"",Vector3(0.2,0.2,1));
          intersections ++;
      }
      if (abs(d.distance_) < dist_min){
          dist_min = abs(d.distance_);
          axis_min = axis[i]*d.direction_;
      }
  }

  isCollide_ = intersections == 4;


  // FIN Compléter

  // exploite votre dist_min et axis_min pour affecter les informations nécessaires à la réponse
  // mtd_ = "minimum translational distance"
  // axis_ = normale à la collision
  // applicationPoint_ = le point qui va subir l'impulsion
  if (isCollide_) {
    mtd_=dist_min;
    axis_=axis_min;

    // Calcul du point d'application de l'impulsion (ici barycentre des sommets intérieurs).
    // on créé la liste de tous les sommets inclus dans une des boites // TODO : faire plus direct sur un seul point
    vector<Vector3> liste;
    liste.clear();
    for(size_t i=0; i<4; ++i) { // 4 vertices for each box
      if (b1.isInside(b2.vertex(i))) liste.push_back(b2.vertex(i));
    }

    for(size_t i=0; i<4; ++i) {
      if (b2.isInside(b1.vertex(i))) liste.push_back(b1.vertex(i));
    }

    Vector3 apply=Vector3(0,0,0);
    for(unsigned int i=0; i<liste.size(); i++) {
      apply+=liste[i];
    }
    apply/=liste.size();

    applicationPoint_=apply;

  }

}


void Collision::collision(Box &b1,Box &b2) {

  detectCollision(b1,b2); // set isCollide_ (and, if true, set all parameters for the response)
  if (isCollide_) {
    b1.enableVisualEffect(1);
    b2.enableVisualEffect(1); // sert uniquement à faire un retour visuel de collision.

    Vector3 normal=axis_; // normale à la collision calculée lors de la détection
    Vector3 r1=b1.position()-applicationPoint_; // position relative du point de collision dans la boite b1; applicationPoint_ est le point de collision calculé lors de la détection de collision
    Vector3 r2=b2.position()-applicationPoint_; // position relative du point de collision dans la boite b2

    double impulse=impulseCoefficient(b1,b2); // calcul du coefficient d'impulsion entre les 2 boites (utilise applicationPoint_)




    // début Réponse à la collision :

    // calcul de la direction d'impulsion
    Vector3 imp=impulse*normal;

    // correction du mouvement :
    // - on corrige la vitesse angulaire et la vitesse des boites
    // - on corrige aussi la position (car on répond à une situation en recouvrement après le contact).
    b1.addVelocityCorrec(imp/b1.mass());
    b1.addOmegaCorrec(cross(imp,r1)/b1.inertia());

    b2.addVelocityCorrec(-imp/b2.mass());
    b2.addOmegaCorrec(-cross(imp,r2)/b2.inertia());

    b1.addPositionCorrec(normal*mtd_); // mtd_ = distance de recouvrement calculée lors de la détection
    b2.addPositionCorrec(-normal*mtd_);
    // fin réponse à la collision


  }
  else {
    b1.disableVisualEffect(1); // pas d'effet visuel de détection de collision
    b2.disableVisualEffect(1);
  }
}





DistanceInfo Collision::distance(const Box &b1,const Box &b2, const Vector3 &axis) {
  DistanceInfo res; // res.distance_ = distance de séparation (sera positive si boites séparées; négative si boites en recouvrement)
                    // res.direction_ = 1 (b2 peut être séparée de b1 dans la direction +axis) ou -1 (b2 peut être séparée de b1 dans la direction -axis)
  double dist;
  Interval i1=b1.project(axis); // l'intervalle de projection de la boite b1
  Interval i2=b2.project(axis); // l'intervalle de projection de la boite b2

  if (visualDebug_) drawDebugProjection(b1,b2,axis,i1,i2);

  if(i1.inf_< i2.inf_){
      res.distance_ = i2.inf_ - i1.sup_;
      res.direction_ = -1;
  }
  else{
      res.distance_ = i1.inf_ - i2.sup_;
      res.direction_ = +1;
  }

// A completer
  // affecter la distance de recouvrement (res.distance_ = ?)
  // affecter res.direction_  (-1 ou 1 ?)



  return res;
}

/************************************************************************** */
/************************************************************************** */
/************************************************************************** */
/************************************************************************** */

/*
 * @precondition : detectCollision was called (and true)
 *
*/
double Collision::impulseCoefficient(const Box &b1,const Box &b2) {
  Vector3 normal=axis_;
  normal.normalize();
  Vector3 r1=b1.position()-applicationPoint_;

  Vector3 r2=b2.position()-applicationPoint_;

  Vector3 r1xN=-cross(r1,normal);
  Vector3 r2xN=-cross(r2,normal);

  // calcul de l'impulsion (coefficient de restitution fixé à 0.5)
  double impulseNum;
  double impulseDen;

  impulseNum=-(1+0.5)*(dot(normal,b1.velocity()-b2.velocity())+dot(b1.omega(),r1xN)-dot(b2.omega(),r2xN));
  impulseDen=1.0/b1.mass()+1.0/b2.mass()+1.0/b1.inertia()*dot(r1xN,r1xN)+1.0/b2.inertia()*dot(r2xN,r2xN);

  return impulseNum/impulseDen;

}

double Collision::impulseCoefficient(const Box &b, const Plane &plane) {
  Vector3 normal=normalize(plane.normal());

  // on raisonne sur le point de collision (applicationPoint_) trouvé lors de la détection
  Vector3 r1=b.position()-applicationPoint_;

  // on calcule l'impulsion (restitution à 0.5 ici)
  Vector3 r1xN=-cross(r1,normal);
  double impulseNum;
  double impulseDen;

  impulseNum=-(1+0.5)*(dot(normal,b.velocity())+dot(b.omega(),r1xN)); // la vitesse du point de collision dépend de la vitesse de la boite et de la vitesse angulaire de la boite
  impulseDen=1.0/b.mass()+1.0/b.inertia()*dot(r1xN,r1xN);

  return impulseNum/impulseDen;

}

void Collision::detectCollision(const Box &b,const p3d::Plane &plane) {
  isCollide_=false;
  std::vector<Vector3> list;
  list.clear();
  for(unsigned int k=0;k<4;k++) {
    list.push_back(b.vertex(k));
  }

  size_t plus_loin=0;
  double dist=plane.distance(list[0]);
  for(size_t k=1; k<list.size(); ++k) {
    double comp=plane.distance(list[k]);
    if (comp<dist) {
      dist=comp;
      plus_loin=k;
    }
  }
  Vector3 c_choice=list[plus_loin];
  // c_choice correspond au sommet le plus loin, dist donne la distance de recouvrement

  if (dist<0) {
    isCollide_=true;
    applicationPoint_=c_choice;
    mtd_=dist;
  }
}

void Collision::collision(Box &b,const Plane &plane) {
  detectCollision(b,plane);

  if (isCollide_) { // il y a collision
    Vector3 normal=normalize(plane.normal());
    Vector3 r1=b.position()-applicationPoint_;

    double impulse=impulseCoefficient(b,plane);
    Vector3 force=impulse*normal; // "force" d'impulsion

    b.addVelocityCorrec(force/b.mass()); // correction en vitesse
    b.addOmegaCorrec(cross(force/b.inertia(),r1)); // correction en vitesse angulaire

    Vector3 PH;
    PH=dot(plane.point()-applicationPoint_,normal)/normal.length2()*normal;
    b.addPositionCorrec((1.0+0.5)*PH); // correction en position


  }

}


void Collision::drawDebugProjection(const Box &b1,const Box &b2,const Vector3 &axis,Interval i1,Interval i2) {

  Vector3 ref=0.5*(b1.position()+b2.position());
  double lambda=dot(ref,axis);
  Vector3 decal=cross(axis,Vector3(0,0,1));
  decal.normalize();
  decal=decal/10.0;
  p3d::addDebug(ref+decal+(i1.inf_-lambda)*axis,ref+decal+(i1.sup_-lambda)*axis,"",b1.color());
  p3d::addDebug(ref-decal+(i2.inf_-lambda)*axis,ref-decal+(i2.sup_-lambda)*axis,"",b2.color());
}

