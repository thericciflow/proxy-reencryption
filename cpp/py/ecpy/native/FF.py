from library import *

class FF(object):
  def __init__(s, p):
    s.p = p
    s.ptr = lib.FF_create(str(p))

  def __str__(s):
    b = create_string_buffer(1024)
    lib.FF_to_string(s.ptr, b, 1024)
    return b.value

  def add(s, ret, a, b):
    lib.FF_add(s.ptr, ret.ptr, a.ptr, b.ptr)

  def sub(s, ret, a, b):
    lib.FF_sub(s.ptr, ret.ptr, a.ptr, b.ptr)

  def mul(s, ret, a, b):
    lib.FF_mul(s.ptr, ret.ptr, a.ptr, b.ptr)

  def div(s, ret, a, b):
    lib.FF_div(s.ptr, ret.ptr, a.ptr, b.ptr)

  def pow(s, ret, a, b):
    lib.FF_pow(s.ptr, ret.ptr, a.ptr, b.ptr)

  def __del__(s):
    lib.FF_delete(s.ptr)

class FF_elem(object):
  def __init__(s, v):
    s.ptr = lib.FF_elem_create(str(v))
    s.v = v

  def __str__(s):
    b = create_string_buffer(1024)
    lib.FF_elem_to_string(s.ptr, b, 1024)
    return b.value

  def __int__(s):
    return int(str(s))

  def __del__(s):
    lib.FF_elem_delete(s.ptr)
