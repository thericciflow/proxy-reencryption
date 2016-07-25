from library import *
from ZZ_Native import *
from weakref import ref as weakref

class EF_Native(Structure):
  def __add__(s, a):
    if not isinstance(a, EF_Native):
      raise TypeError("%r must be EF_Native instance." % a)
    return cast(lib.EF_add(LPEF(s), LPEF(a)), LPEF).contents

  def __mul__(s, a):
    if not isinstance(a, EF_Native):
      raise TypeError("%r must be EF_Native instance." % a)
    return cast(lib.EF_mul(LPEF(s), LPEF(a)), LPEF).contents

  def inv(s):
    return cast(lib.EF_inv(LPEF(s)), LPEF).contents

  def __div__(s, a):
    if not isinstance(a, EF_Native):
      raise TypeError("%r must be EF_Native instance." % a)
    return a.inv() * s

  def __neg__(s):
    return cast(lib.EF_neg(LPEF(s)), LPEF).contents

  def __str__(s):
    d = create_string_buffer(1024)
    lib.EF_to_string(LPEF(s), d, 1024)
    return d.value

  def __repr__(s):
    d = create_string_buffer(1024)
    lib.EF_to_string(LPEF(s), d, 1024)
    return d.value

  def __eq__(s, rhs):
    return lib.EF_is_equals(LPEF(s), LPEF(rhs))

  def __del__(s):
    lib.EF_destroy(LPEF(s))

LPEF = POINTER(EF_Native)

def EF_create(x, y, modulo, poly):
  t = cast(lib.EF_create(str(x), str(y), str(modulo), poly), LPEF).contents
  return t

