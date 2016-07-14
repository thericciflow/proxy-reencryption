from ctypes import *

lib = cdll.LoadLibrary("libecpy_native.so")

class ZZ_Native(Structure):
  _fields_ = [("x", c_void_p)]

  def __add__(s, a):
    if not isinstance(a, ZZ_Native):
      a = ZZ_create(a)
    return cast(lib.ZZ_add(LPZZ(s), LPZZ(a)), LPZZ).contents

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

  def __str__(s):
    return cast(lib.ZZ_to_string(LPZZ(s)), c_char_p).value

  def __del__(s):
    print "destroy %s" % s
    lib.ZZ_destroy(LPZZ(s))

LPZZ = POINTER(ZZ_Native)

def ZZ_create(x):
  return cast(lib.ZZ_create(str(x)), LPZZ).contents

x = ZZ_create(22) * 0xdeadbeef
print str(x)
del x
