#pragma once

#include <gmp.h>
#include <gmpxx.h>
#include <iostream>
#include <fstream>
#include <random>
#include <string>

#define MPZ_T(x) (x).get_mpz_t()

extern bool is_prime(mpz_class);

#include "field.h"
#include "finitefield.h"
#include "ec.h"
