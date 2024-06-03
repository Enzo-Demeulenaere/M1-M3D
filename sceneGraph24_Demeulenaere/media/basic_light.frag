#version 330

in vec3 colorFront;
in vec3 colorBack;

out vec4 fragColor;

void main(void)
{
  if (gl_FrontFacing) {
    fragColor = vec4(colorFront,1);
  }
  else {
    fragColor = vec4(colorBack,1);
  }
}
