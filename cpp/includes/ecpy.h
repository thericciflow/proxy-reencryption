#pragma once

#include <gmp.h>
#include <gmpxx.h>
#include <iostream>
#include <fstream>
#include <random>
#include <string>

#define MPZ_T(x) (x).get_mpz_t()

extern bool is_prime(const mpz_class&);

template <class T>
inline mpz_class to_mpz_cls(const T& t) {
  return t;
}

#include "field.h"
#include "finitefield.h"
#include "complexfield.h"
#include "ec.h"
#include "pairing.h"
