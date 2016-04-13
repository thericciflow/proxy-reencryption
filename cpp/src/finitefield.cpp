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

bool FiniteFieldElement::operator==(const FiniteFieldElement& rhs) const {
  return x == rhs.x;
}

ostream& operator<<(ostream& os, const FiniteFieldElement& elem) {
  char *buf = nullptr;
  gmp_asprintf(&buf, "%Zd", MPZ_T(elem.x));
  os << buf;
  return os;
}
/*
FiniteFieldElement FiniteFieldElement::operator+(const FiniteFieldElement& rhs) const {
  FiniteFieldElement z;
  f->add(z, *this, rhs);
  return z;
}

FiniteFieldElement FiniteFieldElement::operator-(const FiniteFieldElement& rhs) const {
  FiniteFieldElement z;
  f->sub(z, *this, rhs);
  return z;
}

FiniteFieldElement FiniteFieldElement::operator*(const FiniteFieldElement& rhs) const {
  FiniteFieldElement z;
  f->mul(z, *this, rhs);
  return z;
}

FiniteFieldElement FiniteFieldElement::operator/(const FiniteFieldElement& rhs) const {
  FiniteFieldElement z;
  f->div(z, *this, rhs);
  return z;
}*/
/*
template <class T>
FiniteFieldElement FiniteFieldElement::operator+(const T& rhs) const {
  FiniteFieldElement z;
  f->add(z, *this, rhs);
  return z;
}

template <class T>
FiniteFieldElement FiniteFieldElement::operator-(const T& rhs) const {
  FiniteFieldElement z;
  f->sub(z, *this, rhs);
  return z;
}

template <class T>
FiniteFieldElement FiniteFieldElement::operator*(const T& rhs) const {
  FiniteFieldElement z;
  f->mul(z, *this, rhs);
  return z;
}

template <class T>
FiniteFieldElement FiniteFieldElement::operator/(const T& rhs) const {
  FiniteFieldElement z;
  f->div(z, *this, rhs);
  return z;
}

template <class T>
FiniteFieldElement operator+(const T& lhs, const FiniteFieldElement& rhs) {
  FiniteFieldElement z;
  rhs.f->add(z, lhs, rhs);
  return z;
}

template <class T>
FiniteFieldElement operator-(const T& lhs, const FiniteFieldElement& rhs) {
  FiniteFieldElement z;
  rhs.f->sub(z, lhs, rhs);
  return z;
}

template <class T>
FiniteFieldElement operator*(const T& lhs, const FiniteFieldElement& rhs) {
  FiniteFieldElement z;
  rhs.f->mul(z, lhs, rhs);
  return z;
}

template <class T>
FiniteFieldElement operator/(const T& lhs, const FiniteFieldElement& rhs) {
  FiniteFieldElement z;
  rhs.f->div(z, lhs, rhs);
  return z;
}
*/
