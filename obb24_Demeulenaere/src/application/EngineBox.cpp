#include "EngineBox.h"

#include <ctime>
#include <cstdlib>
#include <iostream>


#include <cmath>

#include "Collision.h"

#include "GLRender.h"
#include "GLDraw.h"
#include "Texture.h"

using namespace p3d;
using namespace std;


/** Mise à jour pour la génération de chaque image (appelée avant le tracé de la scène)
 * */
void EngineBox::update() {
  double dt=deltaTimeSimule(); // pas de temps (temps écoulé depuis l'image précédente)

  computeForce();              // affectation de toutes les forces
  euler(dt);                   // mouvement (mise à jour positions, vitesses, etc)

  interCollision();            // résolution collision entre boites
  collisionPlane();            // résolution collision avec plans fixes ("murs" de la scène)
  applyCorrection();           // application des corrections des positions, vitesses, etc calculées par la résolution des collisions

  previousTime_=currentTime(); // mise à jour pour le pas de temps à l'image suivante
}


/** Intégration Euler
 *  - dt est le pas de temps.
 *  - b.position()/b.position(une_position) permet de get/set la position de la boite b (similaire pour b.velocity, b.theta, b.omega)
 *  - b.force(), b.moment(), b.inertia() permettent de récupérer la force, le moment et le moment d'inertie de la boite
**/
void EngineBox::euler(double dt) {
  for(auto &b:boxList_.all()) {
    // à compléter
    b.velocity(b.velocity()+(b.force()/b.mass()*dt));
    b.omega(b.omega()+(b.moment()*dt));
    b.theta(b.theta()+b.omega().z()*dt);
    b.position(b.position()+(b.velocity()*dt));
//    b.position(b.position()+b.force()*dt);
//    b.theta(b.theta()+b.moment().z()*dt);

    // à laisser en fin :
    b.resetForce();
    b.resetMoment();
  }
}

/** Application des forces et des moments à chacune des boites
- b.addForce(un Vector3 f) => ajoute la force f à la boite b (appliquée au centre de masse)
- b.addMoment(un Vector3 m) => ajoute le moment m
- si l'attachement par un ressort est actif sur la boite b (cursorActive == true) :
  - Box &b=boxList_.selected() donne la boite qui subit le ressort (c'est une référence donc modifiable)
  - b.attachWorld() donne le point d'attache du ressort à cette boite dans le repère du monde
  - cursor_ donne la position de la souris (dans le repère du monde)
**/
void EngineBox::computeForce() {
  for(size_t i=0; i<boxList_.size(); ++i) {
    Box &b=boxList_[i];

    //b.addForce(Vector3(0,-9.81,0));
    //b.addMoment(Vector3(0,0,2));
    b.addForce(-b.velocity()/1.2);
    b.addMoment(-b.omega()/1.2);

// A compléter

  }
  if (cursorActive_) {
      Box &b = boxList_.selected();
      Vector3 l = cursor_-b.attachWorld();
      float k = 5;
      b.addForce(k*l);
      b.addMoment(cross(b.force(),b.position()-b.attachWorld()));

  }


}

/** applique le cumul des corrections calculées lors de la résolution des collisions
 * */
void EngineBox::applyCorrection() {
  for(auto &b:boxList_.all()) {

    b.positionCorrection();
    b.velocityCorrection();
    b.omegaCorrection();
  }
}


/** Gère la collision entre toutes les boites
**/
void EngineBox::interCollision() {
  for(size_t i=0; i<boxList_.size(); ++i) {
    Box &b1=boxList_[i];
    for(size_t j=i+1; j<boxList_.size(); ++j) {
      Box &b2=boxList_[j];
      Collision c;
      c.collision(b1,b2); // collision entre b1 et b2
    }
  }
}



/** ***************************************************************************************************************************** **/

EngineBox::EngineBox() {
  //ctor
  startTime();
}

EngineBox::~EngineBox() {
  //dtor
}





void EngineBox::add(const Plane &p) {
  planeList_.push_back(p);
}

void EngineBox::add(const Box &b) {
  boxList_.add(b);
}

void EngineBox::collisionPlane() {

  // pour chacun des plans
  for(auto &plane:planeList_) {
    // pour chacune des boites
    for(auto &b:boxList_.all()) {
      Collision c;
      c.collision(b,plane);

    }

  }
}







void EngineBox::draw() {
  boxList_.draw();
}

void EngineBox::activeCursor(const Vector3 &p) {
  boxList_.select(p);
  if (boxList_.isSelected()) {
    Box &b=boxList_.selected();
    b.enableVisualEffect(2);
    cursor_=p;
    b.attachWorld(p);
    cursorActive_=true;
  } else
    cursorActive_=false;
}

void EngineBox::motionCursor(const Vector3 &p) {
  if (cursorActive_) {
    cursor_=p;
  }
}

void EngineBox::disableCursor() {
  if (boxList_.isSelected()) {
    Box &b=boxList_.selected();
    b.disableVisualEffect(2);
    boxList_.unSelect();
  }
  cursorActive_=false;
}

void EngineBox::drawPlane2D() {
  p3d::ambientColor=Vector4(0.8,0.7,1,1);
  vector<Vector3> pts;
  for(unsigned int i=0; i<planeList_.size(); i++) {
    Plane pl=planeList_[i];
    Vector3 direction=cross(pl.normal(),Vector3(0,0,1));
    pts.push_back((pl.point()-100*direction));
    pts.push_back((pl.point()+100*direction));
  }
  p3d::shaderVertexAmbient();
  p3d::drawLines(pts);
}

bool EngineBox::isCursorActive() {
  return cursorActive_;
}

double EngineBox::deltaTimeSimule() {
  return (currentTime()-previousTime_)*factorTime_;
}

void EngineBox::startTime() {
  startTime_=std::chrono::steady_clock::now();
}

double EngineBox::currentTime() {
  return (std::chrono::duration<double>(std::chrono::steady_clock::now()-startTime_)).count();
}

double EngineBox::deltaTime() {
  return currentTime()-previousTime_;
}

void EngineBox::factorTime(double f) {
  factorTime_=f;
}

void EngineBox::mulFactorTime(double f) {
  factorTime_*=f;
}



void EngineBox::setBoxTexture(size_t i,std::string filename) {
  texture_.push_back(make_unique<Texture>());
  Texture *t=texture_.back().get();
  t->read(filename);
  t->filterLinear();
  boxList_[i].texture(t);

}

