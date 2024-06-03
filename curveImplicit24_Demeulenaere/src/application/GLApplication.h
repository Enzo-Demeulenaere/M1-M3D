#pragma once


/**
*
*  @author F. Aubert
*  @file GLView
*
*/




#include "GLWidget.h"
#include "Camera.h"
#include "Grid.h"
#include "Shader.h"



/**
  @class GLView

  The widget to draw with OpenGL.
  */
class GLApplication : public GLWidget {
  typedef enum {Draw_Bezier,Draw_Catmull,Draw_Hermite,Draw_Roller} EDraw;
  typedef enum {Control_Bezier,Control_Catmull,Control_Hermite} EControl;
  typedef enum {Mouse_Camera,Mouse_Interact,Mouse_Nothing} EMouse;

  Q_OBJECT
public:
  //! Ctor
  GLApplication();
  //! Dtor
  virtual ~GLApplication();

  /** ***** **/
  /** GLApplication must implement these methods : */
  virtual void initialize();
  virtual void update();
  virtual void draw();
  virtual void resize(int width,int height);
  /** ***** **/

  virtual void leftPanel(int i, const std::string &s);
  QSize sizeHint() const {return QSize(1000,500);}


  void updateCamera();

  // ****************************
  // TP :

  void initSquare();
  void drawScene();
  void drawShader();


private:


  // ****************************
  // TP :
  p3d::Camera camera2D_;
  p3d::Camera camera3D_;
  float frustum_=0.1;

  Grid grid_;
  Implicit implicit_;

  GLuint squareVao_;
  p3d::Shader implicitShader_;

  bool viewSign_{false},viewPoint_{false},viewEdge_{false},viewGrid_{true},viewShader_{false};

signals:

public slots:

};




