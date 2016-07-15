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

ZZ *ZZ_create_from_mpz_class(mpz_class);
__EXPORT__ ZZ *ZZ_create(const char *);
__EXPORT__ void ZZ_destroy(ZZ *);
__EXPORT__ ZZ *ZZ_add(ZZ *, ZZ *);
__EXPORT__ ZZ *ZZ_neg(ZZ *);
__EXPORT__ ZZ *ZZ_mul(ZZ *, ZZ *);
__EXPORT__ ZZ *ZZ_div(ZZ *, ZZ *);
__EXPORT__ ZZ *ZZ_mod(ZZ *, ZZ *);
__EXPORT__ ZZ *ZZ_modinv(ZZ *, ZZ *);
__EXPORT__ const char *ZZ_to_string(ZZ *);
