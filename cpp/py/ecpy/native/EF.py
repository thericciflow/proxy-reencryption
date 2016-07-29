from library import *
import ast

class EF(object):
  def __init__(s, p, poly):
    s.p = p
    s.poly = poly
    s.ptr = lib.EF_create(str(p), poly)

  def __str__(s):
    b = create_string_buffer(1024)
    lib.EF_to_string(s.ptr, b, 1024)
    return b.value

  def add(s, ret, a, b):
    lib.EF_add(s.ptr, ret.ptr, a.ptr, b.ptr)

  def sub(s, ret, a, b):
    lib.EF_sub(s.ptr, ret.ptr, a.ptr, b.ptr)

  def mul(s, ret, a, b):
    lib.EF_mul(s.ptr, ret.ptr, a.ptr, b.ptr)

  def div(s, ret, a, b):
    lib.EF_div(s.ptr, ret.ptr, a.ptr, b.ptr)

  def pow(s, ret, a, b):
    lib.EF_pow(s.ptr, ret.ptr, a.ptr, b.ptr)

  def __del__(s):
    lib.EF_delete(s.ptr)

class EF_elem(object):
  def __init__(s, u, v):
    s.ptr = lib.EF_elem_create(str(u), str(v))

  def __str__(s):
    b = create_string_buffer(1024)
    lib.EF_elem_to_string(s.ptr, b, 1024)
    return b.value

  def __iter__(s):
    r = str(s).lstrip("EF_elem")
    return iter(ast.literal_eval(r))

  def __del__(s):
    lib.EF_elem_delete(s.ptr)

