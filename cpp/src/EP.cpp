#include "ecpy_native.h"
#include "EP_impl.h"
#include "EC_impl.h"

using namespace std;
using namespace g_object;

MAKE_FUNC_TABLE(_ep_ff_func, EP_destroy, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, EP_equals, EP_is_same_type, EP_to_std_string, EP_copy);
MAKE_FUNC_TABLE(_ep_ef_func, EP_destroy, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, EP_equals, EP_is_same_type, EP_to_std_string, EP_copy);

__EXPORT__ EP *EP_FF_create(EC *ec, const char *x, const char *y, const char *z, const char *p) {
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

__EXPORT__ EP *EP_EF_create(EC *ec, const char *x1, const char *x2, const char *y1, const char *y2, const char *z1, const char *z2, const char *modulo, const char *poly) {
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

__EXPORT__ void EP_destroy(const EP *ep) {
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
  delete ep;
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
      return is_same_type(AS_OBJECT(a_->curve), AS_OBJECT(b_->curve)) && equals(a_->u.FF.p, b_->u.FF.p);
    } else if (b->type == ObjectType::EP_EF) {
      auto a_ = to_EP_EF(const_cast<g_object_t*>(a));
      auto b_ = to_EP_EF(const_cast<g_object_t*>(b));
      return is_same_type(AS_OBJECT(a_->curve), AS_OBJECT(b_->curve)) && equals(a_->u.EF.modulo, b_->u.EF.modulo) && a_->u.EF.type == b_->u.EF.type;
    }
  }
  return false;
}

string EP_to_std_string(const EP *ep) {
  stringstream ss;
  ss << "ECPoint (" << to_std_string(ep->x)
     << ", " << to_std_string(ep->y)
     << ", " << to_std_string(ep->z)
     << ") over " << to_std_string(AS_OBJECT(ep->curve));
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
