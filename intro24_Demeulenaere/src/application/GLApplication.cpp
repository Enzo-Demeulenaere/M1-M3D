#include "GLApplication.h"

#include <iostream>
#include <cmath>

using namespace std;

GLApplication::~GLApplication() {
  makeCurrent();
}

GLApplication::GLApplication() {

  trianglePosition_ = {
//    -0.8f,-0.5f,0.0f, // vertex 0
//    -0.2f,-0.5f,0.0f, // 1
//    -0.5f,0.5f,0.0f,  // 2

//     0.2f,0.5f,0.0f,  // 3
//     0.8f,0.5f,0.0f,  // 4
//     0.5f,-0.5f,0.0f  // 5

//     Question 12
//    -0.8f,-0.5f,0.0f, //0 (était 0)
//     0.8f, 0.5f,0.0f, //1 (était 4)
//    -0.5f, 0.5f,0.0f, //2 (était 2)
//    -0.2f,-0.5f,0.0f, //3 (était 1)
//     0.5f,-0.5f,0.0f, //4 (était 5)
//     0.2f, 0.5f,0.0f  //5 (était 3)


//      Question 16
//      -0.8f,-0.8f, 0.0f,
//       0.8f, 0.8f, 0.0f,
//       0.0f, 0.2f, 0.0f,
//      -0.8f, 0.8f, 0.0f,
//       0.8f,-0.8f, 0.0f

// Question 20
//      -0.8f,-0.8f, 0.0f,
//      -0.8f, 0.8f, 0.0f,
//      -0.4f,-0.8f, 0.0f,
//      -0.4f, 0.8f, 0.0f,
//       0.0f,-0.8f, 0.0f,
//       0.0f, 0.8f, 0.0f,
//       0.4f,-0.8f, 0.0f,
//       0.4f, 0.8f, 0.0f
      -0.6f,-0.8f,0.0f,
      -0.6f, 0.8f,0.0f,
       0.6f,-0.8f,0.0f,
       0.6f, 0.8f,0.0f


  };

  triangleColor_ = {
//    0.3f,0.0f,0.6f,1.0f,
//    0.3f,0.0f,0.6f,1.0f,
//    0.0f,0.9f,0.0f,1.0f,

//    0.0f,0.5f,0.6f,1.0f,
//    0.0f,0.5f,0.6f,1.0f,
//    0.9f,0.0f,0.0f,1.0f

      1,0,0,1,
      1,0,0,1,
      1,0,0,1,
      1,0,0,1,

  };

  triangleTexCoord_ = { // coordonnées de texture en chaque sommet
// Question 26
//      0,1,
//      0,0,
//      1,1,
//      1,0
// Question 27
//      0,0,
//      0,1,
//      1,0,
//      1,1
// Question 28
//      0.0f,0,
//      0.0f,1,
//      0.5f,0,
//      0.5f,1
  };

  elementData_ = {
      0,3,2,2,1,4
  };

  coeff_ = 1.0f;
  coeffOffset_ = 0.001f;

}




/** ********************************************************************** **/
void GLApplication::initialize() {
  // appelée 1 seule fois à l'initialisation du contexte
  // => initialisations OpenGL
  glClearColor(1,1,1,1);

  glLineWidth(2.0);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);


  shader0_=initProgram("simple");

  initRing(30,0.2f,0.6f);
  initTriangleBuffer();
  initTriangleVAO();
  initTexture();



}

void GLApplication::resize(int width,int height) {
  // appelée à chaque dimensionnement du widget OpenGL
  // (inclus l'ouverture de la fenêtre)
  // => réglages liés à la taille de la fenêtre
  glViewport(0,0,width,height);
  // ...
}

void GLApplication::update() {
  // appelée toutes les 20ms (60Hz)
  // => mettre à jour les données de l'application
  // avant l'affichage de la prochaine image (animation)
  // ...
    if (coeff_ < 0.0f){
       coeffOffset_ = 0.01f;
    }
    if (coeff_ > 1.0f ){
        coeffOffset_ = -0.01f;
    }
    coeff_ = coeff_ + coeffOffset_;

}

void GLApplication::draw() {
  // appelée après chaque update
  // => tracer toute l'image
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(shader0_);
  glUniform1f(glGetUniformLocation(shader0_,"coeff"),coeff_);
  glBindVertexArray(triangleVAO_);

  //glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

  glActiveTexture(GL_TEXTURE0); // on travaille avec l'unité de texture 0
    // dans l'instruction suivante, textureId_ correspond à l'image "lagoon.jpg"; cf GLApplication::initTexture pour l'initialisation de textureId_
    glBindTexture(GL_TEXTURE_2D,textureId_); // l'unité de texture 0 correspond à la texture _textureId
    // (le fragment shader manipule des unités de textures et non les identifiants de texture directement)
    glUniform1f(glGetUniformLocation(shader0_,"textureUnit"),0); // on affecte la valeur du sampler2D du fragment shader à l'unité de texture 0.

    glDrawArrays(GL_TRIANGLE_STRIP,0,trianglePosition_.size()/3);

  glBindVertexArray(0);
  glUseProgram(0);


  snapshot(); // capture opengl window if requested
}

/** ********************************************************************** **/
/** i = button number, s = button text
 */
void GLApplication::leftPanel(size_t i,const std::string &s) {
  cout << "GLApplication : button clicked " << i << " " << s << endl;
  /*
  switch (i) {
    case 0:menu0();break;
    case 1:menu1();break;
  }
  */
}




/** *********************************************************** **/
GLuint GLApplication::initProgram(const std::string &filename) {
  GLuint program;
  int infoLength; // for message error

  string vertexSource=p3d::readTextFile(filename+".vert");
  string fragmentSource=p3d::readTextFile(filename+".frag");

  program=glCreateProgram();
  GLuint vertexShader=glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
  glAttachShader(program,vertexShader);
  glAttachShader(program,fragmentShader);

  const char *source;
  source=vertexSource.c_str();
  glShaderSource(vertexShader,1,&source,nullptr);
  source=fragmentSource.c_str();
  glShaderSource(fragmentShader,1,&source,nullptr);

  GLint ok;
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&ok);
  if (!ok) {
    glGetShaderiv(vertexShader,GL_INFO_LOG_LENGTH,&infoLength);
    cout << "=============================" << endl;
    cout << "GLSL Error : " << endl;
    char *info=new char[size_t(infoLength)];
    glGetShaderInfoLog(vertexShader,infoLength,nullptr,info);
    cout << info;
    cout << endl;
    delete[] info;
    throw ErrorD("Vertex Shader compilation error in "+filename+".vert");
  }

  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&ok);
  if (!ok) {
    glGetShaderiv(fragmentShader,GL_INFO_LOG_LENGTH,&infoLength);
    cout << "=============================" << endl;
    cout << "GLSL Error : " << endl;
    char *info=new char[size_t(infoLength)];
    glGetShaderInfoLog(fragmentShader,infoLength,nullptr,info);
    cout << info;
    cout << endl;
    delete[] info;
    throw ErrorD("Fragment Shader compilation error in "+filename+".frag");
  }

  glLinkProgram(program);
  glGetProgramiv(program,GL_LINK_STATUS,&ok);
  if (!ok) {
    glGetProgramiv(program,GL_INFO_LOG_LENGTH,&infoLength);
    char *info=new char[size_t(infoLength)];
    glGetProgramInfoLog(program,infoLength,nullptr,info);
    cout << "Info Log :" << endl;
    cout << info;
    cout << endl;
    delete[] info;
    throw ErrorD("Program Shader : link error (varyings ok ?)");
  }

  return program;
}


void GLApplication::initTexture() {
    QImage img;
    img.load("../media/lagoon.jpg");
    img=img.convertToFormat(QImage::Format_ARGB32).mirrored();

    glGenTextures(1,&textureId_);
    glBindTexture(GL_TEXTURE_2D,textureId_);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img.width(),img.height(),0,GL_BGRA,GL_UNSIGNED_BYTE,img.bits());



    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
}


void GLApplication::initTriangleBuffer() {

  glGenBuffers(1,&trianglePositionBuffer_);
  glBindBuffer(GL_ARRAY_BUFFER,trianglePositionBuffer_);
  glBufferData(GL_ARRAY_BUFFER,trianglePosition_.size()*sizeof(float),trianglePosition_.data(),GL_STATIC_DRAW);

  glGenBuffers(1,&triangleColorBuffer_);
  glBindBuffer(GL_ARRAY_BUFFER,triangleColorBuffer_);
  glBufferData(GL_ARRAY_BUFFER,triangleColor_.size()*sizeof(float),triangleColor_.data(),GL_STATIC_DRAW);

  glGenBuffers(1,&triangleTexCoordBuffer_);
  glBindBuffer(GL_ARRAY_BUFFER,triangleTexCoordBuffer_);
  glBufferData(GL_ARRAY_BUFFER,triangleTexCoord_.size()*sizeof(float),triangleTexCoord_.data(),GL_STATIC_DRAW);


  glGenBuffers(1,&elementBuffer_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBuffer_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,elementData_.size()*sizeof(unsigned int),elementData_.data(),GL_STATIC_DRAW);
}


void GLApplication::initTriangleVAO() {
  glGenVertexArrays(1,&triangleVAO_);
  glBindVertexArray(triangleVAO_);

  glBindBuffer(GL_ARRAY_BUFFER,trianglePositionBuffer_);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,nullptr);
  glBindBuffer(GL_ARRAY_BUFFER,triangleColorBuffer_);
  glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,0,nullptr);
  glBindBuffer(GL_ARRAY_BUFFER,triangleTexCoordBuffer_);
  glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,nullptr);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,elementBuffer_);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);


  glBindVertexArray(0);
}

void GLApplication::initStrip(int nbSlice, float xmin, float xmax, float ymin, float ymax){
    trianglePosition_.clear();
    triangleColor_.clear();

    float offset = (xmax-xmin)/(nbSlice-1);
    float current_x = xmin;
    float current_blue = 1.0f;
    float current_green = 0.0f;
    float colorOffset = 1.0f/nbSlice;

    for (int i=0; i<nbSlice;i++){
        //  bottom vertice position
        trianglePosition_.push_back(current_x);
        trianglePosition_.push_back(ymin);
        trianglePosition_.push_back(0.0f);
        // bottom vertice color
        triangleColor_.push_back(0);
        triangleColor_.push_back(current_green);
        triangleColor_.push_back(0);
        triangleColor_.push_back(1);
        // top vertice position
        trianglePosition_.push_back(current_x);
        trianglePosition_.push_back(ymax);
        trianglePosition_.push_back(0.0f);
        // top vertice color
        triangleColor_.push_back(0);
        triangleColor_.push_back(0);
        triangleColor_.push_back(current_blue);
        triangleColor_.push_back(1);

        // offsets update
        current_x = current_x + offset;
        current_blue = current_blue - colorOffset;
        current_green = current_green + colorOffset;
    }

}

void GLApplication::initRing(int nbSlice, float r0, float r1){
    trianglePosition_.clear();
    triangleColor_.clear();
    triangleTexCoord_.clear();

    nbSlice = 2*nbSlice;
    float current_blue = 1.0f;
    float current_green = 0.0f;
    float theta = 0.0f;
    float thetaOffset = 2*3.14159/nbSlice;
    float colorOffset= 1.0f/nbSlice;
    float currentTex = 0.0f;
    float texOffset = 1.0f/nbSlice;

    for (int i=0; i<nbSlice+1;i++){
        //  inner circle vertice position
        trianglePosition_.push_back(r0*cos(theta));
        trianglePosition_.push_back(r0*sin(theta));
        trianglePosition_.push_back(0.0f);
        // inner circle vertice color
        triangleColor_.push_back(0);
        triangleColor_.push_back(current_green);
        triangleColor_.push_back(0);
        triangleColor_.push_back(1);
        // inner circle vertice texture
        triangleTexCoord_.push_back((r0*cos(theta)+1) /2);
        triangleTexCoord_.push_back((r0*sin(theta)+1) /2);
        // outer circle vertice position
        trianglePosition_.push_back(r1*cos(theta));
        trianglePosition_.push_back(r1*sin(theta));
        trianglePosition_.push_back(0.0f);
        // outer circle vertice color
        triangleColor_.push_back(0);
        triangleColor_.push_back(0);
        triangleColor_.push_back(current_blue);
        triangleColor_.push_back(1);
        // outer circle vertice texture
        triangleTexCoord_.push_back((r1*cos(theta)+1) /2);
        triangleTexCoord_.push_back((r1*sin(theta)+1) /2);

        // offsets update
        theta = theta + thetaOffset;
        current_blue = current_blue - colorOffset;
        current_green = current_green + colorOffset;
        currentTex = currentTex + texOffset;
    }
}



