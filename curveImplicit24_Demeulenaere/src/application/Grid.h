/*
 * Grid.h
 *
 *  Created on: 22 sept. 2010
 *      Author: aubert
 */

#pragma once

#include <vector>
#include "Implicit.h"

namespace p3d {
 class VertexArray;
}
#include "Vector3.h"

class Grid {
public:
  Grid();

  void initVAO();
  void updateVAO();

  void drawGrid();
	void drawSign();
  void drawIntersect();
  void drawEdge();
	void drawValues();
	void drawAllSquare();
	double xIndice(unsigned int i);
	double yIndice(unsigned int i);

	double x(unsigned int i);
	double y(unsigned int i);
  void eval(unsigned int i,unsigned int j,const Implicit &f);
  void evalAll(const Implicit &implicit);
	virtual ~Grid();

  p3d::Vector3 interpolate(const p3d::Vector3 &p1, const p3d::Vector3 &p2);
  void addEdgeSquare(std::vector<int> iVertex, std::vector<p3d::Vector3> &listEdge);
  void setSize(int lx, int ly);
  void evalTest();
private:
  int lx_,ly_;   // grid dimension
  std::vector<p3d::Vector3> point_;
  std::vector<double> value_; // value at each node of the grid
  void rotateSquare(std::vector<int> &iVertex, int nbRotate);
  void setupPoint();

  // draw
  p3d::VertexArray *vao_;
};


