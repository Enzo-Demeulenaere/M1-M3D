/**

  @author F. Aubert
  **/


#include "GLApplication.h"
#include "GLTool.h"
#include <iostream>
#include <sstream>


using namespace std;
using namespace p3d;


/// ctor
GLApplication::GLApplication() {
  leftPanelMenu_ << "Start raytrace" << "Next scene";

  cameraGL_.frustum(-frustum_,frustum_,-frustum_,frustum_,0.3,1000);
  cameraGL_.position(0,2,100);
  cameraGL_.lookAt(Vector3(0,0,0));

  cameraRaytrace_.ortho(-1,1,-1,1,-1,1);

  sceneNumber_=0;

  loadScene(sceneNumber_);




}

/// dtor
///
GLApplication::~GLApplication() {

}


void GLApplication::initialize() {
  // appelée 1 seule fois à l'initialisation du contexte
  // => initialisations OpenGL

  glClearColor(1,1,1,1);

  glLineWidth(4);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearDepth(1);

  initGLTool();
  glPointSize(4.0);

  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glPixelStorei(GL_PACK_ALIGNMENT,1);

  raytraceTexture_.init(raytrace_.image()->width(),raytrace_.image()->height(),GL_RGBA);


}



/** ***************************************************************************
resize
  **/


void GLApplication::resize(int width,int height) {
  // appelée à chaque dimensionnement du widget OpenGL
  // (inclus l'ouverture de la fenêtre)
  // => réglages liés à la taille de la fenêtre
  cameraGL_.viewport(0,0,width/2,height);
  cameraRaytrace_.viewport(width/2,0,width/2,height);
}





void GLApplication::update() {
  updateCamera();

  if (keyPressed(Qt::Key_Space)) {
    restartRaytrace();
  }
  if (mouseRightPressed()) {
    Vector3 a=cameraGL_.windowToCamera(mouseX(),mouseY());
    //Vector3 u=a;
    Ray r(Vector3(0,0,0),a);
    r.transform(scene_.localWorld()*cameraGL_.worldCamera());
    scene_.intersection(r,0);
    cout << "raycast done" << endl;
  }
}

void GLApplication::leftPanel(int i,const std::string &s) {
  cout << "GLApplication : button clicked " << i << " " << s << endl;
  switch(i) {
  case 0:
    restartRaytrace();
    break;
  case 1:
    raytrace_.close();
    sceneNumber_=(sceneNumber_+1)%7;
    switch (sceneNumber_) {
    case 0:cameraGL_.position(0,2,100);break;
    case 2:case 3:case 4:case 5:cameraGL_.position(0,0,200);break;
    case 6:cameraGL_.position(80,40,80);break;
    case 7:cameraGL_.position(350,60,350);break;
    default:cameraGL_.position(0,2,100);break;

    }

    cameraGL_.lookAt(Vector3(0,0,0));

    loadScene(sceneNumber_);
    break;
  }

}

void GLApplication::updateCamera() {
  if (mouseLeft()) {
    Vector3 center=cameraGL_.pointTo(Coordinate_Local,Vector3(0,0,0));
    Vector3 vertical=Vector3(0,1,0);
    cameraGL_.translate(center,Coordinate_Local);
    cameraGL_.rotate(-deltaMouseX()/2.0,vertical,Coordinate_World);
    cameraGL_.rotate(deltaMouseY()/2.0,Vector3(1,0,0),Coordinate_Local);
    cameraGL_.translate(-center,Coordinate_Local);
  }
  if (left()) cameraGL_.left(1);
  if (right()) cameraGL_.right(1);
  if (forward()) cameraGL_.forward(1);
  if (backward()) cameraGL_.backward(1);
  if (accelerateWheel()) {
    frustum_*=1.05;
    cameraGL_.frustum(-frustum_,frustum_,-frustum_,frustum_,0.3,1000);
  }
  if (decelerateWheel()) {
    frustum_/=1.05;
    cameraGL_.frustum(-frustum_,frustum_,-frustum_,frustum_,0.3,1000);
  }
}




void GLApplication::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  p3d::apply(cameraGL_);

  scene_.camera(cameraGL_);
  scene_.drawGL();
  p3d::ambientColor=Vector4(1,0,0,1);
  p3d::draw(scene_.name(),Vector2(0,0));

  p3d::apply(cameraRaytrace_);
  raytraceTexture_.set(raytrace_.image()->mirrored());


  raytraceTexture_.bind(0);


  p3d::ambientColor=Vector4(1,1,1,1);
  p3d::shaderTextureAmbient();
  p3d::drawSquare();

  glUseProgram(0);

  snapshot();

}

/** ********************************************************************************
 *
 *
**/

void GLApplication::loadScene(unsigned int number) {
    string name="scene";
    ostringstream nb;
    nb << number;
    name+=nb.str();

    name+=".csg";
    cout << "Loading csg : " << "\"" << name << "\"" << endl;
    scene_.read(name);
    scene_.prepareCsg();
    scene_.camera(cameraGL_);
    raytrace_.scene(&scene_);
    //raytrace_.start();
}

void GLApplication::restartRaytrace() {
  raytrace_.close();
  scene_.camera(cameraGL_);
  raytrace_.start();

}

