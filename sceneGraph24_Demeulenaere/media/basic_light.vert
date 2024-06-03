#version 330

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;

out vec3 colorFront;
out vec3 colorBack;

uniform mat4 mvp;
uniform mat4 modelview;
uniform mat3 normalMatrix;

uniform vec3 lightPosition;
uniform vec3 materialDiffuseFront;
uniform vec3 materialDiffuseBack;

void main(void)
{
  vec4 positionEye=modelview*vec4(position,1);

  vec3 N=normalize(normalMatrix*normal);
  vec3 L=normalize(lightPosition-positionEye.xyz/positionEye.w);

  float intensity = dot(L,N);

  colorFront=materialDiffuseFront*max(0,intensity);
  colorBack=materialDiffuseBack*max(0,-intensity);


  gl_Position = mvp*vec4(position,1);
}
