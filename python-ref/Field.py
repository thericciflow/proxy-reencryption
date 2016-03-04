import math
from util import egcd, gcd, modinv

class Field:
  def __init__(s, element_class):
    s.element_class = element_class

  def __repr__(s):
    return "Field()"

  def __str__(s):
    return "Field"

  def __call__(s, *x):
    return s.element_class(s, *x)

  def _add(s, a, b):
    raise NotImplementedError()

  def _mul(s, a, b):
    raise NotImplementedError()

  def _inv(s, a):
    raise NotImplementedError()

  def _neg(s, a):
    raise NotImplementedError()

  def _equ(s, a, b):
    raise NotImplementedError()

  def order(s):
    return 0

  def _ord(s, a):
    return 0

class FieldElement:
  def __init__(s, field, x):
    s.field = field
    s.x = x

  def change_field(s, _field):
    return s.__class__(_field, s.x)

  def order(s):
    return s.field._ord(s.x)

  def __add__(s, rhs):
    if isinstance(rhs, FieldElement):
      d = rhs.x
    else:
      d = rhs
    return s.field._add(s.x, d)

  def __sub__(s, rhs):
    if isinstance(rhs, FieldElement):
      d = (-rhs).x
    else:
      d = -rhs
    return s.field._add(s.x, d)

  def __neg__(s):
    return s.field._neg(s.x)

  def __mul__(s, rhs):
    if isinstance(rhs, FieldElement):
      d = s.x
    else:
      d = rhs
    return s.field._mul(rhs.x, d)

  def __div__(s, rhs):
    if isinstance(rhs, FieldElement):
      d = rhs.x
    else:
      d = rhs
    return s.field._mul(d, s.field._inv(s.x).x)

  def __rdiv__(s, lhs):
    if isinstance(lhs, FieldElement):
      d = lhs.x
    else:
      d = lhs
    return s.field._mul(s.field._inv(d).x, s.x)

  def __pow__(s, rhs):
    if rhs == 0:
      return s.__class__(s.field, 1)
    if isinstance(rhs, FieldElement):
      d = rhs.x
    else:
      d = rhs
    bits = map(lambda x: x == "1", bin(d)[2:])[::-1]
    x = s
    if bits[0]:
      res = x
    else:
      res = s.field(1)
    for cur in bits[1:]:
      x *= x
      if cur:
        res *= x
    return res

  def __radd__(s, lhs):
    return s + lhs

  def __rsub__(s, lhs):
    return -s + lhs

  def __rmul__(s, lhs):
    return s * lhs

  def __ne__(s, rhs):
    return not (s == rhs)

  def __eq__(s, rhs):
    if isinstance(rhs, FieldElement):
      d = rhs.x
    else:
      d = rhs
    return s.field._equ(s.x, d)

  def __repr__(s):
    return "%r(%s)" % (s.field, s.x)

  def __str__(s):
    return "%s" % s.x
