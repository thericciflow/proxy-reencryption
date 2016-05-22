#include <ecpy.h>

FiniteField::FiniteField(const mpz_class& _p) : p(_p) {
  if (!is_prime(p)) {
    throw "Exception: Invalid p";
  }
};

std::ostream& operator<<(std::ostream& os, const FiniteField& field) {
  os << "FiniteField: p = " << field.p;
  return os;
}

FiniteFieldElement::FiniteFieldElement(FiniteField *_f, mpz_class _x) : f(_f) {
  x = _x % f->p;
  if (x < 0) {
    x += f->p;
  }
}

std::ostream& operator<<(std::ostream& os, const FiniteFieldElement& x) {
  os << x.x;
  return os;
}
