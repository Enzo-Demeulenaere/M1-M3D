#pragma once

#include "GLWidget.h"

#include "BasicCamera.h"

#include "SceneGraph.h"
#include "Shader.h"


/*!
*
* @file
*
* @brief
* @author F. Aubert - Univ Lille 1
*
*/

class GLApplication : public GLWidget
{

  typedef enum {View_Input,View_3D} EView;

public:
  ~GLApplication() override;
  GLApplication();

  /** ***** **/
  /** GLApplication must implement these methods : */
  void initialize() override;
  void update() override;
  void draw() override;
  void resize(int width,int height) override;
  /** ***** **/
  void leftPanel(size_t i, const std::string &s) override;
  QSize sizeHint() const override {return QSize(500,500);}

  void updateCamera();

  // ****************************
  // TP :



  void drawScene();
  void draw0();
  void draw1();
  void draw2();
  void draw3();
  void draw4();


private:
  BasicCamera camera_;
  SceneGraph scene_;

  int viewStep_{3};

  float angle_{0};
  float angleCam_{0};
  float stepAngleCam_{2};
  //EView viewMode_=View_3D;

};


