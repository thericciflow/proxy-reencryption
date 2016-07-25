#pragma once
#include <array>
#include <string>
#include <random>
#include <vector>
#include <sstream>
#include <numeric>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include <gmp.h>
#include <gmpxx.h>

#define __EXPORT__ extern "C"

enum struct IrreduciblePolynomialType : unsigned int {
  X2_1,   /* x^2+1 */
  X2_X_1, /* x^2+x+1 */
};

enum struct EC_Type {
  FF, /* on Finite field */
  EF, /* on Extended Finite Field */
};

struct ZZ;
struct FF;
struct EF;
struct EC;
struct EP;

#include "objects.h"

ZZ *ZZ_create_from_mpz_class(mpz_class);
std::string ZZ_to_string_as_std_string(const ZZ *zz);
ZZ *ZZ_copy(ZZ*);

FF *FF_create_from_mpz_class(mpz_class, mpz_class);
std::string FF_to_string_as_std_string(const FF*);
std::string FF_to_raw_string_as_std_string(const FF*);
FF *FF_copy(const FF*);

bool is_prime(mpz_class);

IrreduciblePolynomialType EF_Detect_Polynomial(const ZZ*);
IrreduciblePolynomialType EF_Get_Polynomial(const char*);
EF *EF_create_from_mpz_class(mpz_class, mpz_class, mpz_class, IrreduciblePolynomialType);
std::string EF_to_string_as_std_string(const EF*);
std::string EF_to_tuple_std_string(const EF*);
EF *EF_copy(const EF*);

std::string EC_to_std_string(const EC*);

std::string EP_to_tuple_std_string(const EP*);
std::string EP_to_std_string(const EP*);
EP *EP_copy(const EP*);

bool ZZ_is_same_type(const g_object_t*, const g_object_t*);
bool FF_is_same_type(const g_object_t*, const g_object_t*);
bool EF_is_same_type(const g_object_t*, const g_object_t*);
bool EC_is_same_type(const g_object_t*, const g_object_t*);
bool EP_is_same_type(const g_object_t*, const g_object_t*);

#include "exports.h"
