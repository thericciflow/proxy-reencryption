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

#include "FF.h"

#include "EF.h"

#include "EC.h"

template <class T>
void write_to_python_string(const T *x, char *ptr, int len) {
  std::stringstream ss;
  ss << x->to_string();
  std::string r = ss.str();
  if (r.size() < static_cast<unsigned int>(len)) {
    strcpy(ptr, r.c_str());
  }
}

template <class T, class E>
void h(E& ret, const EC<T> curve, const EC_elem<E> P, const EC_elem<E> Q, const EC_elem<E> R) {
  E u {0}, v {0};
  E p {0}, q {0};
  curve.base.mul(u, P.x, Q.z);
  curve.base.mul(v, P.z, Q.x);
  if ((curve.equ(P, Q) && curve.base.equ(P.y, E {0})) || (!curve.equ(P, Q) && curve.base.equ(u, v))) {
      curve.base.mul(u, R.x, P.z);
      curve.base.mul(v, R.z, P.x);
      curve.base.sub(u, u, v);     // R.x * P.z - R.z * P.x
      curve.base.mul(v, P.z, R.z);
      curve.base.div(ret, u, v);   // (R.x * P.z - R.z * P.x) / (P.z * R.z)
      return;
  }
  E L = curve.line_coeff(P, Q);

  curve.base.mul(p, R.y, P.z);
  curve.base.mul(u, P.y, R.z);
  curve.base.sub(p, p, u);     // R.y * P.z - P.y * R.z
  curve.base.mul(u, R.x, P.z);
  curve.base.mul(v, P.x, R.z);
  curve.base.sub(u, u, v);     // R.x * P.z - P.x * R.z
  curve.base.mul(u, u, L);     // L * (R.x * P.z - P.x * R.z)
  curve.base.sub(p, p, u);     // R.y * P.z - P.y * R.z - L * (R.x * P.z - P.x * R.z)


  curve.base.mul(q, P.x, Q.z);
  curve.base.mul(q, q, R.z);

  curve.base.mul(u, Q.x, P.z);
  curve.base.mul(u, u, R.z);
  curve.base.add(q, q, u);     // P.x * Q.z * R.z + P.z * Q.x * R.z

  curve.base.mul(u, P.z, Q.z);
  curve.base.mul(u, u, R.x);
  curve.base.add(q, q, u);     // P.x * Q.z * R.z + P.z * Q.x * R.z + P.z * Q.z * R.x

  curve.base.mul(u, L, L);
  curve.base.mul(u, u, P.z);
  curve.base.mul(u, u, Q.z);
  curve.base.mul(u, u, R.z);
  curve.base.sub(q, q, u);     // P.x * Q.z * R.z + P.z * Q.x * R.z + P.z * Q.z * R.x - (L^2 * P.z * Q.z * R.z)


  curve.base.div(ret, p, q);
  curve.base.mul(ret, ret, Q.z);
}

template <class T, class E>
void miller(E& ret, const EC<T> curve, const EC_elem<E> P, const EC_elem<E> Q, const mpz_class& m) {
  E t {0};

  ret = E {1};

  if (curve.equ(P, Q)) {
    return;
  }
  unsigned int n = 0;
  for (;(m>>n) != 0; n++);
  EC_elem<E> G {P};
  for (int i = n - 2; i >= 0; i--) {
    curve.base.mul(ret, ret, ret);
    h(t, curve, G, G, Q);
    curve.base.mul(ret, ret, t);
    curve.add(G, G, G);
    if (((m >> i) & 1) == 1) {
      h(t, curve, G, P, Q);
      curve.base.mul(ret, ret, t);
      curve.add(G, G, P);
    }
  }
}
