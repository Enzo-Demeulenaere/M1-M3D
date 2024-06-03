#pragma once

#include "Matrix4.h"
#include "Shader.h"
#include "BasicVAO.h"

namespace tp {
extern p3d::Matrix4 modelview;
extern p3d::Matrix4 projection;
extern p3d::Vector3 lightPosition;

extern p3d::Vector3 materialDiffuseFront;
extern p3d::Vector3 materialDiffuseBack;

extern p3d::Shader shader;

extern BasicVAO cube,torus,cylinder;

void drawVAO(const BasicVAO &vao);

void setupShader();
void initPrimitive();
void release();
}




