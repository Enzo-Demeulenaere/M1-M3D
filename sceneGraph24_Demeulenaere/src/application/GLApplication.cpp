#include "GLApplication.h"

#include "Vector2.h"

#include "BasicRender.h"
#include "Shader.h"

#include <iostream>

/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/


using namespace p3d;
using namespace std;

GLApplication::~GLApplication() {
  makeCurrent();
  tp::release();
}

GLApplication::GLApplication() {
  leftPanelMenu_ << "Step 0" << "Step 1" << "Step 2" << "Step 3";


  scene_.read("example0.xml");


  tp::modelview=Matrix4::identity();
  tp::projection=Matrix4::identity();

}

void GLApplication::initialize() {
  glClearColor(1,1,1,1);

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearDepth(1);
  glPointSize(5.0);
  glFrontFace(GL_CCW);

  tp::initPrimitive();
  tp::shader.read("basic_light");


}

/** ***************************************************************** **/
void GLApplication::resize(int,int) {
}


void GLApplication::update() {
  angle_+=1;
  angle_=fmod(angle_,360);

  Node *n=scene_.find("n1");
  if (n) {n->orientation.rotate(1,0,0,1);}
  n=scene_.find("root");
  if (n) {n->orientation.rotate(1,0,1,0);}

  angleCam_+=stepAngleCam_;
  if (angleCam_>45 || angleCam_<-45) stepAngleCam_=-stepAngleCam_;
  camera_.orientation=Quaternion::fromAngleAxis(angleCam_,1,0,0);
  camera_.position=Vector3(3,0,10);


    Vector2 mouse(mouseX(),mouseY());
    if (left()) {
    }
    if (right()) {
    }
    if (forward()) {
    }
    if (backward()) {
    }
}


void GLApplication::draw0() {

  tp::projection=Matrix4::fromFrustum(-0.1,0.1,-0.1,0.1,0.1,100);
  tp::modelview=Matrix4::identity();
  // On recule le cube
  tp::modelview.translate(0,0,-5);
  // On tourne le cube dans le sens horaire
  //tp::modelview.rotate(15,0,0,-1);
  // On tourne periodiquement le cube dans le sens anti horaire
  tp::modelview.rotate(angle_,1,1,1);

  tp::setupShader();
  tp::cube.draw();

  // pour un nouveau cube :
  tp::modelview.translate(3,0,0);           // On bouge la modelview
  tp::materialDiffuseFront=Vector3(0,0,1);  // On change la couleur
  tp::modelview.rotate(angle_,1,0,0);
  tp::setupShader();                        // Il faut resetup le shader
  tp::cube.draw();                          // On draw à nouveau le cube


}

void GLApplication::draw1() {
    tp::projection=Matrix4::fromFrustum(-0.1,0.1,-0.1,0.1,0.1,100);
    tp::modelview=Matrix4::identity();
    // On recule le cube
    tp::modelview.translate(0,0,-10);
    // On tourne le cube dans le sens horaire
    //tp::modelview.rotate(15,0,0,-1);
    // On tourne periodiquement le cube dans le sens anti horaire
    tp::modelview.rotate(angle_,1,1,1);

    tp::setupShader();
    tp::cube.draw();

    // Cube 1
    tp::modelview.push();                     // On sauvegarde le M_Eye->CubeRouge
    tp::modelview.translate(3,0,0);           // On bouge la modelview
    tp::materialDiffuseFront=Vector3(0,0,1);  // On change la couleur
    tp::modelview.rotate(angle_,1,0,0);
    tp::setupShader();                        // Il faut resetup le shader
    tp::cube.draw();                          // On draw à nouveau le cube

    // Tore 1
    tp::modelview.push();                     // On garde le repère du cube 1
    tp::materialDiffuseFront=Vector3(1,0,1);  // On passe en violet
    tp::modelview.translate(0,3,0);
    tp::modelview.rotate(angle_,1,0,0);
    tp::setupShader();
    tp::torus.draw();
    // Tore 2
    tp::modelview.pop();                      // On retourne sur le cube 1
    tp::modelview.translate(0,-3,0);
    tp::modelview.rotate(angle_,1,0,0);
    tp::setupShader();
    tp::torus.draw();

    // Cube 2
    tp::materialDiffuseFront=Vector3(0,0,1);  // On repasse en bleu
    tp::modelview.pop();                      // On récupère le repère
    tp::modelview.push();                     // On place à nouveau le repère dans la pile
    tp::modelview.translate(0,3,0);
    tp::modelview.rotate(angle_,1,0,0);
    tp::setupShader();
    tp::cube.draw();
    // Cube 3
    tp::modelview.pop();
    tp::modelview.push();
    tp::modelview.translate(-3,0,0);
    tp::modelview.rotate(angle_,1,0,0);
    tp::setupShader();
    tp::cube.draw();
    // Cube 4
    tp::modelview.pop();
    tp::modelview.push();
    tp::modelview.translate(0,-3,0);
    tp::modelview.rotate(angle_,1,0,0);
    tp::setupShader();
    tp::cube.draw();

}

void GLApplication::draw2() {
  tp::projection=Matrix4::fromFrustum(-0.1,0.1,-0.1,0.1,0.1,100);
  tp::modelview=Matrix4::identity();

  tp::modelview.push();
  tp::modelview.translate(0,0,-10);
  scene_.draw();
  tp::modelview.pop();
}

void GLApplication::draw3() {
  tp::projection=Matrix4::fromFrustum(-0.1,0.1,-0.1,0.1,0.1,100);
  tp::modelview=Matrix4::identity();

  tp::modelview.push();

  tp::modelview.rotate(camera_.orientation.inverse()); // On applique la rotation inverse
  tp::modelview.translate(-camera_.position);          // Puis la translation inverse

  scene_.draw();
  tp::modelview.pop();
}

void GLApplication::draw4() {

}


void GLApplication::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  tp::lightPosition=Vector3(0,0,0);
  tp::materialDiffuseFront=Vector3(1,0,0);
  tp::materialDiffuseBack=Vector3(0,1,0);

  switch(viewStep_) {
  case 0:draw0();break;
  case 1:draw1();break;
  case 2:draw2();break;
  case 3:draw3();break;
  case 4:draw4();break;
  }

}


void GLApplication::leftPanel(size_t i,const std::string &) {
  viewStep_=i;
}

