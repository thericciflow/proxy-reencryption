from library import *

class ZZ_Native(Structure):
  _fields_ = [("x", c_void_p)]

  def __add__(s, a):
    if not isinstance(a, ZZ_Native):
      a = ZZ_create(a)
    return cast(lib.ZZ_add(LPZZ(s), LPZZ(a)), LPZZ).contents

  def __sub__(s, a):
    if not isinstance(a, ZZ_Native):
      a = ZZ_create(a)
    return cast(lib.ZZ_add(LPZZ(s), LPZZ(-a)), LPZZ).contents

  def __neg__(s):
    return cast(lib.ZZ_neg(LPZZ(s)), LPZZ).contents

  def __mul__(s, a):
    if not isinstance(a, ZZ_Native):
      a = ZZ_create(a)
    return cast(lib.ZZ_mul(LPZZ(s), LPZZ(a)), LPZZ).contents

  def __div__(s, a):
    if not isinstance(a, ZZ_Native):
      a = ZZ_create(a)
    return cast(lib.ZZ_div(LPZZ(s), LPZZ(a)), LPZZ).contents

  def __mod__(s, a):
    if not isinstance(a, ZZ_Native):
      a = ZZ_create(a)
    return cast(lib.ZZ_mod(LPZZ(s), LPZZ(a)), LPZZ).contents

  def __str__(s):
    d = create_string_buffer(1024)
    lib.ZZ_to_string(LPZZ(s), d, 1024)
    return d.value

  def __repr__(s):
    return "ZZ_Native('%s')" % str(s)

  def __eq__(s, rhs):
    return lib.ZZ_is_equals(LPZZ(s), LPZZ(rhs))

  def __del__(s):
    import sys
    sys.stderr.write("Prepare Delete: %s\n" % s.__class__.__name__)
    sys.stderr.write("Delete: %r..." % s)
    lib.ZZ_destroy(LPZZ(s))
    sys.stderr.write("\n")

LPZZ = POINTER(ZZ_Native)

def ZZ_create(x):
  return cast(lib.ZZ_create(str(x)), LPZZ).contents

