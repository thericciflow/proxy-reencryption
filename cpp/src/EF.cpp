#include "ecpy_native.h"
#include "EF_impl.h"

using namespace std;
using namespace g_object;

MAKE_FUNC_TABLE(_ef_func, EF_destroy, EF_add, EF_neg, EF_mul, nullptr, EF_inv, nullptr, EF_is_equals, EF_is_same_type, EF_to_string_as_std_string, nullptr);

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
  ef->type = ObjectType::EF;
  ef->functions = _ef_func;
  ef->modulo = AS_OBJECT(ZZ_create_from_mpz_class(n));
  ef->poly = type;
  ef->x = AS_OBJECT(ZZ_create_from_mpz_class(x % n));
  ef->y = AS_OBJECT(ZZ_create_from_mpz_class(y % n));
  return ef;
}

__EXPORT__ EF *EF_create(const char *x, const char *y, const char *n, const char *poly) {
  EF *ef = new EF;
  ef->type = ObjectType::EF;
  ef->functions = _ef_func;
  ef->modulo = AS_OBJECT(ZZ_create(n));
  {
    auto t = AS_OBJECT(ZZ_create(x));
    ef->x = mod(t, ef->modulo);
    destroy(t);
  }
  {
    auto t = AS_OBJECT(ZZ_create(y));
    ef->y = mod(t, ef->modulo);
    destroy(t);
  }
  ef->poly = EF_Get_Polynomial(poly);
  return ef;
}

__EXPORT__ void EF_destroy(const EF *ef) {
  destroy(ef->modulo);
  destroy(ef->x);
  destroy(ef->y);
  delete ef;
}

__EXPORT__ EF *EF_add(const EF *a, const EF *b) {
  EF *ret = new EF;
  ret->type = ObjectType::EF;
  ret->functions = _ef_func;
  if (a->poly == b->poly && equals(a->modulo, b->modulo)) {
    switch (a->poly) { // addition is same operation
    case IrreduciblePolynomialType::X2_1:
    case IrreduciblePolynomialType::X2_X_1:
      {
        auto t = add(a->x, b->x);
        ret->x = mod(t, a->modulo);
        destroy(t);
      }
      {
        auto t = add(a->y, b->y);
        ret->y = mod(t, a->modulo);
        destroy(t);
      }
    }
    ret->modulo = copy(a->modulo);
    ret->poly = a->poly;
  } else {
    ret->x = AS_OBJECT(ZZ_create_from_mpz_class(-1));
    ret->y = AS_OBJECT(ZZ_create_from_mpz_class(-1));
    ret->modulo = AS_OBJECT(ZZ_create_from_mpz_class(-1));
    ret->poly = IrreduciblePolynomialType::X2_1;
  }
  return ret;
}

__EXPORT__ EF *EF_neg(const EF *a) {
  EF *ret = new EF;
  ret->type = ObjectType::EF;
  ret->functions = _ef_func;
  switch (a->poly) {
  case IrreduciblePolynomialType::X2_1:
  case IrreduciblePolynomialType::X2_X_1:
    {
      auto t = neg(a->x);
      ret->x = mod(t, a->modulo);
      destroy(t);
    }
    {
      auto t = neg(a->y);
      ret->y = mod(t, a->modulo);
      destroy(t);
    }
  }
  ret->modulo = copy(a->modulo);
  ret->poly = a->poly;
  return ret;
}

__EXPORT__ EF *EF_mul(const EF *a, const EF *b) {
  EF *ret = new EF;
  ret->type = ObjectType::EF;
  ret->functions = _ef_func;
  auto i = a->x;
  auto j = a->y;
  auto k = b->x;
  auto l = b->y;
  if (a->poly == b->poly && equals(a->modulo, b->modulo)) {
    switch (a->poly) {
    case IrreduciblePolynomialType::X2_1:
      {
        auto t = mul(i, k); // ac
        auto u = mul(j, l); // bd
        auto v = neg(u);    // -bd
        auto w = add(t, v); // ac-bd
        ret->x = mod(w, a->modulo);
        destroy(t);
        destroy(u);
        destroy(v);
        destroy(w);
      }
      {
        auto t = mul(i, l); // ad
        auto u = mul(j, k); // bc
        auto v = add(t, u); // ad+bc
        ret->y = mod(v, a->modulo);
        destroy(t);
        destroy(u);
        destroy(v);
      }
      break;
    case IrreduciblePolynomialType::X2_X_1:
      {
        auto t = mul(i, k); // ac
        auto u = mul(j, l); // bd
        auto v = neg(u);    // -bd
        auto w = add(t, v); // ac-bd
        ret->x = mod(w, a->modulo);
        destroy(t);
        destroy(u);
        destroy(v);
        destroy(w);
      }
      {
        auto t = mul(i, l); // ad
        auto u = mul(j, k); // bc
        auto v = add(t, u); // ad+bc
        auto w = mul(j, l); // bd
        auto x = neg(w);    // -bd
        auto y = add(v, x); // ad+bc-bd
        ret->y = mod(y, a->modulo);
        destroy(t);
        destroy(u);
        destroy(v);
        destroy(w);
        destroy(x);
        destroy(y);
      }
      break;
    }
    ret->modulo = copy(a->modulo);
    ret->poly = a->poly;
  } else {
    ret->x = AS_OBJECT(ZZ_create_from_mpz_class(-1));
    ret->y = AS_OBJECT(ZZ_create_from_mpz_class(-1));
    ret->modulo = AS_OBJECT(ZZ_create_from_mpz_class(-1));
    ret->poly = IrreduciblePolynomialType::X2_1;
  }
  return ret;
}

__EXPORT__ EF *EF_inv(const EF *x) {
  EF *ret = new EF;
  ret->type = ObjectType::EF;
  ret->functions = _ef_func;
  auto a = x->x;
  auto b = x->y;
  auto r = mul(a, a); // a^2
  auto s = mul(b, b); // b^2
  auto t = add(r, s); // a^2+b^2
  destroy(r);
  destroy(s);
  switch (x->poly) {
  case IrreduciblePolynomialType::X2_1:
    {
      auto u = AS_OBJECT(ZZ_modinv(to_ZZ(t), to_ZZ(x->modulo)));
      {
        auto z = mul(a, u);
        ret->x = mod(z, x->modulo);
        destroy(z);
      }
      {
        auto s = mul(b, u);
        auto z = neg(s);
        ret->y = mod(z, x->modulo);
        destroy(z);
        destroy(s);
      }
      destroy(u);
    }
    break;
  case IrreduciblePolynomialType::X2_X_1:
    {
      auto f = mul(a, b); // ab
      auto g = neg(f);    // -ab
      auto h = add(t, g); // a^2-ab+b^2
      destroy(f);
      destroy(g);
      auto u = AS_OBJECT(ZZ_modinv(to_ZZ(h), to_ZZ(x->modulo)));
      destroy(h);
      {
        auto g = neg(b);
        auto h = add(g, a);
        auto z = mul(h, u);
        ret->x = mod(z, x->modulo);
        destroy(g);
        destroy(h);
        destroy(z);
      }
      {
        auto s = mul(b, u);
        auto z = neg(s);
        ret->y = mod(z, x->modulo);
        destroy(s);
        destroy(z);
      }
      destroy(u);
    }
    break;
  }
  destroy(t);
  ret->modulo = copy(x->modulo);
  ret->poly = x->poly;
  return ret;
}

__EXPORT__ bool EF_is_equals(const EF *a, const EF *b) {
  return (a->poly == b->poly) &&
    equals(a->modulo, b->modulo) &&
    equals(a->x, b->x) &&
    equals(a->y, b->y);
}

bool EF_is_same_type(const g_object_t *a, const g_object_t *b) {
  return (a->type == b->type && a->type == ObjectType::EF &&
      to_EF(a)->poly == to_EF(b)->poly && equals(to_EF(a)->modulo, to_EF(b)->modulo));
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
  ss << "(" << to_std_string(ef->x)
     << ", " << to_std_string(ef->y)
     << ") over Extended Field GF("
     << to_std_string(ef->modulo)
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
