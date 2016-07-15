#pragma once
#include <array>
#include <string>
#include <random>
#include <vector>
#include <sstream>
#include <iostream>

#include <gmp.h>
#include <gmpxx.h>

#define __EXPORT__ extern "C"

struct ZZ {
  mpz_class x;
};

struct FF {
  ZZ *x;
  ZZ *p;
};

ZZ *ZZ_create_from_mpz_class(mpz_class);
__EXPORT__ ZZ *ZZ_create(const char*);
__EXPORT__ void ZZ_destroy(ZZ*);
__EXPORT__ ZZ *ZZ_add(ZZ*, ZZ*);
__EXPORT__ ZZ *ZZ_neg(ZZ*);
__EXPORT__ ZZ *ZZ_mul(ZZ*, ZZ*);
__EXPORT__ ZZ *ZZ_div(ZZ*, ZZ*);
__EXPORT__ ZZ *ZZ_mod(ZZ*, ZZ*);
__EXPORT__ ZZ *ZZ_modinv(ZZ*, ZZ*);
__EXPORT__ const char *ZZ_to_string(ZZ*);
__EXPORT__ bool ZZ_is_equals(ZZ*, ZZ*);
ZZ *ZZ_copy(ZZ*);

__EXPORT__ FF *FF_create_from_mpz_class(mpz_class, mpz_class);
__EXPORT__ FF *FF_create(const char*, const char*);
__EXPORT__ void FF_destroy(FF*);
__EXPORT__ FF *FF_add(FF*, FF*);
__EXPORT__ FF *FF_neg(FF*);
__EXPORT__ FF *FF_mul(FF*, FF*);
__EXPORT__ FF *FF_div(FF*, FF*);
__EXPORT__ FF *FF_mod(FF*, FF*);
__EXPORT__ const char *FF_to_string(FF*);
__EXPORT__ bool FF_is_equals(FF*, FF*);
