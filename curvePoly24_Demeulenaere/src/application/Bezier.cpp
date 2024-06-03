#include "Bezier.h"

#include "Vector3.h"
#include "Matrix4.h"
#include "Tools.h"

#include <vector>

#include "GLDrawCamera.h"

using namespace p3d;
using namespace std;



/**
* Evaluation de la Curve de Bézier en $t$
**/
Vector3 Bezier::eval(double t) {
    Vector3 result;
    if (nbPoint()>1) {
      // on recopie les points de controles dans le tableau castel (castel est donc initialisé avec la première ligne de l'algo triangulaire).
      vector<Vector3> castel=controlPts();      // tableau de points sur lesquels on applique deCasteljau

        // A COMPLETER : appliquer la méthode de De Casteljau (évolution du tableau castel)
        for (int k= nbPoint(); k>0;k--){
            for(int i=0; i<k-1;i++){
                castel[i]= (1-t)*castel[i] + t*castel[i+1];
            }
        }

        // le point de la courbe en t doit se trouver dans castel[0] à la fin de l'algo
        result=castel[0];
    }
    return result;
}




void Bezier::draw() {
  if (nbPoint()<2) return;


    double pas=1.0/(100.0-1);
    double t=0;

    vector<Vector3> drawPts;

    for(int i=0;i<100;i++) {
        drawPts.push_back(eval(t));
        t+=pas;
    }
    p3d::drawThickLineStrip(drawPts);
}


