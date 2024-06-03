#version 330

layout(location=0) in vec2 position;

uniform mat4 projection;
uniform mat4 modelview;

void main() {

  vec4 pos = projection*modelview*vec4(position,-1,1); // z=-1 to draw the square behind the grid
  gl_Position=pos;
}
