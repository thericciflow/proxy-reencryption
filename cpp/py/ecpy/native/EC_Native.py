from library import *
from ZZ_Native import *
from weakref import ref as weakref

class EC_Native(Structure):
  def __str__(s):
    d = create_string_buffer(1024)
    lib.EC_to_string(LPEC(s), d, 1024)
    return d.value

  def __repr__(s):
    d = create_string_buffer(1024)
    lib.EC_to_string(LPEC(s), d, 1024)
    return d.value

  def __eq__(s, rhs):
    return lib.EC_is_equals(LPEC(s), LPEC(rhs))

  def __del__(s):
    lib.EC_destroy(LPEC(s))

LPEC = POINTER(EC_Native)

def EC_create(a, b, type):
  t = cast(lib.EC_create(str(a), str(b), type), LPEC).contents
  return t

