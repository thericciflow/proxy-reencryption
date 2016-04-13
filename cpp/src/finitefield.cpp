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
