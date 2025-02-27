#include "Revolution.h"

#include "glsupport.h"


#include "GLDraw.h"
#include "GLRender.h"

#include "Vector3.h"


using namespace std;
using namespace p3d;

Revolution::Revolution() {
  // default profile
  profile_.push_back(Vector3(-0.8,0.6,0.0));
  profile_.push_back(Vector3(-0.5,0.0,0.0));
  profile_.push_back(Vector3(-0.9,-0.8,0.0));
}


void Revolution::initTetrahedron() {
  vector<float> p;
  vector<int> index;

  p={-1,0,-1,1,0,-1,0,1,1,-1,0,-1,0,1,1,0,-1,1,-1,0,-1,1,0,-1,0,-1,1,1,0,-1,0,1,1,0,-1,1};
// Original
//  index={0,1,2,3,4,5,6,7,8,9,10,11};
// Question 2
  index={0,2,1,3,5,4,6,7,8,9,10,11};

  initVAO(index,p,{},{}); // normal and texture coordinates not required here

}

void Revolution::initSphere() {
  vector<float> p;
  vector<float> n;
  vector<float> t;

  vector<int> index;

  int nbSlice=20; // include last slice that closes sphere
  int nbStack=20;

  float theta = 0;
  float phi = 0;

  float theta_offset = 2.*M_PI/(nbSlice-1);
  float phi_offset = M_PI/(nbStack-1);

  // positions, normals and textures
  for (int i=0; i<nbStack; i++){
      for (int j=0;j<nbSlice; j++){
           p.push_back(cos(theta)*sin(phi));
           p.push_back(cos(phi));
           p.push_back(sin(theta)*sin(phi));

           n.push_back(cos(theta)*sin(phi));
           n.push_back(cos(phi));
           n.push_back(sin(theta)*sin(phi));

           t.push_back(1-(theta/(2.*M_PI)));
           t.push_back(1-(phi/M_PI));

           theta = theta + theta_offset;

      }
      theta = 0;
      phi = phi + phi_offset;
  }
  // indexes
  for (int i=0; i<nbStack-1; i++){
      for (int j=0;j<nbSlice-1; j++){
          // first triangle
           index.push_back(i*nbSlice+j);     // 0
           index.push_back(i*nbSlice+j+1);   // 1
           index.push_back((i+1)*nbSlice+j); // 6
          // second triangle
           index.push_back(i*nbSlice+j+1);     // 1
           index.push_back((i+1)*nbSlice+j+1); // 7
           index.push_back((i+1)*nbSlice+j);   // 6
      }
  }


  initVAO(index,p,n,t);

  currentTexture_=&earth_;

}



void Revolution::initRevolution() {
  if (profile_.size()<2) return; // more than 2 vertices in the profile

  vector<float> p;
  vector<float> n;
  vector<float> t;

  vector<int> index;

  size_t nbSlice=40;
  size_t nbStack=profile_.size();

  std::vector<Vector3> normalProfile; // to set normal profile

  float theta = 0;
  float phi = 0;

  float theta_offset = 2.*M_PI/(nbSlice-1);
  float phi_offset = M_PI/(nbStack-1);

  // positions normals and texture
  for (int i=0; i<(int)nbStack; i++){
      for (int j=0;j<(int)nbSlice; j++){
           // positions
           p.push_back(profile_[i].rotationY(theta).x());
           p.push_back(profile_[i].rotationY(theta).y());
           p.push_back(profile_[i].rotationY(theta).z());

           // normals
           Vector3 orthoPrev;
           Vector3 orthoNext;
           if (i>0){
               Vector3 dirPrev = profile_[i]-profile_[i-1];
               orthoPrev = Vector3(-dirPrev.y(),dirPrev.x(),0);
           }
           if (i<(int)nbStack-1){
               Vector3 dirNext = profile_[i+1]-profile_[i];
               orthoNext = Vector3(-dirNext.y(),dirNext.x(),0);
           }
           if (i == 0){
               n.push_back(orthoNext.rotationY(theta).x());
               n.push_back(orthoNext.rotationY(theta).y());
               n.push_back(orthoNext.rotationY(theta).z());
           }
           else{
               if (i == (int)nbStack-1){
                   n.push_back(orthoPrev.rotationY(theta).x());
                   n.push_back(orthoPrev.rotationY(theta).y());
                   n.push_back(orthoPrev.rotationY(theta).z());
               }
               else{
                   n.push_back(((orthoPrev+orthoNext).rotationY(theta).x())/2);
                   n.push_back(((orthoPrev+orthoNext).rotationY(theta).y())/2);
                   n.push_back(((orthoPrev+orthoNext).rotationY(theta).z())/2);
               }
           }

           // textures
           t.push_back((theta/(2.*M_PI)));
           t.push_back(1-(phi/M_PI));

           theta = theta + theta_offset;

      }
      theta = 0;
      phi = phi + phi_offset;
  }
  // indexes
  for (int i=0; i<(int)nbStack-1; i++){
      for (int j=0;j<(int)nbSlice-1; j++){
          // first triangle
           index.push_back(i*nbSlice+j);     // 0
           index.push_back(i*nbSlice+j+1);   // 1
           index.push_back((i+1)*nbSlice+j); // 6
          // second triangle
           index.push_back(i*nbSlice+j+1);     // 1
           index.push_back((i+1)*nbSlice+j+1); // 7
           index.push_back((i+1)*nbSlice+j);   // 6
      }
  }




  // *******
  //  TODO




  // *******





  initVAO(index,p,n,t);
  currentTexture_=&logoUniv_;
}


void Revolution::init() {
  glGenBuffers(1,&vbo_);
  glGenBuffers(1,&elementBuffer_);
  glGenVertexArrays(1,&vao_);

  earth_.readMipmap("EarthDay512.jpg");
  logoUniv_.readMipmap("univ22.png");

}


void Revolution::draw(EShader mode,bool isFill) {
//  return;
  glBindVertexArray(vao_);


  p3d::material(Vector4(0,0,0,1),Vector3(0,0.8,0.2),Vector3(0,0.2,0.8),100);
  p3d::diffuseBackColor=Vector3(0.8,0,0);
  if (isFill) {
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  }
  else {
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  }
  if (mode==Shader_Ambient) {
    p3d::ambientColor=Vector4(0.0,0.8,0,1);
    p3d::ambientBackColor=Vector4(0.8,0.0,0,1);
  }

  if (currentTexture_) {
    currentTexture_->bind(0);
  }

  switch(mode) {
  case Shader_Ambient:p3d::shaderVertexAmbient();break;
  case Shader_Light:p3d::shaderLightPhong();break;
  case Shader_Texture:p3d::shaderTextureLight();break;
  }


  glDrawElements(GL_TRIANGLES,nbElement_,GL_UNSIGNED_INT,nullptr);



  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBindVertexArray(0);

}


// Caution : position should be a (x,y,z) array, normal (x,y,z) array, texCoord (x,y) array. Array sizes should be coherent for all vertex shader attributes
void Revolution::initVAO(const std::vector<int> &index,const std::vector<float> &position,const std::vector<float> &normal,const std::vector<float> &texCoord) {

  vector<float> attrib;
  attrib.resize(position.size()/3*8);
  for(unsigned int i=0;i<position.size()/3;++i) {
    attrib[i*8+0]=position[i*3+0];
    attrib[i*8+1]=position[i*3+1];
    attrib[i*8+2]=position[i*3+2];

    if (!normal.empty()) {
      attrib[i*8+3]=normal[i*3+0];
      attrib[i*8+4]=normal[i*3+1];
      attrib[i*8+5]=normal[i*3+2];
    }

    if (!texCoord.empty()) {
      attrib[i*8+6]=texCoord[i*2+0];
      attrib[i*8+7]=texCoord[i*2+1];
    }
  }



  glBindBuffer(GL_ARRAY_BUFFER,vbo_);
  glBufferData(GL_ARRAY_BUFFER,int(attrib.size()*sizeof(float)),attrib.data(),GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBuffer_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,int(index.size()*sizeof(int)),index.data(),GL_STATIC_DRAW);
  nbElement_=int(index.size());

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER,vbo_);

  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),nullptr);                                   // position
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),reinterpret_cast<const GLvoid *>(3*sizeof(float)));   // normal
  glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),reinterpret_cast<const GLvoid *>(6*sizeof(float)));   // texCoord

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBuffer_);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);


  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBuffer_);

  glBindVertexArray(0);

}

void Revolution::drawProfile() {
  p3d::ambientColor=Vector4(1,0,0,1);
  p3d::shaderVertexAmbient();

  p3d::drawLineStrip(profile_);
  p3d::drawPoints(profile_);

  vector<Vector3> mirror;
  for(auto &p:profile_) {
    mirror.push_back(Vector3(-p.x(),p.y(),p.z()));
  }

  p3d::uniformAmbient(Vector4(0,1,0,1));
  p3d::drawLineStrip(mirror);
  p3d::drawPoints(mirror);

  p3d::uniformAmbient(Vector4(0,1,0,1));
  p3d::drawLines({Vector3(0,-1,0),Vector3(0,1,0)});



}


void Revolution::interactInsert(size_t i,const p3d::Vector3 &p) {
  if (profile_.size()>0) {
    Vector3 pred=*(profile_.begin()+int(i-1));
    if ((pred-p).length()<=0.1) return;
  }
  profile_.insert(profile_.begin()+int(i),p);
}

