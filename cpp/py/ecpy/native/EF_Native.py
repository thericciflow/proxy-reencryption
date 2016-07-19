from library import *
from ZZ_Native import *
from weakref import ref as weakref

class EF_Native(Structure):
  _fields_ = [("x", LPZZ), ("y", LPZZ), ("modulo", LPZZ)]

  """
  def __str__(s):
    d = create_string_buffer(1024)
    lib.FF_to_string(LPFF(s), d, 1024)
    return d.value

  def __repr__(s):
    d = create_string_buffer(1024)
    lib.FF_to_raw_string(LPFF(s), d, 1024)
    return d.value
  def __eq__(s, rhs):
    return lib.FF_is_equals(LPFF(s), LPFF(rhs))
  """

  def __del__(s):
    import sys
    lib.EF_destroy(LPEF(s))

LPEF = POINTER(EF_Native)

def EF_create(x, y, modulo, poly):
  t = cast(lib.EF_create(str(x), str(p)), LPEF).contents
  return t

