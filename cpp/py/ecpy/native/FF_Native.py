from library import *
from ZZ_Native import *

class FF_Native(Structure):
  _fields_ = [("x", LPZZ), ("p", LPZZ)]

  def __add__(s, a):
    if not isinstance(a, FF_Native):
      a = FF_create(a, s.p)
    return cast(lib.FF_add(LPFF(s), LPFF(a)), LPFF).contents

  def __sub__(s, a):
    if not isinstance(a, FF_Native):
      a = FF_create(a, s.p)
    return cast(lib.FF_add(LPFF(s), LPFF(-a)), LPFF).contents

  def __neg__(s):
    return cast(lib.FF_neg(LPFF(s)), LPFF).contents

  def __mul__(s, a):
    if not isinstance(a, FF_Native):
      a = FF_create(a, s.p)
    return cast(lib.FF_mul(LPFF(s), LPFF(a)), LPFF).contents

  def __div__(s, a):
    if not isinstance(a, FF_Native):
      a = FF_create(a, s.p)
    return cast(lib.FF_div(LPFF(s), LPFF(a)), LPFF).contents

  def __mod__(s, a):
    if not isinstance(a, FF_Native):
      a = FF_create(a, s.p)
    return cast(lib.FF_mod(LPFF(s), LPFF(a)), LPFF).contents

  def __str__(s):
    return cast(lib.FF_to_string(LPFF(s)), c_char_p).value

  def __repr__(s):
    return "FF_Native('%s', '%s')" % (str(s.x.contents), str(s.p.contents))

  def __eq__(s, rhs):
    return lib.FF_is_equals(LPFF(s), LPFF(rhs))

  def __del__(s):
    lib.FF_destroy(LPFF(s))

LPFF = POINTER(FF_Native)

def FF_create(x, p):
  t = cast(lib.FF_create(str(x), str(p)), LPFF).contents
  print t.p.contents
  return t


