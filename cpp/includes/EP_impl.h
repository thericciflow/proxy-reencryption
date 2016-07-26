#pragma once
#include "ecpy_native.h"

struct EP {
  const methods_t *functions;
  ObjectType objtype;
  const EC *curve;
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

#define MUL_P1_RE(x1, x2, y1, y2) (x1 * y1 - x2 * y2)
#define MUL_P1_IM(x1, x2, y1, y2) (x2 * y1 + x1 * y2)
#define POW2_P1_RE(x, y) (x * x - y * y)
#define POW2_P1_IM(x, y) (2 * x * y)
#define SUB_P1_RE(x1, x2, y1, y2) (x1 - y1)
#define SUB_P1_IM(x1, x2, y1, y2) (x2 - y2)
#define MUL_P2_RE(x1, x2, y1, y2) (x1 * y1 - x2 * y2)
#define MUL_P2_IM(x1, x2, y1, y2) (x2 * y1 + x1 * y2 - x2 * y2)
#define POW2_P2_RE(x, y) (x * x - y * y)
#define POW2_P2_IM(x, y) (2 * x * y - y * y)
#define SUB_P2_RE(x1, x2, y1, y2) (x1 - y1)
#define SUB_P2_IM(x1, x2, y1, y2) (x2 - y2)
