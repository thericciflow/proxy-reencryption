#pragma once
#include "ecpy_native.h"

struct EC {
  const methods_t *functions;
  ObjectType obj_type;
  g_object_t *a, *b;
  EC_Type type;
};

