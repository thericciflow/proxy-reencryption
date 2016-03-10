from ..abstract.Field import Field, FieldElement

class RealField(Field):
  def __init__(s):
    Field.__init__(s, RealFieldElement)

  def __repr__(s):
    return "RealField()"

  def __str__(s):
    return "RealField()"

  def _add(s, a, b):
    return s.element_class(s, a+b)

  def _mul(s, a, b):
    return s.element_class(s, a*b)

  def _neg(s, a):
    return s.element_class(s, -a)

  def _inv(s, a):
    return s.element_class(s, 1./a)

  def _equ(s, a, b):
    return a == b

class RealFieldElement(FieldElement):
  def __init__(s, field, x):
    FieldElement.__init__(s, field, x)

def RR(x):
  return RealField()(x)
