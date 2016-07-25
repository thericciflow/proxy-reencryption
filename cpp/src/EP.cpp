#include "ecpy_native.h"
#include <cassert>
#include "EP_impl.h"
#include "EC_impl.h"
#include "EF_impl.h"
#include "FF_impl.h"
#include "ZZ_impl.h"

using namespace std;
using namespace g_object;

MAKE_FUNC_TABLE(_ep_ff_func, EP_destroy, EP_FF_add, nullptr, EP_mul, nullptr, nullptr, nullptr, EP_equals, EP_is_same_type, EP_to_std_string, EP_copy);
MAKE_FUNC_TABLE(_ep_ef_func, EP_destroy, EP_EF_add, nullptr, EP_mul, nullptr, nullptr, nullptr, EP_equals, EP_is_same_type, EP_to_std_string, EP_copy);

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
  P->objtype = ObjectType::EP_EF;
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
  P->objtype = ObjectType::EP_EF;
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
      if (EP_FF_is_infinity(to_EP_force(a)) || EP_FF_is_infinity(to_EP_force(b))) {
        return true;
      }
      auto a_ = to_EP_FF(const_cast<g_object_t*>(a));
      auto b_ = to_EP_FF(const_cast<g_object_t*>(b));
      return (is_same_type(AS_OBJECT_CONST(a_->curve), AS_OBJECT_CONST(b_->curve)) && equals(a_->u.FF.p, b_->u.FF.p));
    } else if (b->type == ObjectType::EP_EF) {
      if (EP_EF_is_infinity(to_EP_force(a)) || EP_EF_is_infinity(to_EP_force(b))) {
        return true;
      }
      auto a_ = to_EP_EF(const_cast<g_object_t*>(a));
      auto b_ = to_EP_EF(const_cast<g_object_t*>(b));
      return is_same_type(AS_OBJECT_CONST(a_->curve), AS_OBJECT_CONST(b_->curve)) && (equals(a_->u.EF.modulo, b_->u.EF.modulo) || (EP_EF_is_infinity(a_) || EP_EF_is_infinity(b_)) && a_->u.EF.type == b_->u.EF.type);
    }
  }
  return false;
}

string EP_to_tuple_std_string(const EP*);
__EXPORT__ bool EP_to_tuple(const EP*, char*, int);
__EXPORT__ bool EP_to_tuple(const EP *ep, char *p, int maxlen) {
  auto c = EP_to_tuple_std_string(ep);
  if (c.size() < maxlen) {
    strcpy(p, c.c_str());
    return true;
  }
  return false;
}

string EP_to_tuple_std_string(const EP *ep) {
  stringstream ss;
  ss << "(";
  switch (ep->curve->type) {
  case EC_Type::FF:
    ss << to_std_string(ep->x) << ", ";
    ss << to_std_string(ep->y) << ", ";
    ss << to_std_string(ep->z) << ")";
    break;
  case EC_Type::EF:
    ss << EF_to_tuple_std_string(to_EF(ep->x)) << ", ";
    ss << EF_to_tuple_std_string(to_EF(ep->y)) << ", ";
    ss << EF_to_tuple_std_string(to_EF(ep->z)) << ")";
    break;
  }
  return ss.str();
}
__EXPORT__ bool EP_to_string(const EP *ep, char *p, int maxlen) {
  auto c = EP_to_std_string(ep);
  if (c.size() < maxlen) {
    strcpy(p, c.c_str());
    return true;
  }
  return false;
}

string EP_to_std_string(const EP *ep) {
  stringstream ss;
  ss << "ECPoint (" << to_std_string(ep->x)
     << ":" << to_std_string(ep->y)
     << ":" << to_std_string(ep->z)
     << ") over " << to_std_string(AS_OBJECT_CONST(ep->curve));
  switch (ep->curve->type) {
  case EC_Type::FF:
    ss << " on F_" << to_std_string(ep->u.FF.p);
    break;
  case EC_Type::EF:
    ss << " on F_" << to_std_string(ep->u.EF.modulo) << "/(";
    switch (ep->u.EF.type) {
    case IrreduciblePolynomialType::X2_1:
      ss << "x^2+1";
      break;
    case IrreduciblePolynomialType::X2_X_1:
      ss << "x^2+x+1";
      break;
    }
    ss << ")";
    break;
  }
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
  switch(ep->curve->type) {
  case EC_Type::FF:
    ret->u.FF.p = copy(ep->u.FF.p);
    break;
  case EC_Type::EF:
    ret->u.EF.modulo = copy(ep->u.EF.modulo);
    ret->u.EF.type = ep->u.EF.type;
    break;
  }
  return ret;
}

template <class T>
inline mpz_class modulo(T a, mpz_class b) {
  auto c = a % b;
  if (c < 0) {
    return c + b;
  }
  return c;
}

__EXPORT__ EP *EP_FF_add(const EP *a, const EP *b) {
  assert(is_same_type(AS_OBJECT_CONST(a), AS_OBJECT_CONST(b)));
  if (EP_FF_is_infinity(a)) {
    return to_EP_FF(copy(AS_OBJECT_CONST(b)));
  } else if (EP_FF_is_infinity(b)) {
    return to_EP_FF(copy(AS_OBJECT_CONST(a)));
  }
  auto Px = static_cast<mpz_class>(to_ZZ(to_FF(a->x)->x)->x);
  auto Py = static_cast<mpz_class>(to_ZZ(to_FF(a->y)->x)->x);
  auto Pz = static_cast<mpz_class>(to_ZZ(to_FF(a->z)->x)->x);
  auto Qx = to_ZZ(to_FF(b->x)->x)->x;
  auto Qy = to_ZZ(to_FF(b->y)->x)->x;
  auto Qz = to_ZZ(to_FF(b->z)->x)->x;
  auto p = to_ZZ(a->u.FF.p)->x;
  if (equals(AS_OBJECT_CONST(a), AS_OBJECT_CONST(b))) { // Point doubling
    auto X = Px;
    auto Y = Py;
    auto Z = Pz;
    auto u = modulo((3 * X * X)%p + (to_ZZ(a->curve->a)->x * Z * Z)%p, p);
    auto v = modulo(Y * Z, p);
    auto a = modulo(Y * v, p);
    auto w = modulo(u * u - 8 * X * a, p);
    auto Rx = FF_create_from_mpz_class(2 * v * w, p);
    auto Ry = FF_create_from_mpz_class(u * (4 * X * a - w) - 8 * a * a, p);
    auto Rz = FF_create_from_mpz_class(8 * v * v * v,  p);
    auto ret = EP_FF_create_with_FF(b->curve, Rx, Ry, Rz);
    destroy(AS_OBJECT(Rx));
    destroy(AS_OBJECT(Ry));
    destroy(AS_OBJECT(Rz));
    return ret;
  } else {
    auto u = modulo((Qy * Pz)%p - (Py * Qz)%p, p);
    auto v = modulo((Qx * Pz)%p - (Px * Qz)%p, p);
    auto v2 = modulo(v * v, p);
    auto v3 = modulo(v2 * v, p);
    auto w = modulo((((u * u)%p * Pz * Qz)%p - v3 - (2 * v2 * Px * Qz)%p), p);
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

__EXPORT__ EP *EP_EF_add(const EP *a, const EP *b) {
  assert(is_same_type(AS_OBJECT_CONST(a), AS_OBJECT_CONST(b)));
  if (EP_EF_is_infinity(a)) {
    return to_EP_EF(copy(AS_OBJECT_CONST(b)));
  } else if (EP_EF_is_infinity(b)) {
    return to_EP_EF(copy(AS_OBJECT_CONST(a)));
  }
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
  switch (poly) {
  case IrreduciblePolynomialType::X2_1:
    {
#define MUL_RE(x1, x2, y1, y2) (x1 * y1 - x2 * y2)
#define MUL_IM(x1, x2, y1, y2) (x2 * y1 + x1 * y2)
#define POW2_RE(x, y) (x * x - y * y)
#define POW2_IM(x, y) (2 * x * y)
#define SUB_RE(x1, x2, y1, y2) (x1 - y1)
#define SUB_IM(x1, x2, y1, y2) (x2 - y2)
      if (equals(AS_OBJECT_CONST(a), AS_OBJECT_CONST(b))) { // Point doubling
        // u = 3x^2+Az^2
        auto u1 = modulo(3 * POW2_RE(Px1, Px2) + A * POW2_RE(Pz1, Pz2), p);
        auto u2 = modulo(3 * POW2_IM(Px1, Px2) + A * POW2_IM(Pz1, Pz2), p);
        // v = yz
        auto v1 = modulo(MUL_RE(Py1, Py2, Pz1, Pz2), p);
        auto v2 = modulo(MUL_IM(Py1, Py2, Pz1, Pz2), p);
        // a = yv
        auto alpha1 = modulo(MUL_RE(v1, v2, Py1, Py2), p);
        auto alpha2 = modulo(MUL_IM(v1, v2, Py1, Py2), p);
        // w = u^2 - 8xa
        auto w1 = modulo(POW2_RE(u1, u2) - 8 * MUL_RE(Px1, Px2, alpha1, alpha2), p);
        auto w2 = modulo(POW2_IM(u1, u2) - 8 * MUL_IM(Px1, Px2, alpha1, alpha2), p);
        // Rx = 2vw
        auto Rx = EF_create_from_mpz_class(
            modulo(2 * MUL_RE(v1, v2, w1, w2), p),
            modulo(2 * MUL_IM(v1, v2, w1, w2), p), p, poly);
        // ax = ax
        auto ax1 = modulo(MUL_RE(alpha1, alpha2, Px1, Px2), p);
        auto ax2 = modulo(MUL_IM(alpha1, alpha2, Px1, Px2), p);
        // gamma = 4ax - w
        auto gamma1 = modulo(4*ax1 - w1, p);
        auto gamma2 = modulo(4*ax2 - w2, p);
        // Ry = u*gamma - 8a^2
        auto Ry = EF_create_from_mpz_class(
            modulo(MUL_RE(u1, u2, gamma1, gamma2) - 8 * POW2_RE(alpha1, alpha2), p),
            modulo(MUL_IM(u1, u2, gamma1, gamma2) - 8 * POW2_IM(alpha1, alpha2), p), p, poly);
        // v_2 = v^2
        auto v_2_1 = modulo(POW2_RE(v1, v2), p);
        auto v_2_2 = modulo(POW2_IM(v1, v2), p);
        // Rz = 8v^3
        auto Rz = EF_create_from_mpz_class(
            modulo(8*MUL_RE(v1, v2, v_2_1, v_2_2), p),
            modulo(8*MUL_IM(v1, v2, v_2_1, v_2_2), p), p, poly);
        auto ret = EP_EF_create_with_EF(b->curve, Rx, Ry, Rz);
        destroy(AS_OBJECT(Rx));
        destroy(AS_OBJECT(Ry));
        destroy(AS_OBJECT(Rz));
        return ret;
      } else { // Point addition
        auto u1 = modulo(MUL_RE(Qy1, Qy2, Pz1, Pz2) - MUL_RE(Py1, Py2, Qz1, Qz2), p);
        auto u2 = modulo(MUL_IM(Qy1, Qy2, Pz1, Pz2) - MUL_IM(Py1, Py2, Qz1, Qz2), p);
        auto v1 = modulo(MUL_RE(Qx1, Qx2, Pz1, Pz2) - MUL_RE(Px1, Px2, Qz1, Qz2), p);
        auto v2 = modulo(MUL_IM(Qx1, Qx2, Pz1, Pz2) - MUL_IM(Px1, Px2, Qz1, Qz2), p);
        auto v_2_1 = modulo(MUL_RE(v1, v2, v1, v2), p);
        auto v_2_2 = modulo(MUL_IM(v1, v2, v1, v2), p);
        auto v_3_1 = modulo(MUL_RE(v1, v2, v_2_1, v_2_2), p);
        auto v_3_2 = modulo(MUL_IM(v1, v2, v_2_1, v_2_2), p);

        auto g1 = modulo(MUL_RE(Qz1, Qz2, Pz1, Pz2), p);
        auto g2 = modulo(MUL_IM(Qz1, Qz2, Pz1, Pz2), p);
        auto ru1 = modulo(MUL_RE(u1, u2, u1, u2), p);
        auto ru2 = modulo(MUL_IM(u1, u2, u1, u2), p);
        auto r1 = modulo(MUL_RE(ru1, ru2, g1, g2), p);
        auto r2 = modulo(MUL_IM(ru1, ru2, g1, g2), p);
        auto w_1_1 = modulo(SUB_RE(r1, r2, v_3_1, v_3_2), p);
        auto w_1_2 = modulo(SUB_IM(r1, r2, v_3_1, v_3_2), p);
        auto s_1 = modulo(MUL_RE(Px1, Px2, Qz1, Qz2), p);
        auto s_2 = modulo(MUL_IM(Px1, Px2, Qz1, Qz2), p);
        auto w_2_1 = modulo(2*MUL_RE(s_1, s_2, v_2_1, v_2_2), p);
        auto w_2_2 = modulo(2*MUL_IM(s_1, s_2, v_2_1, v_2_2), p);
        auto w1 = modulo(w_1_1 - w_2_1, p);
        auto w2 = modulo(w_1_2 - w_2_2, p);

        auto Rx = EF_create_from_mpz_class(
            modulo(MUL_RE(v1, v2, w1, w2), p),
            modulo(MUL_IM(v1, v2, w1, w2), p), p, poly);

        auto Ry_1_1 = modulo(MUL_RE(s_1, s_2, v_2_1, v_2_2) - w1, p);
        auto Ry_1_2 = modulo(MUL_IM(s_1, s_2, v_2_1, v_2_2) - w2, p);
        auto t_1    = modulo(MUL_RE(Py1, Py2, Qz1, Qz2), p);
        auto t_2    = modulo(MUL_IM(Py1, Py2, Qz1, Qz2), p);
        auto Ry_2_1 = modulo(MUL_RE(t_1, t_2, v_3_1, v_3_2), p);
        auto Ry_2_2 = modulo(MUL_IM(t_1, t_2, v_3_1, v_3_2), p);
        auto Ry_3_1 = modulo(MUL_RE(Ry_1_1, Ry_1_2, u1, u2), p);
        auto Ry_3_2 = modulo(MUL_IM(Ry_1_1, Ry_1_2, u1, u2), p);

        auto Ry = EF_create_from_mpz_class(
            modulo(SUB_RE(Ry_3_1, Ry_3_2, Ry_2_1, Ry_2_2), p),
            modulo(SUB_IM(Ry_3_1, Ry_3_2, Ry_2_1, Ry_2_2), p), p, poly);

        auto Rz = EF_create_from_mpz_class(
            modulo(MUL_RE(v_3_1, v_3_2, g1, g2), p),
            modulo(MUL_IM(v_3_1, v_3_2, g1, g2), p), p, poly);

        auto ret = EP_EF_create_with_EF(b->curve, Rx, Ry, Rz);
        destroy(AS_OBJECT(Rx));
        destroy(AS_OBJECT(Ry));
        destroy(AS_OBJECT(Rz));
        return ret;
    }
#undef MUL_RE
#undef MUL_IM
#undef POW2_RE
#undef POW2_IM
#undef SUB_RE
#undef SUB_IM
  }
    break;
  case IrreduciblePolynomialType::X2_X_1:
    {
#define MUL_RE(x1, x2, y1, y2) (x1 * y1 - x2 * y2)
#define MUL_IM(x1, x2, y1, y2) (x2 * y1 + x1 * y2 - x2 * y2)
#define POW2_RE(x, y) (x * x - y * y)
#define POW2_IM(x, y) (2 * x * y - y * y)
#define SUB_RE(x1, x2, y1, y2) (x1 - y1)
#define SUB_IM(x1, x2, y1, y2) (x2 - y2)
      if (equals(AS_OBJECT_CONST(a), AS_OBJECT_CONST(b))) { // Point doubling
        // u = 3x^2+Az^2
        auto u1 = modulo(3 * POW2_RE(Px1, Px2) + A * POW2_RE(Pz1, Pz2), p);
        auto u2 = modulo(3 * POW2_IM(Px1, Px2) + A * POW2_IM(Pz1, Pz2), p);
        // v = yz
        auto v1 = modulo(MUL_RE(Py1, Py2, Pz1, Pz2), p);
        auto v2 = modulo(MUL_IM(Py1, Py2, Pz1, Pz2), p);
        // a = yv
        auto alpha1 = modulo(MUL_RE(v1, v2, Py1, Py2), p);
        auto alpha2 = modulo(MUL_IM(v1, v2, Py1, Py2), p);
        // w = u^2 - 8xa
        auto w1 = modulo(POW2_RE(u1, u2) - 8 * MUL_RE(Px1, Px2, alpha1, alpha2), p);
        auto w2 = modulo(POW2_IM(u1, u2) - 8 * MUL_IM(Px1, Px2, alpha1, alpha2), p);
        // Rx = 2vw
        auto Rx = EF_create_from_mpz_class(
            modulo(2 * MUL_RE(v1, v2, w1, w2), p),
            modulo(2 * MUL_IM(v1, v2, w1, w2), p), p, poly);
        // ax = ax
        auto ax1 = modulo(MUL_RE(alpha1, alpha2, Px1, Px2), p);
        auto ax2 = modulo(MUL_IM(alpha1, alpha2, Px1, Px2), p);
        // gamma = 4ax - w
        auto gamma1 = modulo(4*ax1 - w1, p);
        auto gamma2 = modulo(4*ax2 - w2, p);
        // Ry = u*gamma - 8a^2
        auto Ry = EF_create_from_mpz_class(
            modulo(MUL_RE(u1, u2, gamma1, gamma2) - 8 * POW2_RE(alpha1, alpha2), p),
            modulo(MUL_IM(u1, u2, gamma1, gamma2) - 8 * POW2_IM(alpha1, alpha2), p), p, poly);
        // v_2 = v^2
        auto v_2_1 = modulo(POW2_RE(v1, v2), p);
        auto v_2_2 = modulo(POW2_IM(v1, v2), p);
        // Rz = 8v^3
        auto Rz = EF_create_from_mpz_class(
            modulo(8*MUL_RE(v1, v2, v_2_1, v_2_2), p),
            modulo(8*MUL_IM(v1, v2, v_2_1, v_2_2), p), p, poly);
        auto ret = EP_EF_create_with_EF(b->curve, Rx, Ry, Rz);
        destroy(AS_OBJECT(Rx));
        destroy(AS_OBJECT(Ry));
        destroy(AS_OBJECT(Rz));
        return ret;
      } else { // Point addition
        auto u1 = modulo(MUL_RE(Qy1, Qy2, Pz1, Pz2) - MUL_RE(Py1, Py2, Qz1, Qz2), p);
        auto u2 = modulo(MUL_IM(Qy1, Qy2, Pz1, Pz2) - MUL_IM(Py1, Py2, Qz1, Qz2), p);
        auto v1 = modulo(MUL_RE(Qx1, Qx2, Pz1, Pz2) - MUL_RE(Px1, Px2, Qz1, Qz2), p);
        auto v2 = modulo(MUL_IM(Qx1, Qx2, Pz1, Pz2) - MUL_IM(Px1, Px2, Qz1, Qz2), p);
        auto v_2_1 = modulo(MUL_RE(v1, v2, v1, v2), p);
        auto v_2_2 = modulo(MUL_IM(v1, v2, v1, v2), p);
        auto v_3_1 = modulo(MUL_RE(v1, v2, v_2_1, v_2_2), p);
        auto v_3_2 = modulo(MUL_IM(v1, v2, v_2_1, v_2_2), p);

        auto g1 = modulo(MUL_RE(Qz1, Qz2, Pz1, Pz2), p);
        auto g2 = modulo(MUL_IM(Qz1, Qz2, Pz1, Pz2), p);
        auto ru1 = modulo(MUL_RE(u1, u2, u1, u2), p);
        auto ru2 = modulo(MUL_IM(u1, u2, u1, u2), p);
        auto r1 = modulo(MUL_RE(ru1, ru2, g1, g2), p);
        auto r2 = modulo(MUL_IM(ru1, ru2, g1, g2), p);
        auto w_1_1 = modulo(SUB_RE(r1, r2, v_3_1, v_3_2), p);
        auto w_1_2 = modulo(SUB_IM(r1, r2, v_3_1, v_3_2), p);
        auto s_1 = modulo(MUL_RE(Px1, Px2, Qz1, Qz2), p);
        auto s_2 = modulo(MUL_IM(Px1, Px2, Qz1, Qz2), p);
        auto w_2_1 = modulo(2*MUL_RE(s_1, s_2, v_2_1, v_2_2), p);
        auto w_2_2 = modulo(2*MUL_IM(s_1, s_2, v_2_1, v_2_2), p);
        auto w1 = modulo(w_1_1 - w_2_1, p);
        auto w2 = modulo(w_1_2 - w_2_2, p);

        auto Rx = EF_create_from_mpz_class(
            modulo(MUL_RE(v1, v2, w1, w2), p),
            modulo(MUL_IM(v1, v2, w1, w2), p), p, poly);

        auto Ry_1_1 = modulo(MUL_RE(s_1, s_2, v_2_1, v_2_2) - w1, p);
        auto Ry_1_2 = modulo(MUL_IM(s_1, s_2, v_2_1, v_2_2) - w2, p);
        auto t_1    = modulo(MUL_RE(Py1, Py2, Qz1, Qz2), p);
        auto t_2    = modulo(MUL_IM(Py1, Py2, Qz1, Qz2), p);
        auto Ry_2_1 = modulo(MUL_RE(t_1, t_2, v_3_1, v_3_2), p);
        auto Ry_2_2 = modulo(MUL_IM(t_1, t_2, v_3_1, v_3_2), p);
        auto Ry_3_1 = modulo(MUL_RE(Ry_1_1, Ry_1_2, u1, u2), p);
        auto Ry_3_2 = modulo(MUL_IM(Ry_1_1, Ry_1_2, u1, u2), p);

        auto Ry = EF_create_from_mpz_class(
            modulo(SUB_RE(Ry_3_1, Ry_3_2, Ry_2_1, Ry_2_2), p),
            modulo(SUB_IM(Ry_3_1, Ry_3_2, Ry_2_1, Ry_2_2), p), p, poly);

        auto Rz = EF_create_from_mpz_class(
            modulo(MUL_RE(v_3_1, v_3_2, g1, g2), p),
            modulo(MUL_IM(v_3_1, v_3_2, g1, g2), p), p, poly);

        auto ret = EP_EF_create_with_EF(b->curve, Rx, Ry, Rz);
        destroy(AS_OBJECT(Rx));
        destroy(AS_OBJECT(Ry));
        destroy(AS_OBJECT(Rz));
        return ret;
    }
#undef MUL_RE
#undef MUL_IM
#undef POW2_RE
#undef POW2_IM
#undef SUB_RE
#undef SUB_IM
    }
    break;
  }
  return nullptr;
}

EP *EP_get_Infinity(ObjectType type, const EC *curve, const EP *orig = nullptr) {
  if (orig != nullptr) {
    switch (type) {
    case ObjectType::EP_FF:
      return EP_FF_create(curve, "0", "1", "0", to_std_string(orig->u.FF.p).c_str());
    case ObjectType::EP_EF:
      const char *pol;
      switch (orig->u.EF.type) {
      case IrreduciblePolynomialType::X2_1:
        pol = "x^2+1";
        break;
      case IrreduciblePolynomialType::X2_X_1:
        pol = "x^2+x+1";
        break;
      }
      return EP_EF_create(curve, "0", "0", "1", "0", "0", "0", to_std_string(orig->u.EF.modulo).c_str(), pol);
    default:
      return nullptr;
    }
  } else {
    switch (type) {
    case ObjectType::EP_FF:
      return EP_FF_create(curve, "0", "1", "0", "2");
    case ObjectType::EP_EF:
      return EP_EF_create(curve, "0", "0", "1", "0", "0", "0", "2", "x^2+1");
    default:
      return nullptr;
    }
  }
}

__EXPORT__ EP *EP_mul(const EP *point, const ZZ *rhs) {
  auto m = rhs->x;
  if (m == 0) {
    return EP_get_Infinity(point->objtype, point->curve, point);
  } else if (m == 1) {
    return to_EP_force(copy(AS_OBJECT_CONST(point)));
  } else if (m == 2) {
    return to_EP_force(add(AS_OBJECT_CONST(point), AS_OBJECT_CONST(point)));
  }
  auto P = copy(AS_OBJECT_CONST(point));
  auto Q = AS_OBJECT(EP_get_Infinity(point->objtype, point->curve, point));
  while (m != 0) {
    if ((m & 1) == 1) {
      auto t = Q;
      Q = add(Q, P);
      destroy(t);
    }
    auto t = P;
    P = add(P, P);
    destroy(t);
    m >>= 1;
  }
  destroy(P);
  return to_EP_force(Q);
}
