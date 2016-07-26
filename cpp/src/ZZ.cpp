#include "ecpy_native.h"
#include "ZZ_impl.h"

using namespace std;
using namespace g_object;

MAKE_FUNC_TABLE(_zz_func, ZZ_destroy, ZZ_add, ZZ_neg, ZZ_mul, ZZ_div, nullptr, ZZ_mod, ZZ_is_equals, ZZ_is_same_type, ZZ_to_string_as_std_string, ZZ_copy);

ZZ *ZZ_create_from_mpz_class(mpz_class num) {
  ZZ *zz = new ZZ;
  zz->x = num;
  zz->functions = _zz_func;
  zz->type = ObjectType::ZZ;
  return zz;
}

__EXPORT__ ZZ *ZZ_create(const char *num) {
  ZZ *zz = new ZZ;
  zz->x = mpz_class(num);
  zz->functions = _zz_func;
  zz->type = ObjectType::ZZ;
  return zz;
}

__EXPORT__ void ZZ_destroy(ZZ *zz) {
  zz->type = ObjectType::FREE;
  delete zz;
}

__EXPORT__ ZZ *ZZ_add(const ZZ *a, const ZZ *b) {
  ZZ *ret = new ZZ;
  ret->functions = _zz_func;
  ret->type = ObjectType::ZZ;
  ret->x = a->x + b->x;
  return ret;
}

__EXPORT__ ZZ *ZZ_neg(const ZZ *a) {
  ZZ *ret = new ZZ;
  ret->functions = _zz_func;
  ret->type = ObjectType::ZZ;
  ret->x = -a->x;
  return ret;
}

__EXPORT__ ZZ *ZZ_mul(const ZZ *a, const ZZ *b) {
  ZZ *ret = new ZZ;
  ret->functions = _zz_func;
  ret->type = ObjectType::ZZ;
  ret->x = a->x * b->x;
  return ret;
}

__EXPORT__ ZZ *ZZ_div(const ZZ *a, const ZZ *b) {
  ZZ *ret = new ZZ;
  ret->functions = _zz_func;
  ret->type = ObjectType::ZZ;
  ret->x = a->x / b->x;
  return ret;
}

__EXPORT__ ZZ *ZZ_mod(const ZZ *a, const ZZ *b) {
  ZZ *ret = new ZZ;
  ret->functions = _zz_func;
  ret->type = ObjectType::ZZ;
  ret->x = a->x % b->x;
  if (ret->x < 0) {
    ret->x += b->x;
  }
  return ret;
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
  ret->functions = _zz_func;
  ret->type = ObjectType::ZZ;
  ret->x = zz->x;
  return ret;
}

__EXPORT__ ZZ *ZZ_modinv(const ZZ *a, const ZZ *m) {
  auto _a = a->x;
  auto _m = m->x;
  return ZZ_create_from_mpz_class(modinv(_a, _m));
}

__EXPORT__ int ZZ_jacobi(const ZZ *a, const ZZ *b) {
  auto t = a->x.get_mpz_t();
  auto u = b->x.get_mpz_t();
  return mpz_jacobi(t, u);
}

__EXPORT__ int ZZ_legendre(const ZZ *a, const ZZ *b) {
  auto t = a->x.get_mpz_t();
  auto u = b->x.get_mpz_t();
  return mpz_legendre(t, u);
}

bool ZZ_is_same_type(const g_object_t* a, const g_object_t *b) {
  return a->type == b->type && a->type == ObjectType::ZZ;
}
