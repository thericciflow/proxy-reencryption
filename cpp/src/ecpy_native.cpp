#include "ecpy_native.h"

using namespace std;

template<>
mpz_class get_modulus(FF base) {
  return base.p;
}

template<>
mpz_class get_modulus(EF base) {
  return base.base.p * base.base.p;
}

