#pragma once
#include "ecpy_native.h"

struct EF {
  const methods_t *functions;
  ObjectType type;
  IrreduciblePolynomialType poly;
  g_object_t *modulo;
  g_object_t *x, *y;
};


