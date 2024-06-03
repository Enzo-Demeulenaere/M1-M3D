#version 330

const int nbBlobMax = 30; // number of max blob (the C++ application should be coherent : no more Blob than nbBlobMax)

uniform int nbBlob;             // number of blobs
uniform vec2 center[nbBlobMax]; // center of each blob
uniform float radius[nbBlobMax]; // radius of each blob

out vec4 color; // resulting color of the pixel

// compute the value of the i-th blob at the point (x,y) of this pixel
// note : you can use distance(vec2,vec2), pow(float,float) in glsl
float computeBlobValue(int i) {
  float res=0.0;

  // TODO

  return res;
}

// compute the total value of the implicit function at this pixel
float computeImplicit() {
  float res=0.0;

  // TODO

  return res;
}

// compute color according to the height (for example : interpolate blue/red between height 0 and height 2)
vec4 computeColor(float height) {
  vec4 color=vec4(1,1,1,1);

  // TODO

  return color;
}

void main() {
  float height = computeImplicit();
  color = computeColor(height);
}
