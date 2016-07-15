#include "ecpy_native.h"

using namespace std;

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

__EXPORT__ void ZZ_destroy(const ZZ *zz) {
  delete zz;
}

__EXPORT__ ZZ *ZZ_add(const ZZ *a, const ZZ *b) {
  ZZ *ret = new ZZ;
  ret->x = a->x + b->x;
  return ret;
}

__EXPORT__ ZZ *ZZ_neg(const ZZ *a) {
  ZZ *ret = new ZZ;
  ret->x = -a->x;
  return ret;
}

__EXPORT__ ZZ *ZZ_mul(const ZZ *a, const ZZ *b) {
  ZZ *ret = new ZZ;
  ret->x = a->x * b->x;
  return ret;
}

__EXPORT__ ZZ *ZZ_div(const ZZ *a, const ZZ *b) {
  ZZ *ret = new ZZ;
  ret->x = a->x / b->x;
  return ret;
}

__EXPORT__ ZZ *ZZ_mod(const ZZ *a, const ZZ *b) {
  ZZ *ret = new ZZ;
  ret->x = a->x % b->x;
  return ret;
}

__EXPORT__ ZZ *ZZ_modinv(const ZZ *a, const ZZ *m) {
  auto _a = a->x;
  auto _m = m->x;
  if (_a < 0) {
    _a = _a + _m;
  }
  mpz_class t;
  mpz_invert(t.get_mpz_t(), _a.get_mpz_t(), _m.get_mpz_t());
  if (t < 0) {
    t += m->x;
  }
  return ZZ_create_from_mpz_class(t);
}

__EXPORT__ bool ZZ_to_string(const ZZ *zz, char *p, int maxlen) {
  const int base = 10;
  auto x = zz->x.get_mpz_t();
  auto len = mpz_sizeinbase(x, base) + 2;
  if (len <= maxlen) {
    mpz_get_str(p, base, x);
    return true;
  }
  return false;
}

string ZZ_to_string_as_std_string(const ZZ *zz) {
  return zz->x.get_str();
}

__EXPORT__ bool ZZ_is_equals(const ZZ *yy, const ZZ *zz) {
  return yy->x == zz->x;
}

ZZ *ZZ_copy(ZZ *zz) {
  ZZ *ret = new ZZ;
  ret->x = zz->x;
  return ret;
}
