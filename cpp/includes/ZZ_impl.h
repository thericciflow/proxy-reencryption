#pragma once

#include "ecpy_native.h"

struct ZZ {
  const methods_t *functions;
  ObjectType type;
  mpz_class x;
};


