#pragma once

#include "Vector3.h"
#include "Ray.h"
#include "CsgTree.h"
#include "IntersectionInfo.h"
#include "Scene.h"
#include <QThread>
#include <QImage>




class Raytrace : public QThread {
public:

  virtual ~Raytrace();
  Raytrace(unsigned int width,unsigned int height);

  Raytrace() : Raytrace(300,300) {} // resolution

    void close();
    void run();
    inline void scene(Scene *s) {scene_=s;}

    QImage *image() {return image_;}

private:
    Scene *scene_;
    QImage *image_;
    p3d::Camera camera_;
    unsigned int width_,height_;
    bool stopRequest_;

    // boucle principale (calcul pour chaque pixel)
    void computeImage();

    // calcul de la couleur portée par un rayon (récursif si objets réfléchissant ou réfractant)
    p3d::Vector3 computeRayColor(const Ray &ray, int level, double contribution);

    // calcule de la couleur par le modèle de Phong en un point d'intersection
    p3d::Vector3 computeLocalColor(const IntersectionInfo &inter,const Ray &ray);


 };



