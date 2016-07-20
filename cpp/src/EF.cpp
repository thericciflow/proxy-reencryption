#include "ecpy_native.h"

using namespace std;

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

__EXPORT__ EF *EF_add(const EF *a, const EF *b) {
  EF *ret = new EF;
  if (a->poly == b->poly && ZZ_is_equals(a->modulo, b->modulo)) {
    switch (a->poly) { // addition is same operation
    case IrreduciblePolynomialType::X2_1:
    case IrreduciblePolynomialType::X2_X_1:
      {
        auto t = ZZ_add(a->x, b->x);
        ret->x = ZZ_mod(t, a->modulo);
        ZZ_destroy(t);
      }
      {
        auto t = ZZ_add(a->y, b->y);
        ret->y = ZZ_mod(t, a->modulo);
        ZZ_destroy(t);
      }
    }
    ret->modulo = ZZ_copy(a->modulo);
    ret->poly = a->poly;
  } else {
    ret->x = ZZ_create_from_mpz_class(-1);
    ret->y = ZZ_create_from_mpz_class(-1);
    ret->modulo = ZZ_create_from_mpz_class(-1);
    ret->poly = IrreduciblePolynomialType::X2_1;
  }
  return ret;
}

__EXPORT__ EF *EF_neg(const EF *a) {
  EF *ret = new EF;
  switch (a->poly) {
  case IrreduciblePolynomialType::X2_1:
  case IrreduciblePolynomialType::X2_X_1:
    {
      auto t = ZZ_neg(a->x);
      ret->x = ZZ_mod(t, a->modulo);
      ZZ_destroy(t);
    }
    {
      auto t = ZZ_neg(a->y);
      ret->y = ZZ_mod(t, a->modulo);
      ZZ_destroy(t);
    }
  }
  ret->modulo = ZZ_copy(a->modulo);
  ret->poly = a->poly;
  return ret;
}

__EXPORT__ EF *EF_mul(const EF *a, const EF *b) {
  EF *ret = new EF;
    auto i = a->x;
    auto j = a->y;
    auto k = b->x;
    auto l = b->y;
  if (a->poly == b->poly && ZZ_is_equals(a->modulo, b->modulo)) {
    switch (a->poly) {
    case IrreduciblePolynomialType::X2_1:
      {
        auto t = ZZ_mul(i, k); // ac
        auto u = ZZ_mul(j, l); // bd
        auto v = ZZ_neg(u);    // -bd
        auto w = ZZ_add(t, v); // ac-bd
        ret->x = ZZ_mod(w, a->modulo);
        ZZ_destroy(t);
        ZZ_destroy(u);
        ZZ_destroy(v);
        ZZ_destroy(w);
      }
      {
        auto t = ZZ_mul(i, l); // ad
        auto u = ZZ_mul(j, k); // bc
        auto v = ZZ_add(t, u); // ad+bc
        ret->y = ZZ_mod(v, a->modulo);
        ZZ_destroy(t);
        ZZ_destroy(u);
        ZZ_destroy(v);
      }
      break;
    case IrreduciblePolynomialType::X2_X_1:
      {
        auto t = ZZ_mul(i, k); // ac
        auto u = ZZ_mul(j, l); // bd
        auto v = ZZ_neg(u);    // -bd
        auto w = ZZ_add(t, v); // ac-bd
        ret->x = ZZ_mod(w, a->modulo);
        ZZ_destroy(t);
        ZZ_destroy(u);
        ZZ_destroy(v);
        ZZ_destroy(w);
      }
      {
        auto t = ZZ_mul(i, l); // ad
        auto u = ZZ_mul(j, k); // bc
        auto v = ZZ_add(t, u); // ad+bc
        auto w = ZZ_mul(j, l); // bd
        auto x = ZZ_neg(w);    // -bd
        auto y = ZZ_add(v, x); // ad+bc-bd
        ret->y = ZZ_mod(y, a->modulo);
        ZZ_destroy(t);
        ZZ_destroy(u);
        ZZ_destroy(v);
        ZZ_destroy(w);
        ZZ_destroy(x);
        ZZ_destroy(y);
      }
      break;
    }
    ret->modulo = ZZ_copy(a->modulo);
    ret->poly = a->poly;
  } else {
    ret->x = ZZ_create_from_mpz_class(-1);
    ret->y = ZZ_create_from_mpz_class(-1);
    ret->modulo = ZZ_create_from_mpz_class(-1);
    ret->poly = IrreduciblePolynomialType::X2_1;
  }
  return ret;
}

__EXPORT__ EF *EF_inv(const EF *x) {
  EF *ret = new EF;
  auto a = x->x;
  auto b = x->y;
  auto r = ZZ_mul(a, a); // a^2
  auto s = ZZ_mul(b, b); // b^2
  auto t = ZZ_add(r, s); // a^2+b^2
  ZZ_destroy(r);
  ZZ_destroy(s);
  switch (x->poly) {
  case IrreduciblePolynomialType::X2_1:
    {
      auto u = ZZ_modinv(t, x->modulo);
      {
        auto z = ZZ_mul(a, u);
        ret->x = ZZ_mod(z, x->modulo);
        ZZ_destroy(z);
      }
      {
        auto s = ZZ_mul(b, u);
        auto z = ZZ_neg(s);
        ret->y = ZZ_mod(z, x->modulo);
        ZZ_destroy(z);
        ZZ_destroy(s);
      }
      ZZ_destroy(u);
    }
    break;
  case IrreduciblePolynomialType::X2_X_1:
    {
      auto f = ZZ_mul(a, b); // ab
      auto g = ZZ_neg(f);    // -ab
      auto h = ZZ_add(t, g); // a^2-ab+b^2
      ZZ_destroy(f);
      ZZ_destroy(g);
      auto u = ZZ_modinv(h, x->modulo);
      ZZ_destroy(h);
      {
        auto g = ZZ_neg(b);
        auto h = ZZ_add(g, a);
        auto z = ZZ_mul(h, u);
        ret->x = ZZ_mod(z, x->modulo);
        ZZ_destroy(g);
        ZZ_destroy(h);
        ZZ_destroy(z);
      }
      {
        auto s = ZZ_mul(b, u);
        auto z = ZZ_neg(s);
        ret->y = ZZ_mod(z, x->modulo);
        ZZ_destroy(s);
        ZZ_destroy(z);
      }
      ZZ_destroy(u);
    }
    break;
  }
  ZZ_destroy(t);
  ret->modulo = ZZ_copy(x->modulo);
  ret->poly = x->poly;
  return ret;
}

__EXPORT__ bool EF_is_equals(const EF *a, const EF *b) {
  return (a->poly == b->poly) &&
    ZZ_is_equals(a->modulo, b->modulo) &&
    ZZ_is_equals(a->x, b->x) &&
    ZZ_is_equals(a->y, b->y);
}

__EXPORT__ bool EF_to_string(const EF *ef, char *p, int maxlen) {
  auto c = EF_to_string_as_std_string(ef);
  if (c.size() < maxlen) {
    strcpy(p, c.c_str());
    return true;
  }
  return false;
}

string EF_to_string_as_std_string(const EF *ef) {
  stringstream ss;
  ss << "(" << ZZ_to_string_as_std_string(ef->x)
     << ", " << ZZ_to_string_as_std_string(ef->y)
     << ") over Extended Field GF("
     << ZZ_to_string_as_std_string(ef->modulo)
     << "^2), Irreducible Polynomial: ";
  switch (ef->poly) {
  case IrreduciblePolynomialType::X2_1:
    ss << "x^2 + 1";
    break;
  case IrreduciblePolynomialType::X2_X_1:
    ss << "x^2 + x + 1";
    break;
  }
  return ss.str();
}
