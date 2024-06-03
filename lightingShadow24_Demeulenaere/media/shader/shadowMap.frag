#version 330

uniform sampler2D depthTexture;

in vec4 fTexCoord;

out vec4 fragColor;

void main() {
  vec4 texCoord=fTexCoord;

  texCoord.xyz/=texCoord.w; // clip coordinates => ndc

  texCoord.xyz = (texCoord.xyz + 1)/2;

  fragColor=texture(depthTexture,texCoord.xy);

  if (texCoord.z >= 0.0 && texCoord.z <= 1.0){
      if(texCoord.z-0.0001 > fragColor.r){
          fragColor = vec4(0,0,0,0.6);
      }
      else fragColor = vec4(0,0,0,0);
  }



}
