#pragma once
/*!
*
* @file
*
* @brief
* @author F. Aubert
*
*/

#include "Primitive.h"
#include "Line.h"
#include "Matrix4.h"
#include "Material.h"
#include "Ray.h"
#include "Matrix3.h"
class IntersectionArray;


/**
@class CsgTree
@brief opérations sur les csg
*/

class CsgTree {

public:
  typedef enum {Op_Inter,Op_Union,Op_Diff,Node_Leaf} ENode;
    /// constructeur par défaut
  CsgTree();
  /// destructeur (deletes children and primitive)
  ~CsgTree();

    /// constructeur par champ pour un noeud interne
  CsgTree(const p3d::Matrix4 &trans,CsgTree *left,CsgTree *right,ENode op);
  /// constructeur par champ pour une feuille
    CsgTree(const p3d::Matrix4 &trans, Primitive *p);

    /// Calcul de la normale au point P d'une feuille
    p3d::Vector3 normalPrimitive(const p3d::Vector3 &p);


    inline const p3d::Matrix4 &matrix() const {return parentLocal_;}


    p3d::Vector3 pointWorld(const p3d::Vector3 &p);
    p3d::Vector3 directionWorld(const p3d::Vector3 &p);
    p3d::Vector3 pointLocal(const p3d::Vector3 &p);
    p3d::Vector3 directionLocal(const p3d::Vector3 &p);
    p3d::Vector3 normalWorld(const p3d::Vector3 &p);

    Primitive *primitive() const;


    bool isLeaf() const;

    /// prepare le csg (les feuilles contiendront le passage de racine à feuille et inverse
    //void cumulateMatrix();
    void composeMatrix(const p3d::Matrix4 &worldParent, const p3d::Matrix4 &parentWorld);


    void cumulateMatrix();
    static void cumulateMatrix(CsgTree *t, const p3d::Matrix4 &worldParent, const p3d::Matrix4 &parentWorld);

    void worldLocal(const p3d::Matrix4 &m) {worldLocal_=m;}
    void localWorld(const p3d::Matrix4 &m) {localWorld_=m;}
    void normalWorld(const p3d::Matrix3 &m) {normalWorld_=m;}

    const p3d::Matrix4 &parentLocal() const {return parentLocal_;}
    const p3d::Matrix4 &localParent() const {return localParent_;}

    const p3d::Matrix4 &worldLocal() const {return worldLocal_;}
    const p3d::Matrix4 &localWorld() const {return localWorld_;}

    static CsgTree* createNode(const p3d::Matrix4 &trans,CsgTree *left,CsgTree *right,ENode op);
    static CsgTree* createLeaf(const p3d::Matrix4 &trans,Primitive *p);

    CsgTree *left() const {return left_;}
    CsgTree *right() const {return right_;}
    ENode nodeType() const {return nodeType_;}

    void setNode(const p3d::Matrix4 &trans,CsgTree *left,CsgTree *right,ENode op);
    void setLeaf(const p3d::Matrix4 &trans, Primitive *p);

    inline void left(CsgTree *left) {left_=left;}
    inline void right(CsgTree *right) {right_=right;}
    inline void primitive(Primitive *p) {primitive_=p;nodeType_=Node_Leaf;}
    inline void nodeType(ENode op) {nodeType_=op;}
    inline void matrix(const p3d::Matrix4 &f) {parentLocal_=f;localParent_=f.inverse();}

    void print();


    static void draw(CsgTree *t);
    void drawLocal();
    void drawParent();

    IntersectionArray intersection(const Ray &ray);
private:
    p3d::Matrix4 parentLocal_;
    p3d::Matrix4 worldLocal_;

    p3d::Matrix4 localParent_;
    p3d::Matrix4 localWorld_;

    p3d::Matrix3 normalWorld_;

    CsgTree *right_{nullptr},*left_{nullptr}; // this is the owner (no sharing)
    Primitive *primitive_{nullptr}; // this is the owner
    CsgTree::ENode nodeType_{Node_Leaf}; // Node_Or | Node_And | Node_Diff | Node_Leaf

};



