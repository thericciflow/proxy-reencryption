from library import *

class FF(object):
  def __init__(s, p):
    s.p = p
    s.ptr = lib.FF_create(str(p))

  def __to_string(s, bufsize):
    b = create_string_buffer(bufsize)
    lib.FF_to_string(s.ptr, b, bufsize)
    b = b.value
    if len(b) == 0: # not enough buffer size
      return s.__to_string(2*bufsize)
    return b

  def __str__(s):
    return s.__to_string(1024)

  def add(s, ret, a, b):
    assert isinstance(ret, FF_elem) and isinstance(a, FF_elem) and isinstance(b, FF_elem)
    lib.FF_add(s.ptr, ret.ptr, a.ptr, b.ptr)

  def sub(s, ret, a, b):
    assert isinstance(ret, FF_elem) and isinstance(a, FF_elem) and isinstance(b, FF_elem)
    lib.FF_sub(s.ptr, ret.ptr, a.ptr, b.ptr)

  def mul(s, ret, a, b):
    assert isinstance(ret, FF_elem) and isinstance(a, FF_elem) and isinstance(b, FF_elem)
    lib.FF_mul(s.ptr, ret.ptr, a.ptr, b.ptr)

  def div(s, ret, a, b):
    assert isinstance(ret, FF_elem) and isinstance(a, FF_elem) and isinstance(b, FF_elem)
    lib.FF_div(s.ptr, ret.ptr, a.ptr, b.ptr)

  def pow(s, ret, a, b):
    assert isinstance(ret, FF_elem) and isinstance(a, FF_elem)
    lib.FF_pow(s.ptr, ret.ptr, a.ptr, str(b))

  def __del__(s):
    lib.FF_delete(s.ptr)

class FF_elem(object):
  def __init__(s, v):
    s.ptr = lib.FF_elem_create(str(v))
    s.v = v

  def __to_string(s, bufsize):
    b = create_string_buffer(bufsize)
    lib.FF_elem_to_string(s.ptr, b, bufsize)
    b = b.value
    if len(b) == 0: # not enough buffer size
      return s.__to_string(2*bufsize)
    return b

  def __str__(s):
    return s.__to_string(1024)

  def to_python(s):
    return int(str(s))

  def __del__(s):
    lib.FF_elem_delete(s.ptr)
