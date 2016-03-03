import math
from util import egcd, gcd, modinv

class FiniteField:
  def __init__(s, p):
    s.p = p

  def __repr__(s):
    return "FiniteField(%s)" % s.p

  def __str__(s):
    return "Finite Field : p = %d" % s.p

  def __call__(s, x):
    return FiniteFieldElement(s, x)

class FiniteFieldElement:
  def __init__(s, field, x):
    s.field = field
    s.x = x % field.p

  def change_field(s, _field):
    return s.__class__(_field, s.x)

  def __add__(s, rhs):
    if isinstance(rhs, FiniteFieldElement):
      return s.__class__(s.field, s.x + rhs.x)
    else:
      return s.__class__(s.field, s.x + rhs)

  def __sub__(s, rhs):
    if isinstance(rhs, FiniteFieldElement):
      return s.__class__(s.field, s.x + (s.field.p-rhs.x))
    else:
      return s.__class__(s.field, s.x + (s.field.p-rhs))

  def __neg__(s):
    return s.__class__(-s.x)

  def __mul__(s, rhs):
    if isinstance(rhs, FiniteFieldElement):
      return s.__class__(s.field, s.x * (rhs.x))
    else:
      return s.__class__(s.field, s.x * rhs)

  def __div__(s, rhs):
    if isinstance(rhs, FiniteFieldElement):
      return s.__class__(s.field, s * modinv(rhs.x, s.field.p))
    else:
      return s.__class__(s.field, s * modinv(rhs, s.field.p))

  def __rdiv__(s, lhs):
    if isinstance(lhs, FiniteFieldElement):
      return s.__class__(s.field, modinv(s.x, s.field.p) * lhs.x)
    else:
      return s.__class__(s.field, modinv(s.x, s.field.p) * lhs)

  def __pow__(s, rhs):
    if rhs == 0:
      return s.__class__(s.field, 1)
    if isinstance(rhs, FiniteFieldElement):
      d = rhs.x
    else:
      d = rhs
    bits = map(lambda x: x == "1", bin(d)[2:])[::-1]
    x = s
    if bits[0]:
      res = x
    else:
      res = s.__class__(s.field, 1)
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
    if isinstance(rhs, FiniteFieldElement):
      return s.x == rhs.x
    else:
      return s.x  == rhs

  def __repr__(s):
    return "%r(%s)" % (s.field, s.x)

  def __str__(s):
    return "%s" % s.x
