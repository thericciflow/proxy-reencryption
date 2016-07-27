#include "ecpy_native.h"

using namespace std;
FF_elem *FF_create_elem(const mpz_class& v) {
  FF_elem t(v);
  return t.clone();
}

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

FF *FF::clone() {
  return new FF(p);
}

__EXPORT__ FF *FF_create(const char *p) {
  return FF(mpz_class(p)).clone();
}

__EXPORT__ FF_elem *FF_create_elem(const char *v) {
  return FF_create_elem(mpz_class(v));
}

__EXPORT__ FF_elem *FF_add(FF *ff, const FF_elem *x, const FF_elem *y) {
  return ff->add(x, y);
}

__EXPORT__ FF_elem *FF_sub(FF *ff, const FF_elem *x, const FF_elem *y) {
  return ff->sub(x, y);
}

__EXPORT__ FF_elem *FF_mul(FF *ff, const FF_elem *x, const FF_elem *y) {
  return ff->mul(x, y);
}

__EXPORT__ FF_elem *FF_div(FF *ff, const FF_elem *x, const FF_elem *y) {
  return ff->div(x, y);
}

__EXPORT__ FF_elem *FF_pow(FF *ff, const FF_elem *x, const FF_elem *y) {
  return ff->pow(x, y);
}
