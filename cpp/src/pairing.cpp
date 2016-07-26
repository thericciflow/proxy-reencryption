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
        return AS_OBJECT(ZZ_create_from_mpz_class((modinv(q, modulus) * p) % modulus));
      } else {
        auto Qx = to_ZZ(to_FF(b->x)->x)->x;
        auto Qy = to_ZZ(to_FF(b->y)->x)->x;
        auto Qz = to_ZZ(to_FF(b->z)->x)->x;
        auto p = modulo(Qy*Pz - Py*Qz, modulus);
        auto q = modulo(Qx*Pz - Px*Qz, modulus);
        return AS_OBJECT(ZZ_create_from_mpz_class((modinv(q, modulus) * p) % modulus));
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
