from library import *
from EC_Native import *
from EF_Native import LPEF
from ZZ_Native import ZZ_create, LPZZ
class EP_Native(Structure):
  def __add__(s, a):
    if not isinstance(a, EP_Native):
      raise TypeError("%r must be EP_Native instance." % a)
    return cast(lib.EP_add(LPEP(s), LPEP(a)), LPEP).contents

  def __neg__(s):
    return cast(lib.EP_neg(LPEP(s)), LPEP).contents

  def line_coeff(s, a):
    if not isinstance(a, EP_Native):
      raise TypeError("%r must be EP_Native instance." % a)
    d = lib.EP_line_coeff(LPEP(s), LPEP(a))
    if s.type == 0:
      return cast(d, LPZZ).contents
    elif s.type == 1:
      return cast(d, LPEF).contents

  def __mul__(s, a):
    d = ZZ_create(a)
    return cast(lib.EP_mul(LPEP(s), LPZZ(d)), LPEP).contents

  def __str__(s):
    d = create_string_buffer(2048)
    lib.EP_to_string(LPEP(s), d, 2048)
    return d.value

  def __repr__(s):
    return "EP_Native('%s')" % str(s)

  def __eq__(s, rhs):
    return lib.EP_equals(LPEP(s), LPEP(rhs))

  def __del__(s):
    import sys
    lib.EP_destroy(LPEP(s))

  def tuple(s):
    d = create_string_buffer(1024)
    lib.EP_to_tuple(LPEP(s), d, 1024)
    return eval(d.value)

LPEP = POINTER(EP_Native)

def EP_FF_create(curve, x, y, z, p):
  d = cast(lib.EP_FF_create(LPEC(curve), str(x), str(y), str(z), str(p)), LPEP).contents
  d.type = 0
  return d

def EP_EF_create(curve, x1, x2, y1, y2, z1, z2, modulo, poly):
  d = cast(lib.EP_EF_create(LPEC(curve), str(x1), str(x2), str(y1), str(y2), str(z1), str(z2), str(modulo), poly), LPEP).contents
  d.type = 1
  return d

