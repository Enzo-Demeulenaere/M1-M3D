#pragma once

#include <vector>

#include "glsupport.h"

class BasicVAO
{
  GLuint vao_{0};

  GLuint vbo_{0}; // VBO (all attributes)
  GLuint ibo_{0}; // Element Buffer (all indexes)

  int nbElement_{0}; // nb vertices to draw

public:
  ~BasicVAO();
  BasicVAO();
  void release();

  void init(const std::vector<int> &index, const std::vector<float> &position, const std::vector<float> &normal);
  void draw();

  void initCube();
  void initTorus(size_t nbSlice, size_t nbStack, double rg, double rp);
  void initCylinder(size_t nbSlice);
};


