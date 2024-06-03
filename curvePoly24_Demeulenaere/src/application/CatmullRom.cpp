#include "GLDrawCamera.h"
#include "CatmullRom.h"
#include "Vector3.h"
#include "Tools.h"

#include "Curve.h"
#include "Bezier.h"

#include "Matrix4.h"
#include "GLTool.h"

/**
@file
@author F. Aubert
*/


using namespace p3d;
using namespace std;


/**
* Tracé de la courbe d'interpolation
*/

void CatmullRom::draw() {


  // A COMPLETER :
  //
  // doit tracer une courbe en reliant 2 points saisis successifs par une bézier cubique
  // nbPoint() vous donne le nombre de points saisis
  // on doit donc tracer nbPoint()-1 courbes de bézier cubiques : la ième courbe a pour points de controle point(i),intermediate(i,0),intermediate(i,1) et point(i+1)
  // il suffit donc d'affecter la variable "cubic" avec ces points :
  // => affectez les points de contrôle de cubic avec le setter cubic.point(j,un_point)
  // puis de la tracer (en appelant simplement cubic.draw() ).


  Bezier cubic;
  cubic.resize(4); // Bézier cubique = 4 points de contrôle

  if (nbPoint()<2) return; // tracé uniquement si plus de 2 points saisis.

  for (int i=0; i < int(nbPoint()-1);i++){
    cubic.point(0,point(i));
    cubic.point(1,intermediate(i,0));
    cubic.point(2,intermediate(i,1));
    cubic.point(3,point(i+1));
    cubic.draw();
  }



}


/**
*  Calcul des points intermédiaires pour obtenir une continuité C1 par Catmull-Rom
*/
void CatmullRom::setup() {
  Vector3 parallele;

  // A COMPLETER : il s'agit d'initialiser les points intermédiaires (les points bleus) en appelant les setters intermediate(i,0,un Vector3) et intermediate(i,1,un Vector3) )
  // les points intermédiaires doivent être fixés par la méthode vue en cours (tangentes parallèles aux segments [point(i-1),point(i+1)]).

  double k = 0.4;
  int n = int(nbPoint());
  for (int i =1; i<n-1;i++){
      Vector3 direction = point(i+1)-point(i-1);
      Vector3 pointPos = direction * k/2;
      this->intermediate(i-1,1,point(i) - pointPos);
      this->intermediate(i,0,point(i) + pointPos);
  }
  if (nbPoint()<2) return;
  // First intermediate between P0 and I0,1
  Vector3 direction0 = (intermediate(0,1)-point(0))/2.0;
  Vector3 i00 = point(0) + direction0;
  this->intermediate(0,0,i00);
  // Last intermediate between In,0 and Pn
  Vector3 directionN = (point(n-1)-intermediate(n-2,0))/2;
  Vector3 in1 = intermediate(n-2,0) + directionN;
  this->intermediate(n-2,1,in1);
}



/** *********************************************************************************************************** */
/** *********************************************************************************************************** */
/** *********************************************************************************************************** */
/** *********************************************************************************************************** */


void CatmullRom::resize(size_t nb) {
  Curve::resize(nb);
  intermediate_.resize(2*(nb-1));
}

void CatmullRom::interactInsert(size_t i, const Vector3 &insertP) {
    Curve::interactInsert(i/3+i%3,insertP);
    addIntermediate();
}


const Vector3 &CatmullRom::intermediate(size_t i,size_t j) const {
  return intermediate_[2*i+j];
}

void CatmullRom::intermediate(size_t i,size_t j,const Vector3 &p) {
  intermediate_[2*i+j]=p;
}

void CatmullRom::addIntermediate() {
  if (nbPoint()<2) return;
  size_t i=nbPoint()-2;

  if (intermediate_.size()!=(nbPoint()-2)*2)
    throw ErrorD("incoherence between pts and intermediate");

  Vector3 p1=point(i)+0.33*(point(i+1)-point(i));
  Vector3 p2=point(i)+0.66*(point(i+1)-point(i));
  intermediate_.push_back(p1);
  intermediate_.push_back(p2);
}



void CatmullRom::drawPoint() {
  vector<Vector3> allPts;

  for(size_t i=0; i<nbPoint(); i++) {
    allPts.push_back(point(i));
    p3d::draw(int(i),point(i)+Vector3(0.02,0.02,0.0));
  }
  p3d::shaderVertexAmbient();
  p3d::draw(GL_POINTS,allPts);
}

vector<Vector3> *CatmullRom::intermediatev() {
  return &intermediate_;
}

void CatmullRom::drawControl(int mode) {
  if (nbPoint()>0) {
    if (mode>0) {
      p3d::uniformAmbient(1,0,0);
      if (mode==1) {
        drawPoint();
      }
      if (mode>1) {
        Curve::drawControl();
        if (mode>2) {
          p3d::uniformAmbient(0,0,1);
          for(unsigned int i=0; i<nbPoint()-1; i++) {
            p3d::drawLines(vector<Vector3>{point(i),intermediate(i,0),
                            intermediate(i,0),intermediate(i,1),
                            intermediate(i,1),point(i+1)});
            p3d::drawPoints(vector<Vector3>{intermediate(i,0),intermediate(i,1)});
          }
        }
      }
    }
  }
}

Bezier CatmullRom::segment(size_t i) {
  Bezier cubic;
  cubic.resize(4);

  cubic.point(0,point(i));
  cubic.point(1,intermediate(i,0));
  cubic.point(2,intermediate(i,1));
  cubic.point(3,point(i+1));

  return cubic;
}


Bezier CatmullRom::segment(double t,double *tCubic) {
  double intpart,fracpart;
  fracpart=modf(t*(nbPoint()-1.0),&intpart);
  double realT=fracpart;
  size_t whichCurve=size_t(intpart);


  Bezier cubic;
  cubic.resize(4);

  cubic.point(0,point(whichCurve));
  cubic.point(1,intermediate(whichCurve,0));
  cubic.point(2,intermediate(whichCurve,1));
  cubic.point(3,point(whichCurve+1));

  *tCubic=realT;

  return cubic;
}



void CatmullRom::whichSegment(double t,size_t *cubicNumber,double *tCubic) {
  double intpart,fracpart;
  fracpart=modf(t*(nbPoint()-1.0),&intpart);
  *tCubic=fracpart;
  *cubicNumber=size_t(intpart);
}



Vector3 CatmullRom::eval(double t) {
  double tCubic;
  Bezier c=segment(t,&tCubic);
  return c.eval(tCubic);
}



