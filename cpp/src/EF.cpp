#include "ecpy_native.h"

using namespace std;

IrreduciblePolynomialType EF_Detect_Polynomial(const ZZ *n) {
  if (n->x % 4 == 3) {
    return IrreduciblePolynomialType::X2_1;
  } else if (n->x % 3 == 2 && n->x % 6 == 5 && is_prime((n->x + 1) / 6)) {
    return IrreduciblePolynomialType::X2_X_1;
  }
  throw invalid_argument("cannot determine irreducible polynomial");
}

IrreduciblePolynomialType EF_Get_Polynomial(const char *poly_str) {
  auto poly_ = string(poly_str);
  transform(poly_.begin(), poly_.end(), poly_.begin(), ::tolower);
  auto poly = accumulate(poly_.begin(), poly_.end(), string(), [](string a, char b) {
        if (b != ' ') {
          a += b;
        }
        return a;
      });
  if (poly == "x^2+1") {
    return IrreduciblePolynomialType::X2_1;
  } else if (poly == "x^2+x+1") {
    return IrreduciblePolynomialType::X2_X_1;
  }
  throw invalid_argument("invalid irreducible polynomial");
}

EF *EF_create_from_mpz_class(mpz_class x, mpz_class y, mpz_class n, IrreduciblePolynomialType type) {
  EF *ef = new EF;
  ef->modulo = ZZ_create_from_mpz_class(n);
  ef->poly = type;
  ef->x = ZZ_create_from_mpz_class(x % n);
  ef->y = ZZ_create_from_mpz_class(y % n);
  return ef;
}

__EXPORT__ EF *EF_create(const char *x, const char *y, const char *n, const char *poly) {
  EF *ef = new EF;
  ef->modulo = ZZ_create(n);
  {
    auto t = ZZ_create(x);
    ef->x = ZZ_mod(t, ef->modulo);
    ZZ_destroy(t);
  }
  {
    auto t = ZZ_create(y);
    ef->y = ZZ_mod(t, ef->modulo);
    ZZ_destroy(t);
  }
  ef->poly = EF_Get_Polynomial(poly);
  return ef;
}

__EXPORT__ void EF_destroy(const EF *ef) {
  ZZ_destroy(ef->modulo);
  ZZ_destroy(ef->x);
  ZZ_destroy(ef->y);
  delete ef;
}

