#pragma once


/**
*
*  @author F. Aubert
*  @file GLView
*
*/




#include "GLWidget.h"

#include "Camera.h"
#include "Texture.h"
#include "Scene.h"
#include "Raytrace.h"



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

  void loadScene(unsigned int n);

  void restartRaytrace();
private:

  p3d::Camera cameraGL_;
  p3d::Camera cameraRaytrace_;
  double frustum_=0.01;


  // ****************************
  // TP :
  Scene scene_;
  int sceneNumber_;
  Raytrace raytrace_;
  p3d::Texture raytraceTexture_;



signals:

public slots:

};




