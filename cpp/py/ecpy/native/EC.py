from library import *
from FF import FF
from EF import EF
import ast

class EC(object):
  def __init__(s, base, a, b):
    s.base = base
    s.a = a
    s.b = b
    if isinstance(base, FF):
      s.ptr = lib.EC_FF_create(str(a), str(b), str(base.p))
      s.type = 1
    elif isinstance(base, EF):
      s.ptr = lib.EC_EF_create(str(a), str(b), str(base.p), base.poly)
      s.type = 2

  def __to_string(s, bufsize):
    b = create_string_buffer(bufsize)
    cond = {
        1 : lib.EC_FF_to_string,
        2 : lib.EC_EF_to_string
    }
    cond[s.type](s.ptr, b, bufsize)
    b = b.value
    if len(b) == 0: # not enough buffer size
      return s.__to_string(2*bufsize)
    return b

  def __str__(s):
    return s.__to_string(1024)

  def add(s, ret, a, b):
    assert isinstance(ret, EC_elem) and isinstance(a, EC_elem) and isinstance(b, EC_elem)
    cond = {
        1 : lib.EC_FF_add,
        2 : lib.EC_EF_add
    }
    cond[s.type](s.ptr, ret.ptr, a.ptr, b.ptr)

  def sub(s, ret, a, b):
    assert isinstance(ret, EC_elem) and isinstance(a, EC_elem) and isinstance(b, EC_elem)
    cond = {
        1 : lib.EC_FF_sub,
        2 : lib.EC_EF_sub
    }
    cond[s.type](s.ptr, ret.ptr, a.ptr, b.ptr)

  def mul(s, ret, a, b):
    assert isinstance(ret, EC_elem) and isinstance(a, EC_elem) and isinstance(b, EC_elem)
    cond = {
        1 : lib.EC_FF_mul,
        2 : lib.EC_EF_mul
    }
    cond[s.type](s.ptr, ret.ptr, a.ptr, str(b))

  def div(s, ret, a, b):
    raise NotImplementedError()

  def pow(s, ret, a, b):
    raise NotImplementedError()

  def __del__(s):
    cond = {
        1 : lib.EC_FF_delete,
        2 : lib.EC_EF_delete
    }
    cond[s.type](s.ptr)

class EC_elem(object):
  def __init__(s, curve, x, y, z):
    s.x = x
    s.y = y
    s.z = z
    s.curve = curve
    cond = {
      1 : lib.EC_elem_FF_create, 
      2 : lib.EC_elem_EF_create, 
    }
    s.ptr = cond[curve.type](str(x), str(y), str(z))

  def to_python(s):
    r = str(s).lstrip("EC_elem")
    return tuple(ast.literal_eval(r))

  def __to_string(s, bufsize):
    b = create_string_buffer(bufsize)
    cond = {
      1 : lib.EC_elem_FF_to_string, 
      2 : lib.EC_elem_EF_to_string, 
    }
    cond[s.curve.type](s.ptr, b, bufsize)
    b = b.value
    if len(b) == 0: # not enough buffer size
      return s.__to_string(2*bufsize)
    return b

  def __str__(s):
    return s.__to_string(1024)

  def __del__(s):
    cond = {
      1 : lib.EC_elem_FF_delete, 
      2 : lib.EC_elem_EF_delete, 
    }
    cond[s.curve.type](s.ptr)

