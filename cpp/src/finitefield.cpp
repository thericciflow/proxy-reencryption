#include <ecpy.h>
using namespace std;

FiniteField::FiniteField(mpz_class _p) : p(_p) {
  if (!is_prime(p)) {
    throw "Exception: Invalid p";
  }
};

ostream& operator<<(ostream& os, const FiniteField& field) {
  os << "FiniteField: p = " << field.p;
  return os;
}

FiniteFieldElement::FiniteFieldElement(FiniteField *_f, mpz_class _x) : f(_f) {
  x = _x % f->p;
  if (x < 0) {
    x += f->p;
  }
}

FiniteFieldElement FiniteFieldElement::operator+(const FiniteFieldElement& rhs) const {
  return FiniteFieldElement(f, x + rhs.x);
}

FiniteFieldElement FiniteFieldElement::operator-(const FiniteFieldElement& rhs) const {
  return FiniteFieldElement(f, x - rhs.x);
}

FiniteFieldElement FiniteFieldElement::operator*(const FiniteFieldElement& rhs) const {
  return FiniteFieldElement(f, x * rhs.x);
}

FiniteFieldElement FiniteFieldElement::operator/(const FiniteFieldElement& rhs) const {
  mpz_class t;
  mpz_invert(MPZ_T(t), MPZ_T(rhs.x), MPZ_T(f->p));
  return FiniteFieldElement(f, x * t);
}

bool FiniteFieldElement::operator==(const FiniteFieldElement& rhs) const {
  return x == rhs.x;
}

ostream& operator<<(ostream& os, const FiniteFieldElement& elem) {
  char *buf = nullptr;
  gmp_asprintf(&buf, "%Zd", MPZ_T(elem.x));
  os << buf;
  return os;
}
