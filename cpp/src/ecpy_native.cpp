#include "ecpy_native.h"

using namespace std;

struct ZZ {
  mpz_class x;
};

ZZ *ZZ_create_from_mpz_class(mpz_class num) {
  ZZ *zz = new ZZ;
  zz->x = num;
  return zz;
}

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

__EXPORT__ ZZ *ZZ_mod(ZZ *a, ZZ *b) {
  ZZ *ret = new ZZ;
  ret->x = a->x % b->x;
  return ret;
}

__EXPORT__ ZZ *ZZ_modinv(ZZ *a, ZZ *m) {
  auto _a = a->x;
  auto _m = m->x;
  if (_a < 0) {
    _a = _a + _m;
  }
  mpz_class t;
  mpz_invert(t.get_mpz_t(), _a.get_mpz_t(), _m.get_mpz_t());
  return ZZ_create_from_mpz_class(t);
}

__EXPORT__ const char *ZZ_to_string(ZZ *zz) {
  stringstream ss;
  ss << zz->x;
  return ss.str().c_str();
}
