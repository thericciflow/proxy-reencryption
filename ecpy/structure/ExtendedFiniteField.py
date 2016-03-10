from ..abstract.Field import Field, FieldElement
from .RationalField import QQ
from .FiniteField import FiniteField, FiniteFieldElement
from .Zmod import Zmod
from ..util import is_prime
import math

class ExtendedFiniteField(FiniteField):
  def __init__(s, p, poly="x^2+1"):
    if poly == "x^2+1":
      assert p % 4 == 3
      s.t = 1
    elif poly == "x^2+x+1":
      assert p % 3 == 2 and (p+1)%6 == 0 and is_prime((p+1)/6)
      s.t = 2
    else:
      raise ValueError("Invalid Polynomial: %s" % poly)
    FiniteField.__init__(s, p)
    Field.__init__(s, ExtendedFiniteFieldElement)

  def __str__(s):
    return Zmod.__str__(s, "p^%d" % s.degree()) + " : Polynomial is i^2+1 = 0"

  def _add(s, a, b):
    if s.t == 1:
      return s.element_class(s, a[0] + b[0], a[1] + b[1])
    elif s.t == 2:
      return s.element_class(s, a[0] + b[0], a[1] + b[1], a[2] + b[2])

  def _neg(s, a):
    if s.t == 1:
      return s.element_class(s, s.p-a[0], s.p-a[1])
    elif s.t == 2:
      return s.element_class(s, s.p-a[0], s.p-a[1], s.p-a[2])

  def _mul(s, a, b):
    if s.t == 1:
      return s.element_class(s, a[0] * b[0] - a[1] * b[1], a[0]*b[1] + a[1] * b[0])
    elif s.t == 2:
      u = a[0] * b[0] + a[1] * b[1] + a[2] * b[2]
      v = a[0] * b[2] + a[1] * b[0] + a[2] * b[1]
      w = a[0] * b[1] + a[1] * b[2] + a[2] * b[0]
      return s.element_class(s, u, v, w)

  def _equ(s, a, b):
    if s.degree() == 2:
      if len(b) == 1:
        return a[0] == b[0] and a[1] == 0
      return a[0] == b[0] and a[1] == b[1]
    elif s.degree() == 3:
      if len(b) == 1:
        return a[0] == b[0] and a[1] == 0 and a[2] == 0
      return a[0] == b[0] and a[1] == b[1] and a[2] == b[2]

  def _div(s, z, w):
    if s.t == 1:
      a, b, c, d = z[0], z[1], w[0], w[1]
      u = s._inv([c**2 + d**2])
      return s.element_class(s, (a*c + b*d)*u, (b*c-a*d)*u)
    elif s.t == 2:
      a, b = w, z
      r = s._inv([a[0] ** 2 + a[1] ** 2 + a[2] ** 2 - (a[0] * a[1] + a[1] * a[2] + a[0] * a[2])])
      u = a[0] * b[0] + a[1] * b[1] + a[2] * b[2]
      v = a[0] * b[2] + a[1] * b[0] + a[2] * b[1]
      w = a[0] * b[1] + a[1] * b[2] + a[2] * b[0]
      return s.element_class(s, u*r, v*r, w*r)

  def degree(s):
    if s.t == 1:
      return 2
    elif s.t == 2:
      return 3

class ExtendedFiniteFieldElement(FiniteFieldElement):
  def __init__(s, field, x, y=0, z=0):
    if isinstance(x, s.__class__):
      x, y, z = x.x, x.y, x.z
    FiniteFieldElement.__init__(s, field, z)
    s.z = s.x
    FiniteFieldElement.__init__(s, field, y)
    s.y = s.x
    FiniteFieldElement.__init__(s, field, x)

  def __repr__(s):
    if s.field.t == 1:
      return "%r(%r, %r)" % (s.field, s.x, s.y)
    elif s.field.t == 2:
      return "%r(%r, %r, %r)" % (s.field, s.x, s.y, s.z)

  def __str__(s):
    res = None
    if s.field.t == 1:
      res = ""
      if s.x != 0:
        res += "%r" % s.x
      if s.y != 0:
        if s.x != 0:
          res += " + "
        if s.y != 1:
          res += "%r" % s.y
        res += "i"
    elif s.field.t == 2:
      res = ""
      if s.x != 0:
        res += "%r" % s.x
      if s.y != 0:
        if s.x != 0:
          res += " + "
        if s.y != 1:
          res += "%r" % s.y
        res += "w"
      if s.z != 0:
        if len(res) != 0:
          res += " + "
        if s.z != 1:
          res += "%r" % s.y
        res += "w^2"
    return res

  def __iter__(s):
    if s.field.t == 1:
      return (s.x, s.y).__iter__()
    elif s.field.t == 2:
      return (s.x, s.y, s.z).__iter__()

  def _to_tuple(s, d):
    if isinstance(d, s.__class__):
      return tuple(d)
    elif isinstance(d, tuple):
      return d
    else:
      if s.field.t == 1:
        return (d, d)
      elif s.field.t == 2:
        return (d, 0, 0)

