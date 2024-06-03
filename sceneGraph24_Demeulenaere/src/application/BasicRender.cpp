#include "BasicRender.h"

#include "Matrix4.h"
#include "Matrix3.h"
#include "Vector3.h"

namespace tp {
p3d::Matrix4 projection;
p3d::Matrix4 modelview;

p3d::Vector3 lightPosition;

p3d::Vector3 materialDiffuseFront;
p3d::Vector3 materialDiffuseBack;

p3d::Shader shader;


BasicVAO cube;
BasicVAO cylinder;
BasicVAO torus;

void setupShader() {
  glUseProgram(shader.id()); // all glDraw... instructions will draw with this shader

  // configure uniforms :

  shader.uniform("materialDiffuseFront",tp::materialDiffuseFront);
  shader.uniform("materialDiffuseBack",tp::materialDiffuseBack);
  shader.uniform("lightPosition",tp::lightPosition);
  shader.uniform("modelview",tp::modelview);
  shader.uniform("mvp",tp::projection*tp::modelview);
  shader.uniform("normalMatrix",tp::modelview.normalMatrix());
}

void initPrimitive() {
  cube.initCube();
  torus.initTorus(10,20,1,0.2);
  cylinder.initCylinder(20);
}

void release() {
  shader.release();
  cube.release();
}

} // close namespace p3d



