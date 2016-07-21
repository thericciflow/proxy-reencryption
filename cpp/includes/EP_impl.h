#pragma once
#include "ecpy_native.h"

struct EP {
  const methods_t *functions;
  ObjectType objtype;
  EC *curve;
  g_object_t *x, *y, *z;
  union {
    struct {
      g_object_t *p;
    } FF;
    struct {
      g_object_t *modulo;
      IrreduciblePolynomialType type;
    } EF;
  } u;
};

