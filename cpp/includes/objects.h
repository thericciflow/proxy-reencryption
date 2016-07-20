#pragma once
#include "ecpy_native.h"
#include <assert.h>

#define MAKE_FUNC_TABLE(var, \
    _destroy, _add, _neg, _mul, _div, _inv, \
    _mod, _equals, _is_same_type, _to_std_string, _copy) \
  const static methods_t _##var = {\
    .destroy = (fpop_t<void>) _destroy, \
    .add = (fpbinop_t<>) _add , \
    .neg = (fpop_t<>) _neg , \
    .mul = (fpbinop_t<>) _mul , \
    .div = (fpbinop_t<>) _div , \
    .inv = (fpop_t<>) _inv , \
    .mod = (fpbinop_t<>) _mod, \
    .equals = (fpbinop_t<bool>) _equals , \
    .is_same_type = (fpbinop_t<bool>) _is_same_type , \
    .to_std_string = (fpop_t<std::string>) _to_std_string ,\
    .copy = (fpop_t<>) _copy , \
  }; const static methods_t* var = &_##var

#define FTBL(obj) (((g_object_t*)obj)->functions)

#define INVOKE(instance, func, ...) FTBL(instance)->func((instance) , __VA_ARGS__)
#define INVOKESIMPLE(instance, func) FTBL(instance)->func(instance)

#define AS_OBJECT(x) reinterpret_cast<g_object_t*>(x)

struct g_object_t;
using __vp = void*;
template <class T=g_object_t*>
using fpbinop_t = T (*)(const void *, const void *);
template <class T=g_object_t*>
using fpop_t = T (*)(const void *);

enum struct ObjectType : unsigned int {
  ZZ,
  FF,
  EF,
  EC_FF,
  EC_EF,
  EP,
};

struct methods_t {
  fpop_t<void> destroy;
  fpbinop_t<> add;
  fpop_t<> neg;
  fpbinop_t<> mul;
  fpbinop_t<> div;
  fpop_t<> inv;
  fpbinop_t<> mod;
  fpbinop_t<bool> equals;
  fpbinop_t<bool> is_same_type;
  fpop_t<std::string> to_std_string;
  fpop_t<> copy;
};

struct g_object_t {
  const methods_t *functions;
  ObjectType type;
};

namespace g_object {

  using LPOBJ = g_object_t*;

#define BINOP(op, ret) inline constexpr ret op(const LPOBJ a, const LPOBJ b) { return INVOKE(a, op, b); }
#define OP(op, ret) inline ret op(const LPOBJ a) { return INVOKESIMPLE(a, op); }

  OP(destroy, void)
  BINOP(add, LPOBJ)
  OP(neg, LPOBJ)
  BINOP(mul, LPOBJ)
  BINOP(div, LPOBJ)
  OP(inv, LPOBJ)
  BINOP(mod, LPOBJ)
  BINOP(equals, bool)
  BINOP(is_same_type, bool)
  OP(to_std_string, std::string)
  OP(copy, LPOBJ)

#define MAKE_TO_TYPE(totype) constexpr totype *to_##totype (const g_object_t *ptr) { \
  return (ptr->type == ObjectType::totype) ? reinterpret_cast<totype *>(const_cast<g_object_t*>(ptr)) : throw std::logic_error("Invalid Pointer: ptr->type != ObjectType::" #totype);\
}

  MAKE_TO_TYPE(ZZ)
  MAKE_TO_TYPE(FF)
  MAKE_TO_TYPE(EF)
};
