#include "ecpy_native.h"

using namespace std;

ZZ powermod(ZZ base, ZZ exp, ZZ mod) {
  mpz_class t;
  mpz_powm(MPZ_T(t), MPZ_T(base.to_mpz_class()), MPZ_T(exp.to_mpz_class()), MPZ_T(mod.to_mpz_class()));
  return ZZ{t};
}

ZZ power(ZZ base, ZZ _exp) {
  mpz_class t;
  unsigned long int exp = _exp.to_mpz_class().get_ui();
  mpz_pow_ui(MPZ_T(t), MPZ_T(base.to_mpz_class()), exp);
  return ZZ{t};
}

ZZ modinv(ZZ _a, ZZ _modulo) {
  auto a = _a.to_mpz_class();
  auto modulo = _modulo.to_mpz_class();
  if (a < 0) {
    a = a + modulo;
  }
  mpz_class t;
  mpz_invert(MPZ_T(t), MPZ_T(a), MPZ_T(modulo));
  return ZZ{t};
}
