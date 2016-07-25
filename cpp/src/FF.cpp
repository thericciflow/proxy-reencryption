#include "ecpy_native.h"
#include "FF_impl.h"

using namespace std;
using namespace g_object;

MAKE_FUNC_TABLE(_ff_func, FF_destroy, FF_add, FF_neg, FF_mul, FF_div, nullptr, FF_mod, FF_is_equals, FF_is_same_type, FF_to_string_as_std_string, FF_copy);

FF *FF_create_from_mpz_class(mpz_class x, mpz_class p) {
  FF *ff = new FF;
  ff->p = AS_OBJECT(ZZ_create_from_mpz_class(p));
  while (x < p) {
    x += p;
  }
  ff->x = AS_OBJECT(ZZ_create_from_mpz_class(x % p));
  ff->type = ObjectType::FF;
  ff->functions = _ff_func;
  return ff;
}

__EXPORT__ FF *FF_create(const char *x, const char *p) {
  FF *ff = new FF;
  ff->type = ObjectType::FF;
  ff->functions = _ff_func;
  ff->p = AS_OBJECT(ZZ_create(p));
  auto t = AS_OBJECT(ZZ_create(x));
  ff->x = mod(t, ff->p);
  destroy(t);
  return ff;
}

__EXPORT__ void FF_destroy(FF *ff) {
  destroy(ff->x);
  destroy(ff->p);
  ff->type = ObjectType::FREE;
  delete ff;
}

__EXPORT__ FF *FF_add(const FF *a, const FF *b) {
  FF *ret = new FF;
  ret->type = ObjectType::FF;
  ret->functions = _ff_func;
  if (is_same_type(AS_OBJECT(const_cast<FF*>(a)), AS_OBJECT(const_cast<FF*>(b)))) {
    auto t = add(a->x, b->x);
    ret->x = mod(t, a->p);
    ret->p = copy(a->p);
    destroy(t);
  } else {
    throw logic_error("Invalid Operation: different modulus arithmetic");
  }
  return ret;
}

__EXPORT__ FF *FF_neg(const FF *a) {
  FF *ret = new FF;
  ret->type = ObjectType::FF;
  ret->functions = _ff_func;
  auto t = neg(a->x);
  ret->x = add(t, a->p);
  ret->p = copy(a->p);
  destroy(t);
  return ret;
}

__EXPORT__ FF *FF_mul(const FF *a, const FF *b) {
  FF *ret = new FF;
  ret->type = ObjectType::FF;
  ret->functions = _ff_func;
  if (equals(a->p, b->p)) {
    auto t = mul(a->x, b->x);
    ret->x = mod(t, a->p);
    ret->p = copy(a->p);
    destroy(t);
  } else {
    throw logic_error("Invalid Operation: different modulus arithmetic");
  }
  return ret;
}

__EXPORT__ FF *FF_div(const FF *a, const FF *b) {
  FF *ret = new FF;
  ret->type = ObjectType::FF;
  ret->functions = _ff_func;
  if (is_same_type(a->p, b->p)) {
    auto t = AS_OBJECT(ZZ_modinv(to_ZZ(b->x), to_ZZ(a->p)));
    auto u = mul(a->x, t);
    ret->x = mod(u, a->p);
    ret->p = copy(a->p);
    destroy(t);
    destroy(u);
  } else {
    throw logic_error("Invalid Operation: different modulus arithmetic");
  }
  return ret;
}

__EXPORT__ FF *FF_mod(const FF *a, const FF *b) {
  FF *ret = new FF;
  ret->type = ObjectType::FF;
  ret->functions = _ff_func;
  ret->p = copy(a->p);
  ret->x = mod(a->x, b->x);
  return ret;
}

__EXPORT__ bool FF_to_raw_string(const FF *ff, char *p, int maxlen) {
  auto c = FF_to_raw_string_as_std_string(ff);
  if (c.size() < maxlen) {
    strcpy(p, c.c_str());
    return true;
  }
  return false;
}

__EXPORT__ bool FF_to_string(const FF *ff, char *p, int maxlen) {
  auto c = FF_to_string_as_std_string(ff);
  if (c.size() < maxlen) {
    strcpy(p, c.c_str());
    return true;
  }
  return false;
}

string FF_to_raw_string_as_std_string(const FF *ff) {
  stringstream ss;
  ss << "FF_Native('" << FTBL(ff->x)->to_std_string(ff->x) << "', '" << FTBL(ff->p)->to_std_string(ff->p) << "')";
  return ss.str();
}

string FF_to_string_as_std_string(const FF *ff) {
  stringstream ss;
  ss << to_std_string(ff->x);
  return ss.str();
}

__EXPORT__ bool FF_is_equals(const FF *ee, const FF *ff) {
  return equals(ee->x, ff->x) && equals(ee->p, ff->p);
}

bool FF_is_same_type(const g_object_t *ee, const g_object_t *ff) {
  return ee->type == ff->type && ff->type == ObjectType::FF
    && equals(to_FF(const_cast<g_object_t*>(ee))->p, to_FF(const_cast<g_object_t*>(ff))->p);
}

FF *FF_copy(const FF *ff) {
  auto ret = new FF;
  ret->functions = _ff_func;
  ret->type = ObjectType::FF;
  ret->x = copy(ff->x);
  ret->p = copy(ff->p);
  return ret;
}
