import math
from util import egcd, gcd, modinv
from Field import Field, FieldElement

class FiniteField(Field):
  def __init__(s, p):
    Field.__init__(s, FiniteFieldElement)
    s.p = p

  def __repr__(s):
    return "FiniteField(%s)" % s.p

  def __str__(s):
    return "Finite Field : p = %d" % s.p

  def order(s):
    return s.p - 1

  def _ord(s, a):
    i = 1
    while i <= s.order():
      if s.element_class(s, a)**i == 1:
        return i
      i += 1
    return 0

  def _add(s, a, b):
    return s.element_class(s, a+b)

  def _mul(s, a, b):
    return s.element_class(s, a*b)

  def _inv(s, a):
    return s.element_class(s, modinv(a, s.p))

  def _neg(s, a):
    return s.element_class(s, s.p-a)

  def _equ(s, a, b):
    return a == b

class FiniteFieldElement(FieldElement):
  def __init__(s, field, x):
    FieldElement.__init__(s, field, x)
    s.x = x % field.p

  def __repr__(s):
    return "%r(%s)" % (s.field, s.x)

  def __str__(s):
    return "%s" % s.x
