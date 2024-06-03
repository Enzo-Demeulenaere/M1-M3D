#include "Material.h"
#include "CsgTree.h"
#include "Primitive.h"
#include "Raytrace.h"
#include <cmath>
#include "Line.h"
#include <iostream>
#include "Ray.h"
#include "GLTool.h"
#include <ctime>
#include <cmath>


/**
@file
@author Fabrice Aubert
*/


using namespace p3d;
using namespace std;


/**
Calcul de la couleur pour un rayon donné : la fonction sera récursive lors de l'ajout des rayons réfléchis
 - level détermine la profondeur de récursivité (pour limiter la profondeur de récursivité);
 - contribution donne le pourcentage (contrib \in [0,1]) de la contribution à l'éclairage total (cumul par appels récursif) : si la contribution devient trop faible, on arrête la récursivité
*/
Vector3 Raytrace::computeRayColor(const Ray &ray, int level, double contribution) {

  /**
  * Remarques :
  - intersection.material().reflexionCoefficient() donnera le coefficient de contribution à la réflexion (i.e. 1=réflexion totale, 0=réflexion nulle)
  - intersection.computeReflectRay(ray) : permet de calculer le rayon réfléchi au point d'intersection
  */


  Vector3 color=Vector3(1,1,1);

  if (level>0 && contribution>0.001) {

    // Intersection de la scène avec le rayon (donne uniquement l'intersection la "plus proche").
    IntersectionInfo intersection=scene_->intersection(ray,0.1); // 0.1 pour prendre une intersection qui se trouve un peu devant le "point de départ" (origine) du rayon

    if (intersection.isIntersection()) { // existe-t-il une intersection avec la scène ?

      //return Vector3(1,0,0);

      color=computeLocalColor(intersection,ray); // calcul de la couleur par Phong

      Vector3 colorReflect,colorRefract;
      double cReflect=intersection.material().reflectionCoefficient();
      if (cReflect>0.001) {
        Ray r=intersection.computeReflectRay(ray);
        colorReflect=cReflect*computeRayColor(r,level-1,contribution*cReflect);
      }
      double cRefract=intersection.material().refractionCoefficient();
      if (cRefract>0.001) {
        Ray r=intersection.computeRefractRay(ray);
        colorRefract=cRefract*computeRayColor(r,level-1,contribution*cRefract);
      }
      color=(1.0-cReflect-cRefract)*color+cReflect*colorReflect+cRefract*colorRefract;
      return color;

    }

  }


  return color;
}


/**
  Calcul de l'éclairement local d'un point d'intersection => Phong avec ombres portées.
  - toutes les données nécessaires au point d'intersection sont dans le paramêtre intersection (point, normale, noeud CSG intersecté)
  - les données de la scène (sources lumineuses) sont accessibles par scene()->...
*/
Vector3 Raytrace::computeLocalColor(const IntersectionInfo &intersection,const Ray& ray) {
  /**
  * P est le point d'intersection (Vector3)
  * L est le vecteur d'éclairement (Vector3)
  * N est la normale au point d'intersection (Vector3)
  * V est le vecteur d'observation
  * m contient le materiel au point : m.diffuse() donne le matériel diffus (de type Vector3 : on peut utiliser les opérateurs *, +, etc), de même m.specular(), m.shininess()
  * intersection.incident() donne le rayon qui a provoqué l'intersection
  * Pour les sources :
  *   - _scene->nbLight() donne le nombre de source lumineuses
  *   - _scene->lightPosition(i) donne la position de la source i (uniquement des sources ponctuelles).
  * Remarque : il faut faire la somme des couleurs obtenues pour chacune des sources (risque de saturation si plusieurs sources lumineuses).
  */

  Vector3 P;
  Vector3 L;
  Vector3 N;
  Vector3 V;
  N=intersection.normal();
  P=intersection.point();
  // V= ?, L=?
  Material m=intersection.node()->primitive()->material();


  Vector3 result=m.ambient().xyz();

  V=-ray.direction();
  N.normalize();
  V.normalize();
  if (dot(N,V)<0) N=-N;
  for(unsigned int i=0;i<scene_->nbLight();i++) {
    L=scene_->lightPosition(i)-P;
    Ray shadow(P,L);
    IntersectionInfo occulte=scene_->intersection(shadow,0.01);
    bool isShadow=((occulte.isIntersection()) && (occulte.lambda()<1.0));
    isShadow=false;
    L.normalize();
    if (!isShadow) {
      double diffus=max(dot(N,L),0.0);
      result+=diffus*m.diffuse();
      if (diffus>0) { // si pas éclairé => pas de calcul du spéculaire
        Vector3 R=2.0*(dot(N,L))*N-L;
        double spec=max(dot(R,V),0.0);
        result+=pow(spec,m.shininess())*m.specular();
      }
    }
  }


  return result;
}


/** *************************************************************** **/
/** *************************************************************** **/
/** *************************************************************** **/
/** *************************************************************** **/
/** *************************************************************** **/
/** *************************************************************** **/
/** *************************************************************** **/
void Raytrace::run() {
  stopRequest_=false;
  computeImage();
}


/**
Boucle principale du lancer de rayon

*/
void Raytrace::computeImage() {
  camera_=scene_->camera();
  camera_.viewport(0,0,width_,height_);
  Vector3 eye(0.0,0.0,0.0);
  Vector3 pixel_eye; // pixel dans le repère observateur
  image_->fill(Qt::black);

  clock_t clockStart=clock();

  Matrix4 sceneCamera=scene_->localWorld()*camera_.worldCamera();


  for(unsigned int y=0; y<height_; ++y) {
    for(unsigned int x=0; x<width_; ++x) {
      if (stopRequest_) goto fin;
      pixel_eye=camera_.windowToCamera(x,y); // exprime le rayon dans le repère de l'observateur.

      Ray ray=Ray(eye,pixel_eye);  // rayon primaire
      ray.transform(sceneCamera);  // dans repère world
      Vector3 c=computeRayColor(ray,4,1.0); // calcule la couleur du pixel; 4=profondeur max de récursion, 1.0=contribution; tous les calculs sont entendus dans le repère G
      // mise à jour de la couleur du pixel dans l'image résultante
      c.clamp(0,1);
      QColor color=QColor::fromRgbF(c.r(),c.g(),c.b());

      image_->setPixel(x,y,color.rgba()); // affecte à l'image la couleur calculée
    }
  }
fin:
  clock_t clockElapsed=clock()-clockStart;
  cout << "Raytracing finished in " << double(clockElapsed)/CLOCKS_PER_SEC << " seconds" << endl;
}



/** *********************************************************************************************************** */
/** *********************************************************************************************************** */
/** *********************************************************************************************************** */

Raytrace::Raytrace(unsigned int width,unsigned int height) {
    image_=new QImage(width,height,QImage::Format_ARGB32);
    width_=width;
    height_=height;
}



Raytrace::~Raytrace() {
    close();
    delete image_;
}



void Raytrace::close() {
  if (isRunning()) {
    stopRequest_=true;
    while (isRunning()) {
      cout << "Im waiting" << endl;
      usleep(100);
    }
  }
}








