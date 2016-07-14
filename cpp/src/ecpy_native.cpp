#include "ecpy_native.h"

using namespace std;

struct ZZ {
  mpz_class x;
};

struct FF {
  ZZ *p;
};

__EXPORT__ ZZ *ZZ_create(const char *num) {
  ZZ *zz = new ZZ;
  zz->x = mpz_class(num);
  return zz;
}

__EXPORT__ void ZZ_destroy(ZZ *zz) {
  delete zz;
}

__EXPORT__ ZZ *ZZ_add(ZZ *a, ZZ *b) {
  ZZ *ret = new ZZ;
  ret->x = a->x + b->x;
  return ret;
}

__EXPORT__ ZZ *ZZ_neg(ZZ *a) {
  ZZ *ret = new ZZ;
  ret->x = -a->x;
  return ret;
}

__EXPORT__ ZZ *ZZ_mul(ZZ *a, ZZ *b) {
  ZZ *ret = new ZZ;
  ret->x = a->x * b->x;
  return ret;
}

__EXPORT__ ZZ *ZZ_div(ZZ *a, ZZ *b) {
  ZZ *ret = new ZZ;
  ret->x = a->x / b->x;
  return ret;
}

__EXPORT__ const char *ZZ_to_string(ZZ *zz) {
  stringstream ss;
  ss << zz->x;
  return ss.str().c_str();
}
