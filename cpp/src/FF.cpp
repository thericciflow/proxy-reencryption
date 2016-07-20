#include "ecpy_native.h"
#include "FF_impl.h"

using namespace std;

MAKE_FUNC_TABLE(_ff_func, FF_destroy, FF_add, FF_neg, FF_mul, FF_div, nullptr, FF_mod, FF_is_equals, FF_is_same_type, FF_to_string_as_std_string, nullptr);

FF *FF_create_from_mpz_class(mpz_class x, mpz_class p) {
  FF *ff = new FF;
  ff->p = AS_OBJECT(ZZ_create_from_mpz_class(p));
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
  ff->x = INVOKE(t, mod, ff->p);
  INVOKESIMPLE(t, destroy);
  return ff;
}

__EXPORT__ void FF_destroy(const FF *ff) {
  INVOKESIMPLE(ff->x, destroy);
  INVOKESIMPLE(ff->p, destroy);
  delete ff;
}

__EXPORT__ FF *FF_add(const FF *a, const FF *b) {
  FF *ret = new FF;
  ret->type = ObjectType::FF;
  ret->functions = _ff_func;
  ret->p = INVOKESIMPLE(a->p, copy);
  if (INVOKE(a, is_same_type, b)) {
    auto t = INVOKE(a->x, add, b->x);
    ret->x = INVOKE(t, mod, a->p);
    INVOKESIMPLE(t, destroy);
  } else {
    ret->x = AS_OBJECT(ZZ_create("-1"));
    ret->p = AS_OBJECT(ZZ_create("-1"));
  }
  return ret;
}

__EXPORT__ FF *FF_neg(const FF *a) {
  FF *ret = new FF;
  ret->type = ObjectType::FF;
  ret->functions = _ff_func;
  auto t = INVOKESIMPLE(a->x, neg);
  ret->x = INVOKE(t, add, a->p);
  ret->p = INVOKESIMPLE(a->p, copy);
  INVOKESIMPLE(t, destroy);
  return ret;
}

__EXPORT__ FF *FF_mul(const FF *a, const FF *b) {
  FF *ret = new FF;
  ret->type = ObjectType::FF;
  ret->functions = _ff_func;
  ret->p = INVOKESIMPLE(a->p, copy);
  if (INVOKE(a->p, equals, b->p)) {
    auto t = INVOKE(a->x, mul, b->x);
    ret->x = INVOKE(t, mod, a->p);
    INVOKESIMPLE(t, destroy);
  } else {
    ret->x = AS_OBJECT(ZZ_create_from_mpz_class(-1));
    ret->p = AS_OBJECT(ZZ_create_from_mpz_class(-1));
  }
  return ret;
}

__EXPORT__ FF *FF_div(const FF *a, const FF *b) {
  FF *ret = new FF;
  ret->type = ObjectType::FF;
  ret->functions = _ff_func;
  ret->p = INVOKESIMPLE(a->p, copy);
  if (INVOKE(a->p, is_same_type, b->p) && b->x->type == ObjectType::ZZ && a->p->type == ObjectType::ZZ) {
    auto t = ZZ_modinv((ZZ*)b->x, (ZZ*)a->p);
    auto u = INVOKE(a->x, mul, t);
    ret->x = INVOKE(u, mod, a->p);
    INVOKESIMPLE(t, destroy);
    INVOKESIMPLE(u, destroy);
  } else {
    ret->x = AS_OBJECT(ZZ_create_from_mpz_class(-1));
    ret->p = AS_OBJECT(ZZ_create_from_mpz_class(-1));
  }
  return ret;
}

__EXPORT__ FF *FF_mod(const FF *a, const FF *b) {
  FF *ret = new FF;
  ret->type = ObjectType::FF;
  ret->functions = _ff_func;
  ret->p = INVOKESIMPLE(a->p, copy);
  ret->x = INVOKE(a->x, mod, b->x);
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
  ss << INVOKESIMPLE(ff->x, to_std_string) << " modulo " << INVOKESIMPLE(ff->p, to_std_string);
  return ss.str();
}

__EXPORT__ bool FF_is_equals(const FF *ee, const FF *ff) {
  return INVOKE(ee->x, equals, ff->x) && INVOKE(ee->p, equals, ff->p);
}

bool FF_is_same_type(const g_object_t *ee, const g_object_t *ff) {
  return ee->type == ff->type && ff->type == ObjectType::FF
    && INVOKE(((FF*)ff)->p, equals, ((FF*)ee)->p);
}
