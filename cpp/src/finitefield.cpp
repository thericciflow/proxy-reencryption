#include <ecpy.h>
using namespace std;

FiniteField::FiniteField(mpz_class _p) : p(_p) {
  if (!is_prime(p)) {
    throw "Exception: Invalid p";
  }
};

FiniteFieldElement::FiniteFieldElement(FiniteField *_f, mpz_class _x) : f(_f) {
  x = _x % f->p;
  if (x < 0) {
    x += f->p;
  }
}

template <class T>
FiniteFieldElement FiniteFieldElement::operator-(T rhs) {
  return FiniteFieldElement(f, x - rhs);
}
template <class T>
FiniteFieldElement FiniteFieldElement::operator*(T rhs) {
  return FiniteFieldElement(f, x * rhs);
}
template <class T>
FiniteFieldElement FiniteFieldElement::operator/(T rhs) {
  mpz_class t;
  mpz_invert(MPZ_T(t), MPZ_T(rhs), MPZ_T(f->p));
  return FiniteFieldElement(f, x * t);
}
FiniteFieldElement FiniteFieldElement::operator+(const FiniteFieldElement& rhs) {
  return FiniteFieldElement(f, x + rhs.x);
}

FiniteFieldElement FiniteFieldElement::operator-(const FiniteFieldElement& rhs) {
  return FiniteFieldElement(f, x - rhs.x);
}

FiniteFieldElement FiniteFieldElement::operator*(const FiniteFieldElement& rhs) {
  return FiniteFieldElement(f, x * rhs.x);
}

FiniteFieldElement FiniteFieldElement::operator/(const FiniteFieldElement& rhs) {
  mpz_class t;
  mpz_invert(MPZ_T(t), MPZ_T(rhs.x), MPZ_T(f->p));
  return FiniteFieldElement(f, x * t);
}

bool FiniteFieldElement::operator==(const FiniteFieldElement& rhs) {
  return x == rhs.x;
}

ostream& operator<<(ostream& os, const FiniteField& field) {
  os << "FiniteField: p = " << field.p;
  return os;
}

ostream& operator<<(ostream& os, const FiniteFieldElement& elem) {
  char *buf = nullptr;
  gmp_asprintf(&buf, "%Zd", MPZ_T(elem.x));
  os << buf;
  return os;
}
