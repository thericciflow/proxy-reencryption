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

FiniteFieldElement FiniteFieldElement::operator+(const FiniteFieldElement& rhs) {
  return FiniteFieldElement(f, (mpz_class)(x + rhs.x));
}

FiniteFieldElement FiniteFieldElement::operator-(const FiniteFieldElement& rhs) {
  return FiniteFieldElement(f, (mpz_class)(x - rhs.x));
}

FiniteFieldElement FiniteFieldElement::operator*(const FiniteFieldElement& rhs) {
  return FiniteFieldElement(f, (mpz_class)(x * rhs.x));
}

FiniteFieldElement FiniteFieldElement::operator/(const FiniteFieldElement& rhs) {
  mpz_class t;
  mpz_invert(t.get_mpz_t(), rhs.x.get_mpz_t(), f->p.get_mpz_t());
  return FiniteFieldElement(f, x * t);
}

template <>
FiniteFieldElement FiniteField::operator()<mpz_class>(mpz_class x) {
  return FiniteFieldElement(this, x);
}

ostream& operator<<(ostream& os, const FiniteField& field) {
  os << "FiniteField: p = " << field.p;
  return os;
}

ostream& operator<<(ostream& os, const FiniteFieldElement& elem) {
  char *buf = nullptr;
  gmp_asprintf(&buf, "%Zd", elem.x.get_mpz_t());
  os << buf;
  delete buf;
  return os;
}
