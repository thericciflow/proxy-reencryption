#include "ecpy_native.h"
#include "EP_impl.h"
#include "EC_impl.h"
#include "FF_impl.h"
#include "EF_impl.h"
#include "ZZ_impl.h"
using namespace std;
using namespace g_object;

template <class T>
mpz_class modulo(T a, mpz_class modulus) {
  auto ret = a % modulus;
  if (ret < 0) {
    return a + modulus;
  }
  return a;
}

__EXPORT__ g_object_t *EP_line_coeff(const EP *a, const EP *b) {
  auto A = to_ZZ(a->curve->a)->x;
  switch (a->objtype) {
  case ObjectType::EP_FF:
    {
      auto Px = to_ZZ(to_FF(a->x)->x)->x;
      auto Py = to_ZZ(to_FF(a->y)->x)->x;
      auto Pz = to_ZZ(to_FF(a->z)->x)->x;
      auto modulus = to_ZZ(a->u.FF.p)->x;
      if (equals(AS_OBJECT_CONST(a), AS_OBJECT_CONST(b))) {
        auto p = modulo(3 * Px * Px + A * Pz * Pz, modulus);
        auto q = modulo(2 * Py * Pz, modulus);
        return AS_OBJECT(FF_create_from_mpz_class(modinv(q, modulus) * p, modulus));
      } else {
        auto Qx = to_ZZ(to_FF(b->x)->x)->x;
        auto Qy = to_ZZ(to_FF(b->y)->x)->x;
        auto Qz = to_ZZ(to_FF(b->z)->x)->x;
        auto p = modulo(Qy*Pz - Py*Qz, modulus);
        auto q = modulo(Qx*Pz - Px*Qz, modulus);
        return AS_OBJECT(FF_create_from_mpz_class(modinv(q, modulus) * p, modulus));
      }
    }
  case ObjectType::EP_EF:
    {
      auto Px1 = to_ZZ(to_EF(a->x)->x)->x;
      auto Px2 = to_ZZ(to_EF(a->x)->y)->x;
      auto Py1 = to_ZZ(to_EF(a->y)->x)->x;
      auto Py2 = to_ZZ(to_EF(a->y)->y)->x;
      auto Pz1 = to_ZZ(to_EF(a->z)->x)->x;
      auto Pz2 = to_ZZ(to_EF(a->z)->y)->x;
      auto Qx1 = to_ZZ(to_EF(b->x)->x)->x;
      auto Qx2 = to_ZZ(to_EF(b->x)->y)->x;
      auto Qy1 = to_ZZ(to_EF(b->y)->x)->x;
      auto Qy2 = to_ZZ(to_EF(b->y)->y)->x;
      auto Qz1 = to_ZZ(to_EF(b->z)->x)->x;
      auto Qz2 = to_ZZ(to_EF(b->z)->y)->x;
      auto modulus = to_ZZ(a->u.EF.modulo)->x;
      switch (a->u.EF.type) {
      case IrreduciblePolynomialType::X2_1:
        {
          auto b1 = modulo(MUL_P1_RE(Px1, Px2, Qz1, Qz2) - MUL_P1_RE(Qx1, Qx2, Pz1, Pz2), modulus);
          auto b2 = modulo(MUL_P1_IM(Px1, Px2, Qz1, Qz2) - MUL_P1_IM(Qx1, Qx2, Pz1, Pz2), modulus);
          if (b1 == b2 && b2 == 0) {
            auto t1 = modulo(3 * POW2_P1_RE(Px1, Px2), modulus);
            auto t2 = modulo(3 * POW2_P1_IM(Px1, Px2), modulus);
            auto u1 = modulo(A * POW2_P1_RE(Pz1, Pz2), modulus);
            auto u2 = modulo(A * POW2_P1_IM(Pz1, Pz2), modulus);
            auto p1 = modulo(t1 + u1, modulus);
            auto p2 = modulo(t2 + u2, modulus);
            auto q1 = modulo(2 * MUL_P1_RE(Py1, Py2, Pz1, Pz2), modulus);
            auto q2 = modulo(2 * MUL_P1_IM(Py1, Py2, Pz1, Pz2), modulus);
            auto p = AS_OBJECT(EF_create_from_mpz_class(p1, p2, modulus, IrreduciblePolynomialType::X2_1));
            auto q = AS_OBJECT(EF_create_from_mpz_class(q1, q2, modulus, IrreduciblePolynomialType::X2_1));
            auto x = inv(q);
            auto ret = mul(p, x);
            destroy(p);
            destroy(q);
            destroy(x);
            return ret;
          } else {
            auto p1 = modulo(MUL_P1_RE(Qy1, Qy2, Pz1, Pz2) - MUL_P1_RE(Py1, Py2, Qz1, Qz2), modulus);
            auto p2 = modulo(MUL_P1_IM(Qy1, Qy2, Pz1, Pz2) - MUL_P1_IM(Py1, Py2, Qz1, Qz2), modulus);
            auto q1 = modulo(MUL_P1_RE(Qx1, Qx2, Pz1, Pz2) - MUL_P1_RE(Px1, Px2, Qz1, Qz2), modulus);
            auto q2 = modulo(MUL_P1_IM(Qx1, Qx2, Pz1, Pz2) - MUL_P1_IM(Px1, Px2, Qz1, Qz2), modulus);
            auto p = AS_OBJECT(EF_create_from_mpz_class(p1, p2, modulus, IrreduciblePolynomialType::X2_1));
            auto q = AS_OBJECT(EF_create_from_mpz_class(q1, q2, modulus, IrreduciblePolynomialType::X2_1));
            auto x = inv(q);
            auto ret = mul(p, x);
            destroy(p);
            destroy(q);
            destroy(x);
            return ret;
          }
        }
      case IrreduciblePolynomialType::X2_X_1:
        {
          auto b1 = modulo(MUL_P2_RE(Px1, Px2, Qz1, Qz2) - MUL_P2_RE(Qx1, Qx2, Pz1, Pz2), modulus);
          auto b2 = modulo(MUL_P2_IM(Px1, Px2, Qz1, Qz2) - MUL_P2_IM(Qx1, Qx2, Pz1, Pz2), modulus);
          if (b1 == b2 && b2 == 0) {
            auto t1 = modulo(3 * POW2_P2_RE(Px1, Px2), modulus);
            auto t2 = modulo(3 * POW2_P2_IM(Px1, Px2), modulus);
            auto u1 = modulo(A * POW2_P2_RE(Pz1, Pz2), modulus);
            auto u2 = modulo(A * POW2_P2_IM(Pz1, Pz2), modulus);
            auto p1 = modulo(t1 + u1, modulus);
            auto p2 = modulo(t2 + u2, modulus);
            auto q1 = modulo(2 * MUL_P2_RE(Py1, Py2, Pz1, Pz2), modulus);
            auto q2 = modulo(2 * MUL_P2_IM(Py1, Py2, Pz1, Pz2), modulus);
            auto p = AS_OBJECT(EF_create_from_mpz_class(p1, p2, modulus, IrreduciblePolynomialType::X2_X_1));
            auto q = AS_OBJECT(EF_create_from_mpz_class(q1, q2, modulus, IrreduciblePolynomialType::X2_X_1));
            auto x = inv(q);
            auto ret = mul(p, x);
            destroy(p);
            destroy(q);
            destroy(x);
            return ret;
          } else {
            auto p1 = modulo(MUL_P2_RE(Qy1, Qy2, Pz1, Pz2) - MUL_P2_RE(Py1, Py2, Qz1, Qz2), modulus);
            auto p2 = modulo(MUL_P2_IM(Qy1, Qy2, Pz1, Pz2) - MUL_P2_IM(Py1, Py2, Qz1, Qz2), modulus);
            auto q1 = modulo(MUL_P2_RE(Qx1, Qx2, Pz1, Pz2) - MUL_P2_RE(Px1, Px2, Qz1, Qz2), modulus);
            auto q2 = modulo(MUL_P2_IM(Qx1, Qx2, Pz1, Pz2) - MUL_P2_IM(Px1, Px2, Qz1, Qz2), modulus);
            auto p = AS_OBJECT(EF_create_from_mpz_class(p1, p2, modulus, IrreduciblePolynomialType::X2_X_1));
            auto q = AS_OBJECT(EF_create_from_mpz_class(q1, q2, modulus, IrreduciblePolynomialType::X2_X_1));
            auto x = inv(q);
            auto ret = mul(p, x);
            destroy(p);
            destroy(q);
            destroy(x);
            return ret;
          }
        }
      }
    }
  }
}

namespace {
  bool equals_mpz_class(g_object_t *object, mpz_class x) {
    g_object_t *z;
    bool ret;
    switch (object->type) {
    case ObjectType::ZZ:
      z = AS_OBJECT(ZZ_create_from_mpz_class(x));
      ret = equals(object, z);
      break;
    case ObjectType::FF:
      z = AS_OBJECT(FF_create_from_mpz_class(x, to_ZZ(to_FF(object)->p)->x));
      ret = equals(object, z);
      break;
    case ObjectType::EF:
      z = AS_OBJECT(EF_create_from_mpz_class(x, 0, to_ZZ(to_EF(object)->modulo)->x, to_EF(object)->poly));
      ret = equals(object, z);
      break;
    }
    destroy(z);
    return ret;
  }

  inline g_object_t *sub(g_object_t *a, g_object_t *b) {
    auto c = neg(b);
    auto ret = add(a, c);
    destroy(c);
    return ret;
  }

  inline g_object_t *vertical(const EP *P, const EP *Q, const EP *R) {
    auto t = mul(R->x, P->z);
    auto u = mul(R->z, P->x);
    auto w = sub(t, u);
    auto x = mul(P->z, R->z);
    auto y = div(w, x);
    destroy(t);
    destroy(u);
    destroy(w);
    destroy(x);
    return y;
  }

  inline g_object_t *EP_h_function(const EP *P, const EP *Q, const EP *R) {
    if (equals(AS_OBJECT_CONST(P), AS_OBJECT_CONST(Q)) && equals_mpz_class(P->y, 0)) {
      return vertical(P, Q, R);
    } else {
      auto p = mul(P->x, Q->z);
      auto q = mul(P->z, Q->x);
      auto ret_b = equals(p, q);
      destroy(p);
      destroy(q);
      if (ret_b) {
        return vertical(P, Q, R);
      }
    }
    auto L = AS_OBJECT(EP_line_coeff(P, Q));
    g_object_t *p, *q;
    { // calculate p
      auto a = mul(R->y, P->z);
      auto b = mul(R->z, R->y);
      auto c = mul(R->x, P->z);
      auto d = mul(R->z, P->x);
      auto x = sub(a, b);
      auto y = sub(c, d);
      auto z = mul(L, y);
      auto pbar = sub(x, z);
      p = mul(Q->z, pbar);
      destroy(a);
      destroy(b);
      destroy(c);
      destroy(d);
      destroy(x);
      destroy(y);
      destroy(z);
      destroy(pbar);
    }
    { // calculate q
      auto abar = mul(Q->z, R->z);
      auto a = mul(abar, P->x);
      auto bbar = mul(P->z, Q->x);
      auto b = mul(bbar, R->z);
      auto cbar = mul(P->z, Q->z);
      auto c = mul(cbar, R->x);
      auto dbar2 = mul(L, L);
      auto dbar = mul(dbar2, P->z);
      auto d = mul(dbar, abar);
      auto x = add(a, b);
      auto y = sub(c, d);
      q = add(x, y);
      destroy(abar);
      destroy(a);
      destroy(bbar);
      destroy(b);
      destroy(cbar);
      destroy(c);
      destroy(dbar2);
      destroy(dbar);
      destroy(d);
      destroy(x);
      destroy(y);
    }
    cout << to_std_string(p) << " : " << to_std_string(q) << endl;
    auto ret = div(p, q);
    destroy(p);
    destroy(q);
    destroy(L);
    return ret;
  }

  __EXPORT__ g_object_t *EP_miller(const EP *_P, const EP *_Q, const ZZ *_m) {
    auto m = _m->x;
    if (equals(AS_OBJECT_CONST(_P), AS_OBJECT_CONST(_Q))) {
      return AS_OBJECT(ZZ_create_from_mpz_class(1));
    }
    auto T = copy(AS_OBJECT_CONST(_P));
    int n = mpz_sizeinbase(m.get_mpz_t(), 2);
    g_object_t *f;
    if (_P->objtype == ObjectType::EP_FF) {
      f = AS_OBJECT(FF_create_from_mpz_class(1, to_ZZ(_P->u.FF.p)->x));
    } else if (_P->objtype == ObjectType::EP_EF) {
      f = AS_OBJECT(EF_create_from_mpz_class(1, 0, to_ZZ(_P->u.EF.modulo)->x, _P->u.EF.type));
    } else {
      throw logic_error("Invalid EP Type");
    }
    for (int i = n - 2; i >= 0; i--) {
      { // update f 1
        auto b = f;
        auto t = mul(f, f);
        auto h = EP_h_function(to_EP_force(T), to_EP_force(T), _Q);
        f = mul(t, h);
        destroy(b);
        destroy(t);
        destroy(h);
      }
      { // T <- T + T
        auto b = T;
        T = add(T, T);
        destroy(b);
      }
      if (mpz_tstbit(m.get_mpz_t(), i)) {
        { // update f 2
          auto t = f;
          auto h = EP_h_function(to_EP_force(T), _P, _Q);
          f = mul(f, h);
          destroy(t);
          destroy(h);
        }
        { // T <- T + P
          auto b = T;
          T = add(T, AS_OBJECT_CONST(_P));
          destroy(b);
        }
      }
    }
    destroy(T);
    return f;
  }
}; // anonymous namespace
