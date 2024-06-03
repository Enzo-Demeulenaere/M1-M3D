#include "Primitive.h"
#include <math.h>
#include <iostream>
#include "Matrix4.h"
#include "Material.h"

/**
@file
@author Fabrice Aubert
*/

using namespace std;
using namespace p3d;




void Primitive::materialGL() {
  material_.setGL();
}

