/**

  @author F. Aubert
  **/


#include "GLApplication.h"
#include "GLTool.h"
#include <iostream>
#include <sstream>
#include "p3helpers.h"


using namespace std;
using namespace p3d;


/// ctor
GLApplication::GLApplication() {
  leftPanelMenu_ << "View Sign" << "View Points" << "View Edges" << "View Grid" << "Shader View";

  camera3D_.frustum(-frustum_,frustum_,-frustum_,frustum_,0.3,1000);
  camera3D_.position(-5,-5,5);
  camera3D_.lookAt(Vector3(0,0,0),Vector3(0,0,1));

  float space=2.05;

  camera2D_.position(0,0,1);
  camera2D_.ortho(-space,space,-space,space,-100,100);

  grid_.setSize(20,20);

  implicit_.addBlob(Vector2(0.3,0.3),1);
  implicit_.addBlob(Vector2(-0.3,-0.3),1);





}

/// dtor
///
GLApplication::~GLApplication() {

}

/**
 * @brief initialize the square vao and the implicit shader
 */
void GLApplication::initSquare() {
  vector<float> v{-2.0,-2.0,-2.0,2.0,2.0,-2.0,2.0,2.0};
 // vector<float> v{-1.0,-1.0,-1.0,1.0,1.0,-1.0,1.0,1.0};
  GLuint vb;

  /** init the VAO of the square **/
  glGenBuffers(1,&vb);
  glBindBuffer(GL_ARRAY_BUFFER,vb);
  glBufferData(GL_ARRAY_BUFFER,8*sizeof(GLfloat),v.data(),GL_STATIC_DRAW);

  glGenVertexArrays(1,&squareVao_);
  glBindVertexArray(squareVao_);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);

  glBindVertexArray(0);
  /** ** **/


  implicitShader_.read("shader/implicit");


}

/**
 * @brief GL initializations
 */
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
  glPointSize(6.0);

  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
  glPixelStorei(GL_PACK_ALIGNMENT,1);

  grid_.initVAO();
  initSquare();

}



/** ***************************************************************************
resize
  **/
void GLApplication::resize(int width,int height) {
  // appelée à chaque dimensionnement du widget OpenGL
  // (inclus l'ouverture de la fenêtre)
  // => réglages liés à la taille de la fenêtre
  camera2D_.viewport(0,0,width/2,height);
  camera3D_.viewport(width/2,0,width/2,height);
}




/**
 * @brief update data at each frame (before draw)
 */
void GLApplication::update() {
  updateCamera();


  if (keyPressed(Qt::Key_Space)) {
  }

  //grid_.evalTest();
  grid_.evalAll(implicit_);
}

void GLApplication::leftPanel(int i,const std::string &s) {
  cout << "GLApplication : button clicked " << i << " " << s << endl;
  switch(i) {
  case 0:
    viewSign_=!viewSign_;
    break;
  case 1:
    viewPoint_=!viewPoint_;
    break;
  case 2:
    viewEdge_=!viewEdge_;
    break;
  case 3:
    viewGrid_=!viewGrid_;
    break;
  case 4:
    viewShader_=!viewShader_;
    break;
  }

}

void GLApplication::updateCamera() {
  if (mouseLeft()) {
    Vector3 center=camera3D_.pointTo(Coordinate_Local,Vector3(0,0,0));
    Vector3 vertical=Vector3(0,0,1);
    camera3D_.translate(center,Coordinate_Local);
    camera3D_.rotate(-deltaMouseX()/2.0,vertical,Coordinate_World);
    camera3D_.rotate(deltaMouseY()/2.0,Vector3(1,0,0),Coordinate_Local);
    camera3D_.translate(-center,Coordinate_Local);
  }
  if (left()) camera3D_.left(1);
  if (right()) camera3D_.right(1);
  if (forward()) camera3D_.forward(1);
  if (backward()) camera3D_.backward(1);
  if (accelerateWheel()) {
    frustum_*=1.05;
    camera3D_.frustum(-frustum_,frustum_,-frustum_,frustum_,0.3,1000);
  }
  if (decelerateWheel()) {
    frustum_/=1.05;
    camera3D_.frustum(-frustum_,frustum_,-frustum_,frustum_,0.3,1000);
  }
}


/**
 * @brief draw the grid, sign of vertices, points for height = 0 and implicit curve
 */
void GLApplication::drawScene() {
  if (viewGrid_) {
    grid_.drawGrid();
  }
  if (viewSign_) {
    grid_.drawSign();
  }
  if (viewPoint_) {
    grid_.drawIntersect();
  }
  if (viewEdge_) {
    grid_.drawEdge();
  }

}

/**
 * @brief visualisation of the height function with shader
 */
void GLApplication::drawShader() {
  implicitShader_.use();
  implicitShader_.uniform("modelview",p3d::modelviewMatrix);
  implicitShader_.uniform("projection",p3d::projectionMatrix);

  implicitShader_.uniform("nbBlob",implicit_.nbBlob());
  implicitShader_.uniform("center",implicit_.allCenter());  // should be more efficient : array copy at each draw
  implicitShader_.uniform("radius",implicit_.allRadius());  // same


  glBindVertexArray(squareVao_);
  glDrawArrays(GL_TRIANGLE_STRIP,0,4);
}


/**
 * @brief draw the frame
 */
void GLApplication::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  p3d::apply(camera2D_);
  drawScene();

  if (viewShader_) {
    drawShader();
  }

  p3d::apply(camera3D_);

  drawScene();



  snapshot();

}


