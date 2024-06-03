#include "IntersectionArray.h"
#include <iostream>
#include "Tools.h"

using namespace p3d;
using namespace std;

/**
@file
@author Fabrice Aubert
*/



/**
Construit la liste d'intersection this comme la fusion des 2 listes triées left et right selon l'opération op
L'opération op peut prendre les valeurs CsgTree::Op_Inter, CsgTree::Op_Inter ou CsgTree::Op_Diff
*/
IntersectionArray IntersectionArray::fusion(const IntersectionArray &left,const IntersectionArray &right,CsgTree::ENode op) {

  IntersectionArray res;

  bool sL,sR,sCurrent,sNew;  // états :
  //        - true correspond à IN (intersection qui fait passer de l'extérieur à l'intérieur du volume)
  //        - false correspond à  OUT (intersection qui fait passer de l'intérieur à l'extérieur du volume)

  sL=false;  // état courant gauche
  sR=false;  // état courant droite
  sCurrent=false;  // état courant de la fusion
  sNew=false; // nouvel état de la fusion après mise à jour de sL et sR

  size_t iL=0; // indice de l'intersection courante à gauche
  size_t iR=0; // indice de l'intersection courante à droite

    /**
    * il faut ajouter à this (liste d'intersections initialement vide) les intersections résultantes de la fusion (algo vu en cours) :
    * - left et right (paramètres de la méthode) sont les tableaux (triés) des intersections qu'on doit fusionner.
    * - op (paramètre de la méthode) est l'opérateur de fusion (valeurs symboliques possibles : CsgTree::Op_Inter, CsgTree::Op_Union et CsgTree::Op_Diff).
    * - left[iL] et right[iR] permettent de récupérer chaque intersection
    * - une_intersection.lambda() donne le lambda
    * - intersection_.push_back(une_intersection) ajoute une_intersection à la fin de la liste à construire.
    * - sL, sR, sCurrent sont les états courants des listes gauche, droite et résultat
    * - sNew est l'état résultant de la combinaison des états courants sL et sR (pour déterminer s'il y a changement d'état).
    *
    */
  res.clear(); // initialisation de la fusion
  // while(....)

  while(iL < left.size() || iR < right.size()){
      double lambdaL = MaxInfinity;
      double lambdaR = MaxInfinity;
      IntersectionInfo intersect;

      if (iL < left.size()){
          lambdaL = left[iL].lambda();
      }
      if (iR < right.size()){
          lambdaR = right[iR].lambda();
      }
      if (lambdaL <= lambdaR){
          sL = not sL;
          intersect = left[iL];
          iL++;
      }
      else{
          sR = not sR;
          intersect = right[iR];
          iR++;
      }

      // operations behaviors
      if (op == CsgTree::Op_Inter) sNew = sL and sR;
      if (op == CsgTree::Op_Union) sNew = sL or sR;
      if (op == CsgTree::Op_Diff) sNew = sL and not sR;

      if (sNew != sCurrent){
          res.push_back(intersect);
          sCurrent = sNew;
      }
  }

  return res;

}


IntersectionArray IntersectionArray::intersectPlane(double a,double u) {
  double lambda1,lambda2;

  /* Intersection rayon avec demi espace délimité par le plan passant par (0,0,0) et de normal (0,0,1)
   * Dans ce cas, pour le rayon (A,u), seuls le A.z() et le u.z() sont nécessaires (paramètres a et u)
   * A noter que la fonction est générique selon que la normale est (1,0,0), (0,1,0), etc (appeler avec a et u correspondants)
   * La normale est orientée vers l'extérieur du demi espace
   * Il faut faire les cas 0 intersection ou 2 intersections (déterminer lambda1 et lambda2)
   * Pour traduire une intersection à l'infini, utiliser les constantes MinInfinity et MaxInfinity
   * */
  double epsilon = 1e-5;
  if(fabs(u) < epsilon){
      if(a<0){
          lambda1 = MinInfinity;
          lambda2 = MaxInfinity;
      }
  }
  else{
      lambda1 = -a/u;
      if(u<0) lambda2 = MaxInfinity;
      else lambda2 = MinInfinity;
      swapIfMin(lambda1,lambda2);
  }
  //if(lambda1>0) printf("l1 : %f, l2 : %f \n",float(lambda1),float(lambda2));
  return fromInterval(lambda1,lambda2); // renvoie un IntersectionArray avec les 2 lambda qui seront ordonnés (cf méthode à la fin)
}


IntersectionArray IntersectionArray::intersectInterval(const IntersectionArray &i1,const IntersectionArray &i2) {
  IntersectionArray res;

  /* Il faut créer l'intervalle (par res.push_back(une intersection) résultant de l'intersection des
   * intervalles i1 et i2 (contiennent 0 ou 2 intersections).
   * i1[0], i1[1] : intervalle i1 (ordonné). i1[0].lambda() et i1[1].lambda() donnent les lambdas correspondants
   * idem pour i2.
   * Il "suffit" d'appliquer le max des inf et le min des sup selon le principe déjà rencontré pour le TP sur les axes séparateurs
   *
   * */
  if (i1.size()<2 || i2.size()<2){
      return res;
  }

  double start = max(i1[0].lambda(),i2[0].lambda());
  double end = min(i1[1].lambda(),i2[1].lambda());

  if (start <= end){
      res.push_back(start);
      res.push_back(end);
  }

  return res;
}

/// intersection with a leaf t (t should be a leaf where primitive is not null)
IntersectionArray IntersectionArray::intersectLeaf(CsgTree *t,const Ray &r) {
  if (!t->isLeaf()) throw ErrorD("t is not a leaf");
  Ray rayLocal(t->pointLocal(r.point()),t->directionLocal(r.direction()));
  IntersectionArray res=t->primitive()->intersection(rayLocal);
  for(auto &i:res) {
    i.node(t);
  }
  return res;
}

IntersectionArray IntersectionArray::fromInterval(double lambda1, double lambda2) {
  IntersectionArray res;
  swapIfMin(lambda1,lambda2);
  res.push_back(IntersectionInfo(lambda1));
  res.push_back(IntersectionInfo(lambda2));
  return res;
}




/** *********************************************************************************************** */



