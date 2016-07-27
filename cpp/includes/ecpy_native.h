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

struct FF_elem {
  mpz_class v;
  FF_elem(const mpz_class& v) : v(v) {}
  FF_elem(const FF_elem& ffe) : v(ffe.v) {}
  FF_elem *clone();
};

class FF {
  mpz_class p;
  inline mpz_class mod(mpz_class v) {
    mpz_class t = v % p;
    if (t < 0) {
      t += p;
    }
    return t;
  }
  public:
  FF(const mpz_class& p) : p(p) {}
  FF_elem *add(const FF_elem *x, const FF_elem *y);
  FF_elem *sub(const FF_elem *x, const FF_elem *y);
  FF_elem *mul(const FF_elem *x, const FF_elem *y);
  FF_elem *div(const FF_elem *x, const FF_elem *y);
  FF_elem *pow(const FF_elem *x, const FF_elem *y);
};
