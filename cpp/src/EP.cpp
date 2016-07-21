#include "ecpy_native.h"
#include <cassert>
#include "EP_impl.h"
#include "EC_impl.h"
#include "EF_impl.h"
#include "FF_impl.h"
#include "ZZ_impl.h"

using namespace std;
using namespace g_object;
__EXPORT__ EP *EP_EF_add(const EP*, const EP*);

MAKE_FUNC_TABLE(_ep_ff_func, EP_destroy, EP_FF_add, nullptr, nullptr, nullptr, nullptr, nullptr, EP_equals, EP_is_same_type, EP_to_std_string, EP_copy);
MAKE_FUNC_TABLE(_ep_ef_func, EP_destroy, EP_EF_add, nullptr, nullptr, nullptr, nullptr, nullptr, EP_equals, EP_is_same_type, EP_to_std_string, EP_copy);

__EXPORT__ EP *EP_FF_create_with_FF(const EC *ec, const FF *x, const FF *y, const FF *z) {
  assert(is_same_type(AS_OBJECT_CONST(x), AS_OBJECT_CONST(y)) && is_same_type(AS_OBJECT_CONST(y), AS_OBJECT_CONST(z)));
  EP *P = new EP;
  P->functions = _ep_ff_func;
  P->objtype = ObjectType::EP_FF;
  P->curve = ec;
  P->x = copy(AS_OBJECT_CONST(x));
  P->y = copy(AS_OBJECT_CONST(y));
  P->z = copy(AS_OBJECT_CONST(z));
  P->u.FF.p = copy(x->p);
  return P;
}

__EXPORT__ EP *EP_FF_create(const EC *ec, const char *x, const char *y, const char *z, const char *p) {
  EP *P = new EP;
  P->functions = _ep_ff_func;
  P->objtype = ObjectType::EP_FF;
  P->curve = ec;
  P->x = AS_OBJECT(FF_create(x, p));
  P->y = AS_OBJECT(FF_create(y, p));
  P->z = AS_OBJECT(FF_create(z, p));
  P->u.FF.p = AS_OBJECT(ZZ_create(p));
  return P;
}

__EXPORT__ EP *EP_EF_create_with_EF(const EC *ec, const EF *x, const EF *y, const EF *z) {
  assert(is_same_type(AS_OBJECT_CONST(x), AS_OBJECT_CONST(y)) && is_same_type(AS_OBJECT_CONST(y), AS_OBJECT_CONST(z)));
  EP *P = new EP;
  P->functions = _ep_ef_func;
  P->objtype = ObjectType::EP_FF;
  P->curve = ec;
  P->x = copy(AS_OBJECT_CONST(x));
  P->y = copy(AS_OBJECT_CONST(y));
  P->z = copy(AS_OBJECT_CONST(z));
  P->u.EF.modulo = copy(x->modulo);
  P->u.EF.type = x->poly;
  return P;
}

__EXPORT__ EP *EP_EF_create(const EC *ec, const char *x1, const char *x2, const char *y1, const char *y2, const char *z1, const char *z2, const char *modulo, const char *poly) {
  EP *P = new EP;
  P->functions = _ep_ef_func;
  P->objtype = ObjectType::EP_FF;
  P->curve = ec;
  P->x = AS_OBJECT(EF_create(x1, x2, modulo, poly));
  P->y = AS_OBJECT(EF_create(y1, y2, modulo, poly));
  P->z = AS_OBJECT(EF_create(z1, z2, modulo, poly));
  P->u.EF.modulo = AS_OBJECT(ZZ_create(modulo));
  P->u.EF.type = EF_Get_Polynomial(poly);
  return P;
}

__EXPORT__ void EP_destroy(EP *ep) {
  destroy(ep->x);
  destroy(ep->y);
  destroy(ep->z);
  switch(ep->curve->type) {
  case EC_Type::FF:
    destroy(ep->u.FF.p);
    break;
  case EC_Type::EF:
    destroy(ep->u.EF.modulo);
    break;
  }
  ep->objtype = ObjectType::FREE;
  delete ep;
}

bool EP_FF_is_infinity(const EP *ep) {
  auto x = to_ZZ(to_FF(ep->x)->x)->x;
  auto y = to_ZZ(to_FF(ep->y)->x)->x;
  auto z = to_ZZ(to_FF(ep->z)->x)->x;
  return x == z && y == 1 && x == 0;
}

bool EP_EF_is_infinity(const EP *ep) {
  auto x1 = to_ZZ(to_EF(ep->x)->x)->x;
  auto x2 = to_ZZ(to_EF(ep->x)->y)->x;
  auto y1 = to_ZZ(to_EF(ep->y)->x)->x;
  auto y2 = to_ZZ(to_EF(ep->y)->y)->x;
  auto z1 = to_ZZ(to_EF(ep->z)->x)->x;
  auto z2 = to_ZZ(to_EF(ep->z)->y)->x;
  return x1 == z1 && y1 == 1 && x1 == 0 && x2 == y2 && y2 == z2 && x2 == 0;
}

bool EP_equals(const EP *ep, const EP *fp) {
  if (is_same_type(AS_OBJECT_CONST(ep), AS_OBJECT_CONST(fp))) {
    auto t = mul(ep->x, fp->y);  // x(P) * y(Q)
    auto u = mul(fp->x, ep->y);  // x(Q) * y(P)
    auto ret = equals(t, u);     // x(P) * y(Q) = x(Q) * y(P) mod p?
    destroy(t);
    destroy(u);
    return ret;
  }
  return false;
}

bool EP_is_same_type(const g_object_t *a, const g_object_t *b) {
  if (a->type == b->type) {
    if (a->type == ObjectType::EP_FF) {
      auto a_ = to_EP_FF(const_cast<g_object_t*>(a));
      auto b_ = to_EP_FF(const_cast<g_object_t*>(b));
      return is_same_type(AS_OBJECT_CONST(a_->curve), AS_OBJECT_CONST(b_->curve)) && equals(a_->u.FF.p, b_->u.FF.p);
    } else if (b->type == ObjectType::EP_EF) {
      auto a_ = to_EP_EF(const_cast<g_object_t*>(a));
      auto b_ = to_EP_EF(const_cast<g_object_t*>(b));
      return is_same_type(AS_OBJECT_CONST(a_->curve), AS_OBJECT_CONST(b_->curve)) && equals(a_->u.EF.modulo, b_->u.EF.modulo) && a_->u.EF.type == b_->u.EF.type;
    }
  }
  return false;
}

string EP_to_std_string(const EP *ep) {
  stringstream ss;
  ss << "ECPoint (" << to_std_string(ep->x)
     << ", " << to_std_string(ep->y)
     << ", " << to_std_string(ep->z)
     << ") over " << to_std_string(AS_OBJECT_CONST(ep->curve));
  return ss.str();
}

EP *EP_copy(const EP *ep) {
  EP *ret = new EP;
  ret->functions = ep->functions;
  ret->objtype = ep->objtype;
  ret->curve = ep->curve;
  ret->x = copy(ep->x);
  ret->y = copy(ep->y);
  ret->z = copy(ep->z);
  ret->u = ep->u;
  return ret;
}

__EXPORT__ EP *EP_FF_add(const EP *a, const EP *b) {
  assert(is_same_type(AS_OBJECT_CONST(a), AS_OBJECT_CONST(b)));
  auto Px = static_cast<mpz_class>(to_ZZ(to_FF(a->x)->x)->x);
  auto Py = static_cast<mpz_class>(to_ZZ(to_FF(a->y)->x)->x);
  auto Pz = static_cast<mpz_class>(to_ZZ(to_FF(a->z)->x)->x);
  auto Qx = to_ZZ(to_FF(b->x)->x)->x;
  auto Qy = to_ZZ(to_FF(b->y)->x)->x;
  auto Qz = to_ZZ(to_FF(b->z)->x)->x;
  auto p = to_ZZ(a->u.FF.p)->x;
  if (EP_FF_is_infinity(a)) {
    return to_EP_FF(copy(AS_OBJECT_CONST(b)));
  } else if (EP_FF_is_infinity(b)) {
    return to_EP_FF(copy(AS_OBJECT_CONST(a)));
  }
  if (equals(AS_OBJECT_CONST(a), AS_OBJECT_CONST(b))) { // Point doubling
    auto X = Px;
    auto Y = Py;
    auto Z = Pz;
    auto u = static_cast<mpz_class>((3 * X * X + to_ZZ(a->curve->a)->x * Z * Z) % p);
    auto v = static_cast<mpz_class>((Y * Z) % p);
    auto a = static_cast<mpz_class>((Y * v) % p);
    auto w = static_cast<mpz_class>((u * u - 8 * X * a) % p);
    auto Rx = FF_create_from_mpz_class(2 * v * w,  p);
    auto Ry = FF_create_from_mpz_class(u * (4 * X * a - w) - 8 * a * a, p);
    auto Rz = FF_create_from_mpz_class(8 * v * v * v,  p);
    auto ret = EP_FF_create_with_FF(b->curve, Rx, Ry, Rz);
    destroy(AS_OBJECT(Rx));
    destroy(AS_OBJECT(Ry));
    destroy(AS_OBJECT(Rz));
    return ret;
  } else {
    auto u = static_cast<mpz_class>((Qy * Pz - Py * Qz) % p);
    auto v = static_cast<mpz_class>((Qx * Pz - Px * Qz) % p);
    auto v2 = static_cast<mpz_class>((v * v) % p);
    auto v3 = static_cast<mpz_class>((v2 * v) % p);
    auto w = static_cast<mpz_class>((u * u * Pz * Qz - v3 - 2 * v2 * Px * Qz) % p);
    auto Rx = FF_create_from_mpz_class(v * w, p);
    auto Ry = FF_create_from_mpz_class(u * (v2 * Px * Qz - w) - v3 * Py * Qz, p);
    auto Rz = FF_create_from_mpz_class(v3 * Pz * Qz, p);
    auto ret = EP_FF_create_with_FF(b->curve, Rx, Ry, Rz);
    destroy(AS_OBJECT(Rx));
    destroy(AS_OBJECT(Ry));
    destroy(AS_OBJECT(Rz));
    return ret;
  }
}

template <class T>
inline mpz_class modulo(T a, mpz_class b) {
  auto c = a % b;
  if (c < 0) {
    return c + b;
  }
  return c;
}

__EXPORT__ EP *EP_EF_add(const EP *a, const EP *b) {
  assert(is_same_type(AS_OBJECT_CONST(a), AS_OBJECT_CONST(b)));
  if (EP_EF_is_infinity(a)) {
    return to_EP_EF(copy(AS_OBJECT_CONST(b)));
  } else if (EP_EF_is_infinity(b)) {
    return to_EP_EF(copy(AS_OBJECT_CONST(a)));
  }
  if (a->u.EF.type == IrreduciblePolynomialType::X2_1) {
    auto poly = to_EF(a->x)->poly;
    auto Px1 = to_ZZ(to_EF(a->x)->x)->x;
    auto Py1 = to_ZZ(to_EF(a->y)->x)->x;
    auto Pz1 = to_ZZ(to_EF(a->z)->x)->x;
    auto Px2 = to_ZZ(to_EF(a->x)->y)->x;
    auto Py2 = to_ZZ(to_EF(a->y)->y)->x;
    auto Pz2 = to_ZZ(to_EF(a->z)->y)->x;
    auto Qx1 = to_ZZ(to_EF(b->x)->x)->x;
    auto Qy1 = to_ZZ(to_EF(b->y)->x)->x;
    auto Qz1 = to_ZZ(to_EF(b->z)->x)->x;
    auto Qx2 = to_ZZ(to_EF(b->x)->y)->x;
    auto Qy2 = to_ZZ(to_EF(b->y)->y)->x;
    auto Qz2 = to_ZZ(to_EF(b->z)->y)->x;
    auto p = to_ZZ(a->u.EF.modulo)->x;
    auto A = to_ZZ(a->curve->a)->x;
    if (equals(AS_OBJECT_CONST(a), AS_OBJECT_CONST(b))) { // Point doubling
      auto u1 = modulo(3 * (Px1 * Px1 - Px2 * Px2) + A * (Pz1 * Pz1 - Pz2 * Pz2), p);
      auto u2 = modulo(2*(3*Px1*Px2 + A*Pz1*Pz2), p);
      auto v1 = modulo(Py1 * Pz1 - Py2 * Pz2, p);
      auto v2 = modulo(Py2 * Pz1 + Py1 * Pz2, p);
      auto alpha1 = modulo(Py1 * v1 - Py2 * v2, p);
      auto alpha2 = modulo(Py2 * v1 + Py1 * v2, p);
      auto w1 = modulo(u1*u1 - u2*u2 - 8 * (Px1 * alpha1 - Px2 * alpha2), p);
      auto w2 = modulo(2*u1*u2 + alpha2*Px1 + alpha1*Px2, p);
      auto Rx = EF_create_from_mpz_class(
          modulo(2 * (v1*w1 - v2 * w2), p),
          modulo(2 * (v2 * w1 + v1 * w2), p), p, poly);
      auto ax1 = modulo(alpha1 * Px1 - alpha2 * Px2, p);
      auto ax2 = modulo(alpha2 * Px1 - alpha1 * Px2, p);
      auto gamma1 = modulo(4*ax1 - w1, p);
      auto gamma2 = modulo(4*ax2 - w2, p);
      auto Ry = EF_create_from_mpz_class(
          modulo(u1 * gamma1 + u2 * gamma2 - 8 * (alpha1*alpha1 - alpha2*alpha2), p),
          modulo(u2 * gamma1 + u1 * gamma2 - 16 * alpha1 * alpha2, p), p, poly);
      auto Rz = EF_create_from_mpz_class(
          modulo(v1*v1*v1 - 3*v1*v2*v2, p),
          modulo(3*v1*v1*v2-v2*v2*v2, p), p, poly);
      auto ret = EP_EF_create_with_EF(b->curve, Rx, Ry, Rz);
      destroy(AS_OBJECT(Rx));
      destroy(AS_OBJECT(Ry));
      destroy(AS_OBJECT(Rz));
      return ret;
    } else {
      /*auto u = static_cast<mpz_class>((Qy * Pz - Py * Qz) % p);
      auto v = static_cast<mpz_class>((Qx * Pz - Px * Qz) % p);
      auto v2 = static_cast<mpz_class>((v * v) % p);
      auto v3 = static_cast<mpz_class>((v2 * v) % p);
      auto w = static_cast<mpz_class>((u * u * Pz * Qz - v3 - 2 * v2 * Px * Qz) % p);
      auto Rx = FF_create_from_mpz_class(v * w, p);
      auto Ry = FF_create_from_mpz_class(u * (v2 * Px * Qz - w) - v3 * Py * Qz, p);
      auto Rz = FF_create_from_mpz_class(v3 * Pz * Qz, p);
      auto ret = EP_FF_create_with_FF(b->curve, Rx, Ry, Rz);
      destroy(AS_OBJECT(Rx));
      destroy(AS_OBJECT(Ry));
      destroy(AS_OBJECT(Rz));
      return ret;*/
    }
  }
  return nullptr;
}
