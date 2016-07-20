#pragma once
#include "ecpy_native.h"

struct FF {
  const methods_t *functions;
  ObjectType type;
  g_object_t *x;
  g_object_t *p;
};

