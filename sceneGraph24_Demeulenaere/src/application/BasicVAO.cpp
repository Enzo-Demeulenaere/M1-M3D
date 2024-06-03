#include <cmath>

#include "BasicVAO.h"

using namespace std;

BasicVAO::BasicVAO() {
}

BasicVAO::~BasicVAO() {
  release();
}

void BasicVAO::release() {
  if (vao_) {
    glDeleteBuffers(1,&vbo_);
    glDeleteBuffers(1,&ibo_);
    glDeleteBuffers(1,&vao_);
    vao_=0;
  }
}

void BasicVAO::draw() {
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES,nbElement_,GL_UNSIGNED_INT,nullptr);
  glBindVertexArray(0);
}

void BasicVAO::init(const std::vector<int> &index,const std::vector<float> &position,const std::vector<float> &normal) {
  glGenBuffers(1,&vbo_);
  glGenBuffers(1,&ibo_);
  glGenVertexArrays(1,&vao_);

  int vertexSize = 6; // 3 floats for position + 3 floats for normal

  vector<float> attrib;
  attrib.resize(position.size()/3*vertexSize);
  for(unsigned int i=0;i<position.size()/3;++i) {
    attrib[i*vertexSize+0]=position[i*3+0];
    attrib[i*vertexSize+1]=position[i*3+1];
    attrib[i*vertexSize+2]=position[i*3+2];

    attrib[i*vertexSize+3]=normal[i*3+0];
    attrib[i*vertexSize+4]=normal[i*3+1];
    attrib[i*vertexSize+5]=normal[i*3+2];
  }

  nbElement_=int(index.size()); // nb vertices in vao (i.e. nb vertices to draw)


  // copy data in vbo and ido
  glBindBuffer(GL_ARRAY_BUFFER,vbo_);
  glBufferData(GL_ARRAY_BUFFER,int(attrib.size()*sizeof(float)),attrib.data(),GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,int(index.size()*sizeof(int)),index.data(),GL_STATIC_DRAW);

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER,vbo_);

  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,vertexSize*sizeof(float),nullptr);                                   // position
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,vertexSize*sizeof(float),reinterpret_cast<const GLvoid *>(3*sizeof(float)));   // normal

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);


  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo_);

  glBindVertexArray(0);

}


void BasicVAO::initCube() {
  vector<float> position{
    -1.0,1.0,1.0, // face z=1
    -1.0,-1.0,1.0,
    1.0,1.0,1.0,
    1.0,-1.0,1.0,

    -1.0,-1.0,-1.0, // face z=-1
    -1.0,1.0,-1.0,
    1.0,-1.0,-1.0,
    1.0,1.0,-1.0,

    1.0,-1.0,1.0, // face x=1
    1.0,-1.0,-1.0,
    1.0,1.0,1.0,
    1.0,1.0,-1.0,

    -1.0,-1.0,-1.0, // face x=-1
    -1.0,-1.0,1.0,
    -1.0,1.0,-1.0,
    -1.0,1.0,1.0,

    1.0,1.0,1.0, // face y=1
    1.0,1.0,-1.0,
    -1.0,1.0,1.0,
    -1.0,1.0,-1.0,

    -1.0,-1.0,1.0,  // face y=-1
    -1.0,-1.0,-1.0,
    1.0,-1.0,1.0,
    1.0,-1.0,-1.0
  };
  vector<float> normal {
    0,0,1, // face z=1
    0,0,1,
    0,0,1,
    0,0,1,
    0,0,-1, // face z=-1
    0,0,-1,
    0,0,-1,
    0,0,-1,
    1,0,0, // face x=1
    1,0,0,
    1,0,0,
    1,0,0,
    -1,0,0, // face x=-1
    -1,0,0,
    -1,0,0,
    -1,0,0,
    0,1,0, // face y=1
    0,1,0,
    0,1,0,
    0,1,0,
    0,-1,0, // face y=-1
    0,-1,0,
    0,-1,0,
    0,-1,0
  };

  vector<int> index{
    0,1,2,2,1,3, // face z=1
    4,5,6,6,5,7, // face z=-1
    8,9,10,10,9,11, // face x=1
    12,13,14,14,13,15, // face x=-1
    16,17,18,18,17,19, // face y=1
    20,21,22,22,21,23 // face y=-1
  };
  init(index,position,normal);
}


void BasicVAO::initTorus(size_t nbSlice,size_t nbStack,double rg,double rp) {

  vector<float> position;
  vector<float> normal;
  vector<int> index;

  double theta=0.0; // angle for external radius
  double phi=0.0;   // angle for internal radius
  double stepTheta=2.0*M_PI/nbStack;
  double stepPhi=2.0*M_PI/nbSlice;

  double nx0,ny0,nz0,nx1,ny1,nz1;
  double x0,y0,z0;
  double x1,y1,z1;
  for(size_t i=0;i<nbStack+1;++i) {
    phi=0.0;
    for(size_t j=0;j<nbSlice+1;++j) {
      if (j==nbSlice) phi=0.0; // last slice on first slice
      nx1=cos(phi);
      ny1=0.0;
      nz1=sin(phi);
      x1=rp*nx1+(rg-rp);
      y1=0.0;
      z1=rp*nz1;

      nx0=(nx1*cos(theta)-ny1*sin(theta));
      ny0=(nx1*sin(theta)+ny1*cos(theta));
      nz0=nz1;

      x0=(x1*cos(theta)-y1*sin(theta));
      y0=(x1*sin(theta)+y1*cos(theta));
      z0=z1;
      normal.push_back(nx0);
      normal.push_back(ny0);
      normal.push_back(nz0);
      position.push_back(x0);
      position.push_back(y0);
      position.push_back(z0);
      phi+=stepPhi;
    }
    theta+=stepTheta;
  }
  for(size_t i=0;i<nbStack;++i) {
    for(size_t j=0;j<nbSlice;++j) {
      index.push_back(GLuint(i*(nbSlice+1)+j));
      index.push_back(GLuint(i*(nbSlice+1)+nbSlice+1+j));
      index.push_back(GLuint(i*(nbSlice+1)+j+1));

      index.push_back(GLuint(i*(nbSlice+1)+j+1));
      index.push_back(GLuint(i*(nbSlice+1)+nbSlice+1+j));
      index.push_back(GLuint(i*(nbSlice+1)+nbSlice+1+j+1));

    }
  }
  init(index,position,normal);
}

void BasicVAO::initCylinder(size_t nbSlice) {
  vector<float> position;
  vector<float> normal;
  vector<int> index;

  double step=2.0*M_PI/nbSlice;
  double theta=0.0;
  double x,y;
  for(size_t i=0;i<nbSlice+1;i++) {
    if (i==nbSlice) theta=0;
    x=cos(theta);
    y=sin(theta);
    normal.push_back(x);
    normal.push_back(y);
    normal.push_back(0);

    normal.push_back(x);
    normal.push_back(y);
    normal.push_back(0);

    position.push_back(x); // top
    position.push_back(y);
    position.push_back(1.0);
    position.push_back(x); // bottom
    position.push_back(y);
    position.push_back(0.0);
    theta+=step;
  }

  for(size_t i=0;i<nbSlice;++i) {
    index.push_back(GLuint(i*2));
    index.push_back(GLuint(i*2+1));
    index.push_back(GLuint(i*2+2));

    index.push_back(GLuint(i*2+2));
    index.push_back(GLuint(i*2+1));
    index.push_back(GLuint(i*2+3));
  }
  init(index,position,normal);

}




