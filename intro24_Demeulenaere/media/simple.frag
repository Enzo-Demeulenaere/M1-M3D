#version 330

out vec4 fragColor;
in vec4 fColor;
in vec2 fTexCoord;
uniform sampler2D textureUnit;
uniform float coeff;

void main() {
    fragColor=texture(textureUnit,fTexCoord)*fColor.b;
    fragColor.g*=(1.0-coeff);
}
