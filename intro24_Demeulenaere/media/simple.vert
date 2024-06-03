#version 330
        
layout(location=0) in vec3 position;
layout(location=1) in vec4 color;
out vec4 fColor;
uniform float coeff;
layout(location=2) in vec2 texCoord;
out vec2 fTexCoord;

void main() {
    fColor = color;
    vec3 newPosition=position;
    gl_Position=vec4(newPosition,1.0);
    fTexCoord = texCoord;
}
