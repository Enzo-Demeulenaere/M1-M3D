#include "BasicMesh.h"
#include "Mesh.h"
#include "Vector3.h"
#include "Vector2.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace p3d;

BasicMesh::~BasicMesh() {
  //TODO : can be deleted only when GL context is active
  glDeleteBuffers(1,&attributeBuffer_);
  glDeleteBuffers(1,&elementBuffer_);
}


BasicMesh::BasicMesh() {
}







void BasicMesh::initTetrahedron() {
  std::vector<float> position={
// Original
//    -1,0,-1, // V0
//    1,0,-1,  // V1
//    0,1,1,  // V2
//    0,-1,1,  // V3
// Question 7
//    -1,0,-1, // V0
//    1,0,-1,  // V1
//    0,1,1,  // V2
//    -1,0,-1, // V3 (0)
//    0,1,1,  // V4  (2)
//    0,-1,1,  // V5 (3)
//    -1,0,-1, // V6 (0)
//    1,0,-1,  // V7 (1)
//    0,-1,1,  // V8 (3)
//    1,0,-1,  // V9 (1)
//    0,1,1,  // V10 (2)
//    0,-1,1,  // V12 (3)
// Question 11
      -20,0,-10, //V0
      10,0,-10,  //V1
      0,10,-30,  //V2
      0,-20,-30  //V3

  };

  std::vector<float> color={
// Original
//    1,0,0, // rouge
//    0,1,0, // vert
//    0,0,1, // bleu
//    0,1,1  // cyan
// Question 7
//      1,0,0, // rouge
//      1,0,0, // rouge
//      1,0,0, // rouge
//      0,1,0, // vert
//      0,1,0, // vert
//      0,1,0, // vert
//      0,0,1, // bleu
//      0,0,1, // bleu
//      0,0,1, // bleu
//      0,1,1,  // cyan
//      0,1,1,  // cyan
//      0,1,1  // cyan
// Question 11
      1,0,0, //rouge
      0,1,0, //vert
      0,0,1, //bleu
      0,1,1  //cyan
  };

  // index for 4 triangles
  element_={
// Original
//    0,1,2,0,2,3,0,1,3,1,2,3 // fonctionne aussi sans le premier triangle
// Question 7
//      0,1,2,3,4,5,6,7,8,9,10,11  // encore une fois le premier triangle est inutile car pas affiché
// Question 11
      0,1,2,0,2,3,0,1,3,1,2,3
  };








  attribute_.clear();
  for(size_t i=0;i<position.size()/3;++i) {

    attribute_.push_back(position[i*3+0]);
    attribute_.push_back(position[i*3+1]);
    attribute_.push_back(position[i*3+2]);



    attribute_.push_back(color[i*3+0]);
    attribute_.push_back(color[i*3+1]);
    attribute_.push_back(color[i*3+2]);

  }
}


void BasicMesh::initObj(const Mesh &obj) {
  attribute_.clear();
  element_.clear();

  // initialisation de attribute_ :
  // - obj.nbFace() = nombre de triangles
  // - obj.positionVertexFace(i,j) = position du j-ème sommet dans le i-ème triangle (de type Vector3)
  // - obj.normalVertexFace(i,j) = normale du j-ème sommet dans le i-ième triangle (de type Vector3)
  for(int i=0;i<obj.nbFace();++i) {
    for(int j=0;j<3;++j) {
        // positions
        attribute_.push_back(obj.positionVertexFace(i,j).x());
        attribute_.push_back(obj.positionVertexFace(i,j).y());
        attribute_.push_back(obj.positionVertexFace(i,j).z());
        // normals
        attribute_.push_back(obj.normalVertexFace(i,j).x());
        attribute_.push_back(obj.normalVertexFace(i,j).y());
        attribute_.push_back(obj.normalVertexFace(i,j).z());
    }
  }


}

void BasicMesh::initBuffer() {
  glGenBuffers(1,&attributeBuffer_);
  glBindBuffer(GL_ARRAY_BUFFER,attributeBuffer_);
  glBufferData(GL_ARRAY_BUFFER,long(attribute_.size()*sizeof(float)),attribute_.data(),GL_STATIC_DRAW);


  glGenBuffers(1,&elementBuffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBuffer_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,int(element_.size()*sizeof(unsigned int)),element_.data(),GL_STATIC_DRAW);

}

void BasicMesh::initVAO() {

  glGenVertexArrays(1,&vao_);
  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER,attributeBuffer_);

  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*6,0);
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(float)*6,(void*)(sizeof(float)*3));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBuffer_);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void BasicMesh::initDraw() {
  initBuffer();
  initVAO();
}


void BasicMesh::draw() {
  glBindVertexArray(vao_);

  //glDrawElements(GL_TRIANGLES,int(element_.size()),GL_UNSIGNED_INT,reinterpret_cast<const GLvoid *>(0*sizeof(GLuint)));
  glDrawArrays(GL_TRIANGLES,0,attribute_.size()/6);
  glBindVertexArray(0);
}


void BasicMesh::updateBuffer() {

}

