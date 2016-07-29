#include "ecpy_native.h"

// EF
// create EF instance
// polynomial is string of irreducible polynomial. 
// e.g. x^2+x+1, x^2+1, X^2+1, x^2+ x +1 (ignore spaces and case insensitive)
__EXPORT__ EF *EF_create(const char *p, const char *polynomial) {
  // string to polynomial
  auto poly_ = std::string(polynomial);
  IrreduciblePolynomialType pol;
  // lower case
  std::transform(poly_.begin(), poly_.end(), poly_.begin(), ::tolower);
  // remove spaces
  auto poly = std::accumulate(poly_.begin(), poly_.end(), std::string(), [](std::string a, char b) {
        if (b != ' ') {
          a += b;
        }
        return a;
      });
  if (poly == "x^2+1") {
    pol = IrreduciblePolynomialType::X2_1;
  } else if (poly == "x^2+x+1") {
    pol = IrreduciblePolynomialType::X2_X_1;
  }
  return new EF(mpz_class(p), pol);
}
// delete EF instance
__EXPORT__ void EF_delete(const EF *ef) {
  delete ef;
}

// r = a + b
__EXPORT__ void EF_add(const EF *obj, EF_elem *ret, const EF_elem *a, const EF_elem *b) {
  obj->add(*ret, *a, *b);
}

// r = a - b
__EXPORT__ void EF_sub(const EF *obj, EF_elem *ret, const EF_elem *a, const EF_elem *b)  {
  obj->sub(*ret, *a, *b);
}

// r = a * b
__EXPORT__ void EF_mul(const EF *obj, EF_elem *ret, const EF_elem *a, const EF_elem *b)  {
  obj->mul(*ret, *a, *b);
}
// r = a / b
__EXPORT__ void EF_div(const EF *obj, EF_elem *ret, const EF_elem *a, const EF_elem *b)  {
  obj->div(*ret, *a, *b);
}
// r = a ^ b
__EXPORT__ void EF_pow(const EF *obj, EF_elem *ret, const EF_elem *a, const EF_elem *b)  {
  obj->pow(*ret, *a, *b);
}

__EXPORT__ void EF_to_string(const EF *obj, char *ptr, int len) {
  write_to_python_string(obj, ptr, len);
}

// EF_elem
__EXPORT__ EF_elem *EF_elem_create(const char *u, const char *v) {
  return new EF_elem(mpz_class(u), mpz_class(v));
}

__EXPORT__ void EF_elem_delete(const EF_elem *obj) {
  delete obj;
}

__EXPORT__ void EF_elem_to_string(const EF_elem *obj, char *ptr, int len) {
  write_to_python_string(obj, ptr, len);
}

