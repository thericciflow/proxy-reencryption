from library import *

class FF(Structure):
  def __init__(s, p):
    s.ptr = lib.FF_create(str(p))

  def add(s, lhs, rhs):
    return FF_elem(s, ptr=lib.FF_add(s.ptr, lhs, rhs))

  def sub(s, lhs, rhs):
    return FF_elem(s, ptr=lib.FF_sub(s.ptr, lhs, rhs))

  def mul(s, lhs, rhs):
    return FF_elem(s, ptr=lib.FF_mul(s.ptr, lhs, rhs))

  def div(s, lhs, rhs):
    return FF_elem(s, ptr=lib.FF_div(s.ptr, lhs, rhs))

  def pow(s, lhs, rhs):
    return FF_elem(s, ptr=lib.FF_pow(s.ptr, lhs, rhs))

  def __str__(s):
    d = create_string_buffer(1024)
    lib.FF_to_string(s.ptr, d, 1024)
    return d.value

  def __repr__(s):
    return "FF(%s)" % (str(s)[2:])

  def __del__(s):
    lib.FF_delete(s.ptr)

class FF_elem(Structure):
  def __init__(s, ff, v=0, ptr=None):
    if ptr is None:
      s.ff = ff
      s.ptr = lib.FF_elem_create(str(v))
    else:
      s.ff = ff
      s.ptr = ptr

  def __add__(s, rhs):
    return s.ff.add(s.ptr, rhs.ptr)

  def __sub__(s, rhs):
    return s.ff.sub(s.ptr, rhs.ptr)

  def __mul__(s, rhs):
    return s.ff.mul(s.ptr, rhs.ptr)

  def __div__(s, rhs):
    return s.ff.div(s.ptr, rhs.ptr)

  def __pow__(s, rhs):
    return s.ff.pow(s.ptr, rhs.ptr)

  def __int__(s):
    return int(str(s))

  def __str__(s):
    d = create_string_buffer(1024)
    lib.FF_elem_to_string(s.ptr, d, 1024)
    return d.value

  def __repr__(s):
    return "FF_elem(%r, %s)" % (s.ff, s)

  def __del__(s):
    lib.FF_elem_delete(s.ptr)

LPFF = POINTER(FF)
LPFE = POINTER(FF_elem)
