#version 330

in vec3 fN,fL,fV;

out vec4 fragColor;

uniform vec3 materialDiffuse,materialSpecular;
uniform vec4 materialAmbient;

void main() {

  vec3 N,L,V;
  vec4 color=vec4(0,0,0,0);

  N = normalize(fN);
  L = normalize(fL);
  V = normalize(fV);

  float diffuseIntensity=max(dot(N,L),0.0);

  vec3 R = reflect(-L, N);
  float specularIntensity = pow(max(dot(R,V),0.0),50);

  // rouge, vert,bleu de l'éclairement :
  color.rgb=diffuseIntensity*materialDiffuse+(specularIntensity*materialSpecular)+materialAmbient.xyz;
  // le alpha est porté uniquement par materialAmbient.a.:
  color.a=materialAmbient.a;


  fragColor=color;
}
