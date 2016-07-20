#pragma once
#include <array>
#include <string>
#include <random>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <gmp.h>
#include <gmpxx.h>

#define __EXPORT__ extern "C"

#define MAKE_FUNC_TABLE(var, \
    _destroy, _add, _neg, _mul, _div, _inv, \
    _mod, _equals, _is_same_type, _to_std_string, _copy) \
  const static methods_t _##var = {\
    .destroy = (fpop_t<void>) _destroy, \
    .add = (fpbinop_t<>) _add , \
    .neg = (fpop_t<>) _neg , \
    .mul = (fpbinop_t<>) _mul , \
    .div = (fpbinop_t<>) _div , \
    .inv = (fpop_t<>) _inv , \
    .mod = (fpbinop_t<>) _mod, \
    .equals = (fpbinop_t<bool>) _equals , \
    .is_same_type = (fpbinop_t<bool>) _is_same_type , \
    .to_std_string = (fpop_t<std::string>) _to_std_string ,\
    .copy = (fpop_t<>) _copy , \
  }; const static methods_t* var = &_##var

#define FTBL(obj) (((g_object_t*)obj)->functions)

#define INVOKE(instance, func, ...) FTBL(instance)->func((instance) , __VA_ARGS__)
#define INVOKESIMPLE(instance, func) FTBL(instance)->func(instance)

#define AS_OBJECT(x) reinterpret_cast<g_object_t*>(x)

struct g_object_t;
using __vp = void*;
template <class T=g_object_t*>
using fpbinop_t = T (*)(const void *, const void *);
template <class T=g_object_t*>
using fpop_t = T (*)(const void *);

enum struct ObjectType : unsigned int {
  ZZ,
  FF,
  EF,
  EC_FF,
  EC_EF,
  EP,
};

struct methods_t {
  fpop_t<void> destroy;
  fpbinop_t<> add;
  fpop_t<> neg;
  fpbinop_t<> mul;
  fpbinop_t<> div;
  fpop_t<> inv;
  fpbinop_t<> mod;
  fpbinop_t<bool> equals;
  fpbinop_t<bool> is_same_type;
  fpop_t<std::string> to_std_string;
  fpop_t<> copy;
};

struct g_object_t {
  const methods_t *functions;
  ObjectType type;
};

enum struct IrreduciblePolynomialType : unsigned int {
  X2_1,   /* x^2+1 */
  X2_X_1, /* x^2+x+1 */
};

enum struct EC_Type : unsigned int {
  EC_FF, /* on Finite Field */
  EC_EF, /* on "Extended" Finite Field */
};

struct ZZ;
struct FF;
struct EF;
struct EC;
struct EP;

struct EF {
  const methods_t *functions;
  ObjectType type;
  IrreduciblePolynomialType poly;
  ZZ *modulo;
  ZZ *x, *y;
};

struct EC {
  const methods_t *functions;
  ObjectType type;
  ZZ *a, *b;
};

struct EP {
  const methods_t *functions;
  ObjectType type;
  g_object_t *x, *y, *z;
};

__EXPORT__ {
  ZZ *ZZ_create(const char*);
  void ZZ_destroy(const ZZ*);
  ZZ *ZZ_add(const ZZ*, const ZZ*);
  ZZ *ZZ_neg(const ZZ*);
  ZZ *ZZ_mul(const ZZ*, const ZZ*);
  ZZ *ZZ_div(const ZZ*, const ZZ*);
  ZZ *ZZ_mod(const ZZ*, const ZZ*);
  ZZ *ZZ_modinv(const ZZ*, const ZZ*);
  bool ZZ_to_string(const ZZ *zz, char *p, int maxlen);
  bool ZZ_is_equals(const ZZ*, const ZZ*);
  int ZZ_jacobi(const ZZ*, const ZZ*);
  int ZZ_legendre(const ZZ*, const ZZ*);
}

ZZ *ZZ_create_from_mpz_class(mpz_class);
std::string ZZ_to_string_as_std_string(const ZZ *zz);
ZZ *ZZ_copy(ZZ*);
bool ZZ_is_same_type(const g_object_t*, const g_object_t*);

__EXPORT__ {
  FF *FF_create(const char*, const char*);
  void FF_destroy(const FF*);
  FF *FF_add(const FF*, const FF*);
  FF *FF_neg(const FF*);
  FF *FF_mul(const FF*, const FF*);
  FF *FF_div(const FF*, const FF*);
  FF *FF_mod(const FF*, const FF*);
  bool FF_is_equals(const FF*, const FF*);
  bool FF_to_string(const FF*, char*, int);
  bool FF_to_raw_string(const FF*, char*, int);
}
FF *FF_create_from_mpz_class(mpz_class, mpz_class);
std::string FF_to_string_as_std_string(const FF*);
std::string FF_to_raw_string_as_std_string(const FF*);
bool FF_is_same_type(const g_object_t*, const g_object_t*);

bool is_prime(mpz_class);

IrreduciblePolynomialType EF_Detect_Polynomial(const ZZ*);
IrreduciblePolynomialType EF_Get_Polynomial(const char*);
EF *EF_create_from_mpz_class(mpz_class, mpz_class, mpz_class, IrreduciblePolynomialType);
std::string EF_to_string_as_std_string(const EF*);

__EXPORT__ {
  EF *EF_create(const char*, const char*, const char*, const char*);
  void EF_destroy(const EF*);
  bool EF_to_string(const EF*, char*, int);
  bool EF_is_equals(const EF*, const EF*);
  EF *EF_add(const EF*, const EF*);
  EF *EF_neg(const EF*);
  EF *EF_mul(const EF*, const EF*);
  EF *EF_inv(const EF*);
}
