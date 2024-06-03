#version 330

layout(location=0) in vec3 position; //Eye coordinates
layout(location=1) in vec3 normal;

out vec3 fColor;

uniform mat4 projection;
uniform mat4 transform;
uniform vec3 lightPosition;
uniform vec3 diffuseColor;

void main() {

  vec3 N = normalize((transform*vec4(normal,0.0)).xyz);
  vec3 L = normalize(lightPosition-position);
  float intensity = max(dot(N,L),0.0);
  vec4 eyePosition=vec4(position,1);
  eyePosition=transform*eyePosition;
  vec4 clipPosition=projection*eyePosition;

  fColor=intensity*diffuseColor;

  gl_Position=clipPosition;
}
