import math
from ..util import egcd, gcd, modinv, is_prime
from ..abstract.Field import Field, FieldElement
from .FiniteField import FiniteFieldElement

class Zmod(Field):
  def __init__(s, n):
    if is_prime(n):
      Field.__init__(s, FiniteFieldElement)
    else:
      Field.__init__(s, ZmodElement)
    s.n = n

  def __repr__(s):
    return "Zmod(%s)" % s.n

  def __str__(s):
    return "Zmod : n = %d" % s.n

  def order(s):
    return s.n - 1

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
    return s.element_class(s, modinv(a, s.n))

  def _neg(s, a):
    return s.element_class(s, s.n-a)

  def _equ(s, a, b):
    return a == b

class ZmodElement(FieldElement):
  def __init__(s, field, x):
    FieldElement.__init__(s, field, x)
    s.x = x % field.n

  def __repr__(s):
    return "%r(%s)" % (s.field, s.x)

  def __str__(s):
    return "%s" % s.x
