#pragma once

#include <QXmlStreamReader>

#include "Vector3.h"
#include <string>
#include "Camera.h"
#include "CsgTree.h"
#include "Ray.h"
#include "IntersectionInfo.h"
#include "Material.h"
#include "Object3D.h"


class Scene : public p3d::Object3D {
  public:
    Scene();
    virtual ~Scene();

    void init();

    const std::string &name() {return name_;}
    void name(const std::string &name) {name_=name;}

    void read(const std::string &name);

    inline void csg(CsgTree *csg) {csg_=csg;}
    inline CsgTree *csg() {return csg_;}

    const p3d::Vector3 &lightPosition(int i) const;
    void lightPosition(int i,const p3d::Vector3 &p);
    void addLight(const p3d::Vector3 &p,double intensity=1.0);
    double lightIntensity(int i);

    inline unsigned int nbLight() {return light_.size();}

    inline const p3d::Camera &camera() const {return camera_;}
    inline void camera(const p3d::Camera &cam) {camera_=cam;}

    IntersectionInfo intersection(const Ray &ray,double seuil);

    void drawGL();

    void prepareCsg();



  protected:
    CsgTree *readNode(QXmlStreamReader *elem, p3d::Material currentMat);
  private:
    CsgTree *csg_;
    std::vector<p3d::Vector3> light_;
    std::vector<double> intensity_;

    p3d::Camera camera_;
    std::string name_;
};


