#version 330

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;

uniform mat4 modelviewMatrix;
uniform mat3 normalMatrix;
uniform mat4 mvp;

uniform vec3 lightPosition;

out vec3 fN,fL,fV;


void main() {
  vec4 positionEye;

  positionEye=modelviewMatrix*vec4(position,1);

  fV=-positionEye.xyz/positionEye.w;
  fL=lightPosition+fV;
  fN=normalMatrix*normal;

  gl_Position=mvp*vec4(position,1);
}
