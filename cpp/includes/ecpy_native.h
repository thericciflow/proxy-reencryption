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
  std::string to_string() const;
  FF_elem *clone() const;
};

class FF {
  mpz_class p;
  inline mpz_class mod(const mpz_class& v) const {
    mpz_class t = v % p;
    if (t < 0) {
      t += p;
    }
    return t;
  }
  public:
  FF(const mpz_class& p) : p(p) {}
  FF(const FF& ff) : p(ff.p) {}
  FF_elem *add(const FF_elem *x, const FF_elem *y) const;
  FF_elem *sub(const FF_elem *x, const FF_elem *y) const;
  FF_elem *mul(const FF_elem *x, const FF_elem *y) const;
  FF_elem *div(const FF_elem *x, const FF_elem *y) const;
  FF_elem *pow(const FF_elem *x, const FF_elem *y) const;
  std::string to_string() const;
  FF *clone() const;
};

FF_elem *FF_create_elem(const mpz_class& v);

__EXPORT__ {
  FF *FF_create(const char*);
  FF_elem *FF_elem_create(const char*);
  FF_elem *FF_add(FF*, const FF_elem*, const FF_elem*);
  FF_elem *FF_sub(FF*, const FF_elem*, const FF_elem*);
  FF_elem *FF_mul(FF*, const FF_elem*, const FF_elem*);
  FF_elem *FF_div(FF*, const FF_elem*, const FF_elem*);
  FF_elem *FF_pow(FF*, const FF_elem*, const FF_elem*);
  void FF_delete(const FF*);
  void FF_elem_delete(const FF_elem*);
  void FF_to_string(const FF*, char*, int);
  void FF_elem_to_string(const FF_elem*, char *, int);
};

template <class T>
void write_to_python_string(const T *x, char *ptr, int len) {
  stringstream ss;
  ss << x->to_string();
  string r = ss.str();
  if (r.size() < len) {
    strcpy(ptr, r.c_str());
  }
}
