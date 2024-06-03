/*
 * Grid.cpp
 *
 */

#include <cmath>
#include "Grid.h"
#include <iostream>
#include <sstream>
#include <string>

#include "Vector3.h"
#include "VertexArray.h"
#include "GLRender.h"
#include "GLDraw.h"

using namespace std;
using namespace p3d;


/**
 * @brief constructor (empty grid)
 */
Grid::Grid() {
  lx_=0;
  ly_=0;
  point_.clear();
}


/**
 * @brief set the size of the grid. Number of square is (lx-1)*(ly-1)
 * @param lx : number of vertex in x direction
 * @param ly : number of vertex in y direction
 */
void Grid::setSize(int lx,int ly) {
  lx_=lx;
  ly_=ly;
  point_.resize(lx_*ly_);
  setupPoint();
}

/**
 * @brief set the position (x,y) of each vertex of the grid
 * Note : each point is Vector3 : z is the value of the implicit function (or height value)
 */
void Grid::setupPoint() {
  double stepX = 4.0/(lx_-1);
  double stepY = 4.0/(ly_-1);
  double x = -2;
  double y = -2;
  int current = 0;
  for(int j=0;j<ly_;j++) {
    x=-2.0;
    for(int i=0;i<lx_;i++) {
      point_[current]=Vector3(x,y,0);
      current+=1;
      x+=stepX;
    }
    y+=stepY;
  }

}

/**
 * @brief vao of the grid for drawing
 */
void Grid::initVAO() {
  vao_=p3d::fromGridLine(point_,lx_,ly_);
}

/**
 * @brief update the position of each vertex of the grid in the buffer of the vao
 */
void Grid::updateVAO() {
  vao_->update("position",point_);
}


/**
 * @brief Grid::drawGrid
 */
void Grid::drawGrid() {
  updateVAO();
	glLineWidth(2);
  p3d::ambientColor=Vector4(0,0.8,0.8,1.0);
  p3d::shaderVertexAmbient();
  vao_->draw();
}


/**
 * @brief eval the height (the z coordinate) of each vertex of the grid according to the function implicit
 * @param implicit : the implicit function (implicit.eval(x,y) gives the height of the point (x,y))
 */
void Grid::evalAll(const Implicit &implicit) {
  // TODO : point_ is the array that contains all the grid points
    for (int i =0; i< int(point_.size());i++){
        point_[i].z(implicit.eval(Vector2(point_[i].x(),point_[i].y())));
    }
}

/**
 * @brief to test the marching cube. Useful to test each case on a 2 x 2 grid
 */
void Grid::evalTest() {
  point_[0].z(-5);
  point_[1].z(5);
  point_[2].z(-5);
  point_[3].z(-5);
}


/**
 * @brief draw each vertex of the grid : blue for negative sign (i.e. height<0), and red for positive sign (i.e. height>0)
 */
void Grid::drawSign() {
  vector<Vector3> color; // affecter color par push_back pour chaque point de la grille
  // TODO
  for (int i =0; i<int(point_.size());i++){
      float pZ = point_[i].z();
      if (pZ < 0) color.push_back(Vector3(0,0,1));
      else color.push_back(Vector3(1,0,0));
  }

  if (color.size()==point_.size()) { // to avoid seg fault if the array color is wrongly set
    p3d::shaderVertexColor();
    p3d::drawPoints(point_,color);
  }
}


/**
 * @return the linear interpolation of p1 and p2 (the z of the result should be 0)
 * p1.x(),p1.y() is the position of p1, and p1.z() is the height (similar for p2)
 **/
Vector3 Grid::interpolate(const Vector3 &p1,const Vector3 &p2) {
  Vector3 res(0,0,0);
  // TODO
  float p1Z = p1.z();
  float p2Z = p2.z();
  float diff = fabs(p1Z - p2Z);

  res = (1- fabs(p1Z/diff))*p1 + (1- fabs(p2Z/diff))*p2;

  return res;

}

/**
 * draw all intersection between the plane z=0 and the grid
 */
void Grid::drawIntersect() {
  vector<Vector3> intersect; // affecter intersect par push_back (parcourir chaque arête de la grille pour calculer le point d'intersection à ajouter s'il existe)
  // TODO
  for (int i = 0; i<lx_;i++){
      for (int j =0; j<ly_;j++){
          Vector3 p = point_[i*lx_+j];
          if (i < lx_-1){
            Vector3 nextPV = point_[(i+1)*lx_ + j];
            if((p.z()<0 and nextPV.z()>0) or (p.z()>0 and nextPV.z()<0)){
                intersect.push_back(interpolate(p,nextPV));
            }
          }
          if (j < ly_-1){
            Vector3 nextPH = point_[i*lx_ + j+1];
            if((p.z()<0 and nextPH.z()>0) or (p.z()>0 and nextPH.z()<0)){
                intersect.push_back(interpolate(p,nextPH));
            }
          }

      }
  }

  p3d::ambientColor=Vector4(0,1,0,1);
  p3d::shaderVertexAmbient();
  p3d::drawPoints(intersect);
}


/**
 *  add edges of the square iVertex in listEdge (add two points for each edge)
 *  @in iVertex contains the 4 index of the vertices in grid that form a square
 *  For example : iVertex = {0,1,11,10} for the first square of a 10x10 grid
*/
void Grid::addEdgeSquare(vector<int> iVertex, std::vector<Vector3> &listEdge) {
  int configure = 0; // the configuration of the square according to signs
  // TODO :
  // - determine the configuration according to signs
  // - for each case, rotate iVertex to get a basic case
  // - for each basic case add the vertices in listEdge
  if (point_[iVertex[0]].z()>0) configure+= 8;
  if (point_[iVertex[1]].z()>0) configure+= 4;
  if (point_[iVertex[2]].z()>0) configure+= 2;
  if (point_[iVertex[3]].z()>0) configure+= 1;

  switch (configure) {
      // Nos cas de base seront : 0,15 ,1,7,5 et 3
      case 2: rotateSquare(iVertex,1); configure = 1; break; // -> 1
      case 4: rotateSquare(iVertex,2); configure = 1; break; // -> 2 -> 1
      case 6: rotateSquare(iVertex,1); configure = 3; break; // -> 3
      case 8: rotateSquare(iVertex,3); configure = 1; break ; // -> 4 -> 2 -> 1
      case 9: rotateSquare(iVertex,3); configure = 3; break ; // -> 12 -> 6 -> 3
      case 10: rotateSquare(iVertex,1); configure = 5; break ; // -> 5
      case 11: rotateSquare(iVertex,3); configure = 7; break ; // -> 13 -> 14 -> 7
      case 12: rotateSquare(iVertex,2); configure = 3; break ; // -> 6 -> 3
      case 13: rotateSquare(iVertex,2); configure = 7; break ; // -> 14 -> 7
      case 14: rotateSquare(iVertex,1); configure = 7; break ; // -> 7
  }
  switch (configure) {
      case 1:
        listEdge.push_back(interpolate(point_[iVertex[0]],point_[iVertex[3]]));
        listEdge.push_back(interpolate(point_[iVertex[3]],point_[iVertex[2]]));
        break;
      case 3:
        listEdge.push_back(interpolate(point_[iVertex[0]],point_[iVertex[3]]));
        listEdge.push_back(interpolate(point_[iVertex[1]],point_[iVertex[2]]));
        break;
      case 5:
        listEdge.push_back(interpolate(point_[iVertex[0]],point_[iVertex[1]]));
        listEdge.push_back(interpolate(point_[iVertex[0]],point_[iVertex[3]]));
        listEdge.push_back(interpolate(point_[iVertex[2]],point_[iVertex[3]]));
        listEdge.push_back(interpolate(point_[iVertex[2]],point_[iVertex[1]]));
        break;
      case 7:
        listEdge.push_back(interpolate(point_[iVertex[0]],point_[iVertex[3]]));
        listEdge.push_back(interpolate(point_[iVertex[0]],point_[iVertex[1]]));
        break;
  }

}

/**
 *  draw all edges of the implicit curve. Apply marching square
*/
void Grid::drawEdge() {
  vector<Vector3> listEdge; // edges to draw (two Vector3 per edge)
  vector<int> iVertex={0,0,0,0}; // contains indexes of each vertex of grid_
                           // for example : for the first square, t is {0,1,lx_+1,lx_)
  // TODO :
  // - set iVertex for each square
  // - call addEdgeSquare for this iVertex
    for (int i=0; i<lx_;i++){
        for (int j=0; j<ly_;j++){
//            Vector3 p0 = point_[i*lx_+j];
//            Vector3 p1 = point_[i*lx_+j+1];
//            Vector3 p2 = point_[(i+1)*lx_+j+1];
//            Vector3 p3 = point_[(i+1)*lx_+j];
            iVertex[0] = i*lx_+j;
            iVertex[1] = i*lx_+j+1;
            iVertex[2] = (i+1)*lx_+j+1;
            iVertex[3] = (i+1)*lx_+j;
            addEdgeSquare(iVertex,listEdge);
        }
    }


  // draw listEdge:
  p3d::ambientColor=Vector4(0.9,0.5,0.5,1);
  p3d::shaderVertexAmbient();
  p3d::drawLines(listEdge);
}


/**
 * swap values of iVertex according to nbRotate (number of rotation of values of t)
 * iVertex is modified
 * **/
void Grid::rotateSquare(std::vector<int> &iVertex, int nbRotate) {
  // TODO
  int iV3 = iVertex[3];
  iVertex[3] = iVertex[2];
  iVertex[2] = iVertex[1];
  iVertex[1] = iVertex[0];
  iVertex[0] = iV3;
  if (nbRotate>1) rotateSquare(iVertex,nbRotate-1);
}


/**
 * @brief Grid::~Grid
 */
Grid::~Grid() {
  delete vao_;
}

