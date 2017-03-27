from .library import *
import ast

class EF(object):
  def __init__(s, p, poly):
    s.p = p
    s.poly = poly
    s.ptr = lib.EF_create(to_char_ptr(str(p)), to_char_ptr(poly))

  def __to_string(s, bufsize):
    b = create_string_buffer(bufsize)
    lib.EF_to_string(s.ptr, b, bufsize)
    b = b.value
    if len(b) == 0: # not enough buffer size
      return s.__to_string(2*bufsize)
    return b

  def __str__(s):
    return s.__to_string(1024)

  def add(s, ret, a, b):
    assert isinstance(ret, EF_elem) and isinstance(a, EF_elem) and isinstance(b, EF_elem)
    lib.EF_add(s.ptr, ret.ptr, a.ptr, b.ptr)

  def sub(s, ret, a, b):
    assert isinstance(ret, EF_elem) and isinstance(a, EF_elem) and isinstance(b, EF_elem)
    lib.EF_sub(s.ptr, ret.ptr, a.ptr, b.ptr)

  def mul(s, ret, a, b):
    assert isinstance(ret, EF_elem) and isinstance(a, EF_elem) and isinstance(b, EF_elem)
    lib.EF_mul(s.ptr, ret.ptr, a.ptr, b.ptr)

  def div(s, ret, a, b):
    assert isinstance(ret, EF_elem) and isinstance(a, EF_elem) and isinstance(b, EF_elem)
    lib.EF_div(s.ptr, ret.ptr, a.ptr, b.ptr)

  def pow(s, ret, a, b):
    assert isinstance(ret, EF_elem) and isinstance(a, EF_elem)
    lib.EF_pow(s.ptr, ret.ptr, a.ptr, to_char_ptr(str(b)))

  def __del__(s):
    lib.EF_delete(s.ptr)

class EF_elem(object):
  def __init__(s, u, v):
    s.ptr = lib.EF_elem_create(to_char_ptr(str(u)), to_char_ptr(str(v)))

  def to_python(s):
    r = str(s).lstrip("EF_elem")
    return tuple(ast.literal_eval(r))

  def __to_string(s, bufsize):
    b = create_string_buffer(bufsize)
    lib.EF_elem_to_string(s.ptr, b, bufsize)
    b = b.value
    if len(b) == 0: # not enough buffer size
      return s.__to_string(2*bufsize)
    return b

  def __str__(s):
    return str(s.__to_string(1024).decode('us-ascii'))

  def __del__(s):
    lib.EF_elem_delete(s.ptr)

