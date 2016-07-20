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

enum struct IrreduciblePolynomialType : unsigned int {
  X2_1,   /* x^2+1 */
  X2_X_1, /* x^2+x+1 */
};

struct ZZ;
struct FF;
struct EF;

#include "objects.h"

ZZ *ZZ_create_from_mpz_class(mpz_class);
std::string ZZ_to_string_as_std_string(const ZZ *zz);
ZZ *ZZ_copy(ZZ*);
bool ZZ_is_same_type(const g_object_t*, const g_object_t*);

FF *FF_create_from_mpz_class(mpz_class, mpz_class);
std::string FF_to_string_as_std_string(const FF*);
std::string FF_to_raw_string_as_std_string(const FF*);
bool FF_is_same_type(const g_object_t*, const g_object_t*);

bool is_prime(mpz_class);

IrreduciblePolynomialType EF_Detect_Polynomial(const ZZ*);
IrreduciblePolynomialType EF_Get_Polynomial(const char*);
EF *EF_create_from_mpz_class(mpz_class, mpz_class, mpz_class, IrreduciblePolynomialType);
std::string EF_to_string_as_std_string(const EF*);

#include "exports.h"
