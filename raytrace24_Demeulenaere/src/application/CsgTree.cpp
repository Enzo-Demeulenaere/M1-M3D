#include "CsgTree.h"
#include <iostream>
#include <math.h>
#include "CsgTree.h"
#include "IntersectionArray.h"
#include "Primitive.h"
#include "Matrix3.h"
#include "Tools.h"
#include "GLRender.h"
/**



@author F. Aubert
*/


using namespace std;
using namespace p3d;


/**
effectue l'intersection d'une droite et de l'arbre CSG
CU : ray doit être une droite dans le repère de la scène (i.e. dans le repère du noeud racine du CSG)
*/
IntersectionArray CsgTree::intersection(const Ray &ray) {
  IntersectionArray res;
  if (isLeaf()) { // intersection du rayon avec une primitive (= une feuille de l'arbre csg)
    res=IntersectionArray::intersectLeaf(this,ray);
  }
  else { // intersection du rayon avec un noeud interne => on doit faire la fusion entre les intersections du sous-arbre gauche et les intersections du sous-arbre droit
    IntersectionArray inter_left;
    IntersectionArray inter_right;
    // appels résursifs pour obtenir la liste des intersections à gauche et à droite
    inter_left=left_->intersection(ray);
    inter_right=right_->intersection(ray);
    // faire la fusion des listes d'intersections gauche et droite (la fusion dépendra de l'opération Node_Intersection, Node_Difference, ou Node_Union pour le noeud).
    res=IntersectionArray::fusion(inter_left,inter_right,nodeType());
  }
  return res;
}


/** *********************************************************************** */

CsgTree::~CsgTree() {
    delete right_;
    delete left_;
    delete primitive_;
}


CsgTree::CsgTree() {
    right_=NULL;
    left_=NULL;
    primitive_=NULL;
    nodeType_=Node_Leaf;
}

CsgTree::CsgTree(const Matrix4 &m,CsgTree *left,CsgTree *right,ENode op) {
    this->setNode(m,left,right,op);
}

CsgTree::CsgTree(const Matrix4 &m, Primitive *p) {
    this->setLeaf(m,p);
}


Vector3 CsgTree::normalPrimitive(const Vector3 &p) {
    if (!isLeaf()) throw ErrorD("only compute normal on a leaf");
    return primitive_->computeNormal(p);
}



Vector3 CsgTree::pointWorld(const Vector3 &p) {
    return worldLocal_.transformPoint(p);
}

Vector3 CsgTree::directionWorld(const Vector3 &u) {
    return worldLocal_.transformDirection(u);
}

Vector3 CsgTree::pointLocal(const Vector3 &p) {
    return localWorld_.transformPoint(p);
}

Vector3 CsgTree::directionLocal(const Vector3 &u) {
    return localWorld_.transformDirection(u);
}

Vector3 CsgTree::normalWorld(const Vector3 &n) {
  if (!isLeaf()) throw ErrorD("nomal matrix only computed for leaf");
  return normalWorld_*n;
}

Primitive *CsgTree::primitive() const {
    if (!isLeaf()) throw ErrorD("get primitive on a non-primitive");
    return primitive_;
}



void CsgTree::draw(CsgTree *t) {
  if (t->isLeaf()) {
    p3d::modelviewMatrix.push();
    p3d::modelviewMatrix*=t->worldLocal();
    t->primitive()->drawGL();
    p3d::modelviewMatrix.pop();
  }
  else {
    draw(t->left());
    draw(t->right());
  }
}

void CsgTree::drawLocal() {
    draw(this);
}



void CsgTree::drawParent() {
    p3d::modelviewMatrix.push();
    drawLocal();
    p3d::modelviewMatrix.pop();
}

bool CsgTree::isLeaf() const {
    return nodeType_==Node_Leaf;
}


void CsgTree::setNode(const Matrix4 &m,CsgTree *left,CsgTree *right,ENode op) {
    parentLocal_=m;
    localParent_=parentLocal_.inverse();
    this->left(left);
    this->right(right);
    this->primitive(nullptr);
    this->nodeType(op);
}

void CsgTree::setLeaf(const Matrix4 &m,Primitive *p) {
    parentLocal_=m;
    localParent_=parentLocal_.inverse();
    this->left(nullptr);
    this->right(nullptr);
    this->primitive(p);
    this->nodeType(Node_Leaf);
}


void CsgTree::print() {
    if (isLeaf()) {
        cout << "Primitive";
    } else {
        cout << "<";
        if (left_) left()->print(); else cout << "VIDE";
        cout << ",";
        if (right_) right()->print(); else cout << "VIDE";
        cout << ">";
    }
}


void CsgTree::cumulateMatrix() {
  // entry point for recursive tree walkthrough :
  cumulateMatrix(this,Matrix4::identity(),Matrix4::identity());
}

void CsgTree::composeMatrix(const Matrix4 &worldParent,const Matrix4 &parentWorld) {
    worldLocal_=worldParent*parentLocal_;
    localWorld_=localParent_*parentWorld;
}

void CsgTree::cumulateMatrix(CsgTree *t,const Matrix4 &worldParent,const Matrix4 &parentWorld) {
  if (t) {
    t->composeMatrix(worldParent,parentWorld);
    cumulateMatrix(t->right(),t->worldLocal(),t->localWorld());
    cumulateMatrix(t->left(),t->worldLocal(),t->localWorld());
    if (t->isLeaf()) {
      t->normalWorld(t->worldLocal().normalMatrix());
    }
  }
}













