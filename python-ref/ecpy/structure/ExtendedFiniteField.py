from ..abstract.Field import Field, FieldElement
from .RationalField import QQ
from .FiniteField import FiniteField, FiniteFieldElement
from .Zmod import Zmod
import math

class ExtendedFiniteField(FiniteField):
  def __init__(s, p):
    assert p % 4 == 3
    FiniteField.__init__(s, p)
    Field.__init__(s, ExtendedFiniteFieldElement)

  def __str__(s):
    return Zmod.__str__(s, "p^%d" % s.degree) + " : Polynomial is i^2+1 = 0"

  def _add(s, a, b):
    return s.element_class(s, a[0] + b[0], a[1] + b[1])

  def _neg(s, a):
    return s.element_class(s, s.p-a[0], s.p-a[1])

  def _mul(s, a, b):
    return s.element_class(s, a[0] * b[0] - a[1] * b[1], a[0]*b[1] + a[1] * b[0])

  def _equ(s, a, b):
    return a[0] == b[0] and a[1] == b[1]

  def _div(s, z, w):
    a, b, c, d = z[0], z[1], w[0], w[1]
    u = s._inv([c**2 + d**2])
    return s.element_class(s, (a*c + b*d)*u, (b*c-a*d)*u)

  def degree(s):
    return 2

class ExtendedFiniteFieldElement(FiniteFieldElement):
  def __init__(s, field, x, y=0):
    if isinstance(x, s.__class__):
      x, y = x.x, x.y
    FiniteFieldElement.__init__(s, field, y)
    s.y = s.x
    FiniteFieldElement.__init__(s, field, x)

  def __repr__(s):
    return "%r(%r, %r)" % (s.field, s.x, s.y)

  def __str__(s):
    res = ""
    if s.x != 0:
      res += "%r" % s.x
    if s.y != 0:
      if s.x != 0:
        res += " + "
      if s.y != 1:
        res += "%r" % s.y
      res += "i"
    return res

  def __iter__(s):
    return (s.x, s.y).__iter__()

  def _to_tuple(s, d):
    if isinstance(d, s.__class__):
      return tuple(d)
    elif isinstance(d, tuple):
      return d
    else:
      return (d, d)

