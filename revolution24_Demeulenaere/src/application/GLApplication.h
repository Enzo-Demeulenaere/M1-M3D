#pragma once

#include "GLWidget.h"

#include "Texture.h"
#include "Camera.h"

#include "Revolution.h"


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

  typedef enum {Init_Tetrahedron,Init_Sphere,Init_Revolution,Init_None} ERequestInit;
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
  void apply(const p3d::Camera &c);

  // ****************************
  // TP :



  void drawScene();

private:
  void drawGroundTool(const p3d::Texture &texture);



private:
  p3d::Camera camera_,cameraInput_;

  Revolution revolution_;

  ERequestInit requestInit_=Init_Tetrahedron; // Init_Sphere or Init_Revolution or Init_Tetrahedron
  Revolution::EShader shaderMode_=Revolution::Shader_Ambient; // Shader_Ambient or Shader_Light or Shader_Texture
  EView viewMode_=View_3D; // View_2D for profile input
  bool isFill_=false; // triangles are rendered with GL_LINE or GL_FILL

};


