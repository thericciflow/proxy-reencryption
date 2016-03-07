from ..abstract.Field import Field, FieldElement
import math

class ComplexField(Field):
  def __init__(s):
    Field.__init__(s, ComplexFieldElement)
  def _add(s, a, b):
    return s.element_class(s, a[0] + b[0], a[1] + b[1])
  def _neg(s, a):
    return s.element_class(s, -a[0], -a[1])


class ComplexFieldElement(FieldElement):
  def __init__(s, field, x, y=0):
    FieldElement.__init__(s, field, x)
    s.y = y

  def norm(s):
    return s.x ** 2 - s.y ** 2

  def absolute_value(s):
    return math.sqrt(s.x ** 2 + s.y ** 2)

  def conjugate(s):
    return s.__class__(s.field, s.x, -s.y)

  def __repr__(s):
    return "%r(%r, %r)" % (s.field, s.x, s.y)

  def __str__(s):
    return "%r + %ri" % (s.x, s.y)

  def __add__(s, rhs):
    if isinstance(rhs, ComplexFieldElement):
      d = (rhs.x, rhs.y)
    else:
      d = (rhs, 0)
    return s.field._add((s.x, s.y), d)

  def __sub__(s, rhs):
    if isinstance(rhs, ComplexFieldElement):
      d = ((-rhs).x, -(rhs).y)
    else:
      d = (-rhs, 0)
    return s.field._add((s.x, s.y), d)

  def __neg__(s):
    return s.field._neg((s.x, s.y))

  def __mul__(s, rhs):
    if isinstance(rhs, ComplexFieldElement):
      d = (rhs.x, rhs.y)
    else:
      d = (rhs, 0)
    return s.field._mul((s.x, s.y), d)

  def __div__(s, rhs):
    if isinstance(rhs, ComplexFieldElement):
      d = (rhs.x, rhs.y)
    else:
      d = (rhs, 0)
    return s.field._mul(s.field._inv(d), (s.x, s.y))

  def __rdiv__(s, lhs):
    if isinstance(lhs, ComplexFieldElement):
      d = (lhs.x, lhs.y)
    else:
      d = (lhs, 0)
    return s.field._mul(d, s.field._inv((s.x, s.y)))

def CC(x, y=0):
  return ComplexField()(x, y)
