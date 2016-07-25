#include "ecpy_native.h"
#include "EC_impl.h"
#include "ZZ_impl.h"
using namespace std;
using namespace g_object;

MAKE_FUNC_TABLE(_ec_ff_func, EC_destroy, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, EC_is_equals, EC_is_same_type, EC_to_std_string, nullptr);
MAKE_FUNC_TABLE(_ec_ef_func, EC_destroy, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, EC_is_equals, EC_is_same_type, EC_to_std_string, nullptr);

// y^2 = x^3 + ax + b
__EXPORT__ EC *EC_create(const char *a, const char *b, const char *type) {
  EC *ec = new EC;
  ec->a = AS_OBJECT(ZZ_create(a));
  ec->b = AS_OBJECT(ZZ_create(b));
  auto t = string(type);
  transform(t.begin(), t.end(), t.begin(), ::toupper);
  if (t == "FF") {
    ec->type = EC_Type::FF;
    ec->obj_type = ObjectType::EC_FF;
    ec->functions = _ec_ff_func;
  } else if (t == "EF") {
    ec->type = EC_Type::EF;
    ec->obj_type = ObjectType::EC_EF;
    ec->functions = _ec_ef_func;
  } else {
    throw invalid_argument("invalid ec type");
  }
  return ec;
}

__EXPORT__ void EC_destroy(EC *ec) {
  destroy(ec->a);
  destroy(ec->b);
  ec->obj_type = ObjectType::FREE;
  delete ec;
}

__EXPORT__ bool EC_to_string(const EC *ec, char *p, int maxlen) {
  auto c = EC_to_std_string(ec);
  if (c.size() < maxlen) {
    strcpy(p, c.c_str());
    return true;
  }
  return false;
}


string EC_to_std_string(const EC *ec) {
  stringstream ss;
  ss << "Elliptic Curve on ";
  switch (ec->type) {
  case EC_Type::FF:
    ss << "Finite Field";
    break;
  case EC_Type::EF:
    ss << "Extended Finite Field";
    break;
  }
  ss << ": y^2 = x^3 + ";
  auto a = to_ZZ(ec->a)->x;
  auto b = to_ZZ(ec->b)->x;
  if (a > 1) {
    ss << to_std_string(ec->a);
  }
  if (a >= 1) {
    ss << "x";
  }
  if (b > 0) {
    if (a != 0) {
      ss << " + ";
    }
    ss << b;
  }
  return ss.str();
}

__EXPORT__ bool EC_is_equals(const EC *a, const EC *b) {
  return equals(a->a, b->a) && equals(a->b, b->b) && a->type == b->type;
}

bool EC_is_same_type(const g_object_t *a, const g_object_t *b) {
  return a->type == b->type && (a->type == ObjectType::EC_FF) ?
    (to_EC_FF(a)->type == to_EC_FF(b)->type) :
        (
         (a->type == ObjectType::EC_EF) ?
            (to_EC_EF(a)->type == to_EC_EF(b)->type) :
        false
        );
}
