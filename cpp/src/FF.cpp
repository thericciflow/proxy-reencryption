#include "ecpy_native.h"

using namespace std;

FF_elem *FF_elem::clone() {
  return new FF_elem(v);
}

FF_elem *FF::add(const FF_elem *x, const FF_elem *y) {
  FF_elem z(mod(x->v + y->v));
  return z.clone();
}

FF_elem *FF::sub(const FF_elem *x, const FF_elem *y) {
  FF_elem z(mod(x->v - y->v));
  return z.clone();
}

FF_elem *FF::mul(const FF_elem *x, const FF_elem *y) {
  FF_elem z(mod(x->v * y->v));
  return z.clone();
}

FF_elem *FF::div(const FF_elem *x, const FF_elem *y) {
  mpz_class t;
  mpz_invert(t.get_mpz_t(), y->v.get_mpz_t(), p.get_mpz_t());
  FF_elem z(mod(x->v * t));
  return z.clone();
}
FF_elem *FF::pow(const FF_elem *x, const FF_elem *y) {
  mpz_class t;
  mpz_powm(t.get_mpz_t(), x->v.get_mpz_t(), y->v.get_mpz_t(), p.get_mpz_t());
  FF_elem z(t);
  return z.clone();
}
