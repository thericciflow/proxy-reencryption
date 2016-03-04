from AdditiveGroup import AdditiveGroup, AdditiveGroupElement
from RealField import RR
from util import modinv,ModinvNotFoundError

class EllipticCurve(AdditiveGroup):
  def __init__(s, field, a, b):
    AdditiveGroup.__init__(s, EllipticCurvePoint)
    s.field = field
    s.a = a
    s.b = b
    s.O = s.element_class(s, None, None)
    s.O.infinity = True

  def is_on_curve(s, point):
    return s.field(point.y) ** 2 == s.field(point.x) ** 3 + s.field(s.a) * s.field(point.x) + s.field(s.b)

  def determinant(s):
    return -16*(4*s.a**3 + 27*s.b**2)

  def j_invariant(s):
    return -1728*((4*s.a**3) / s.determinant())

  def __repr__(s):
    return "EllipticCurve(%r, %r, %r)" % (s.field, s.a, s.b)

  def __str__(s):
    res = "Elliptic Curve y^2 = x^3"
    if s.a != 0:
      if s.a == 1:
        res += " + x"
      else:
        res += " + %rx" % s.a
    if s.b != 0:
      res += " + %r" % s.b
    res += " over %r" % s.field
    return res

  def _add(s, P, Q):
    Px, Py = P
    Qx, Qy = Q
    try:
      if Px == Qx:
        l = (3*Px**2 + s.a) * s.field._inv(2*Py).x
      else:
        l = (Qy-Py) * s.field._inv(Qx - Px).x
      l = s.field(l).x
      Rx = l**2 - (Px + Qx)
      Ry = -l * (Rx - Px) - Py
      return s.element_class(s, s.field(Rx).x, s.field(Ry).x)
    except ModinvNotFoundError:
      return s.O

  def _equ(s, P, Q):
    return (P[0] == Q[0]) and (P[1] == Q[1])

  def _neg(s, P):
    return s.element_class(s, P[0], -P[1])

class EllipticCurvePoint(AdditiveGroupElement):
  def __init__(s, group, x, y):
    AdditiveGroupElement.__init__(s, group, x)
    s.y = y
    s.infinity = x == None
    if not (s.infinity or s.group.is_on_curve(s)):
      raise ArithmeticError("Invalid Point: (%s, %s)" % (s.x, s.y))

  def is_infinity(s):
    return s.infinity

  def order(s):
    i = 1
    #while i <= s.order():
    while i <= s.group.field.order():
      if (s*i).is_infinity():
        return i
      i += 1
    return 0

  def change_group(s, _group):
    return s.__class__(_group, s.x, s.y)

  def __add__(s, rhs):
    if isinstance(rhs, EllipticCurvePoint):
      if rhs.is_infinity():
        return s
      d = (rhs.x, rhs.y)
    elif isinstance(rhs, tuple):
      d = rhs
    else:
      raise ArithmeticError("Invalid Parameter")
    if s.is_infinity():
      return s.__class__(s.group, d[0], d[1])
    else:
      return s.group._add((s.x, s.y), d)

  def __sub__(s, rhs):
    if isinstance(rhs, EllipticCurvePoint):
      d = (-rhs.x, -rhs.y)
    elif isinstance(rhs, tuple):
      d = -rhs
    else:
      raise ArithmeticError("Invalid Parameter")
    return s.group._add(s.x, d)

  def __mul__(s, rhs):
    if rhs == 0:
      return s.group.O
    d = rhs
    bits = map(lambda x: x == "1", bin(d)[2:])[::-1]
    x = s
    if bits[0]:
      res = x
    else:
      res = s.group.O
    for cur in bits[1:]:
      x += x
      if cur:
        res += x
    return res

  def __neg__(s):
    return s.group._neg((s.x, s.y))

  def __radd__(s, lhs):
    return s + lhs

  def __rmul__(s, lhs):
    return s * lhs

  def __rsub__(s, lhs):
    return -s + lhs

  def __ne__(s, rhs):
    return not (s == rhs)

  def __eq__(s, rhs):
    if isinstance(rhs, AdditiveGroupElement):
      d = (rhs.x, rhs.y)
    elif isinstance(rhs, tuple):
      d = rhs
    else:
      raise ArithmeticError("Invalid Parameter")
    return s.group._equ((s.x, s.y), d)

  def __repr__(s):
    if s.infinity:
      return "%r.O" % s.group
    return "%r(%r, %r)" % (s.group, s.x, s.y)

  def __str__(s):
    if s.infinity:
      return "Infinity Point (0 : 1 : 0) on %s" % s.group
    return "Point (%s, %s) on %s" % (s.x, s.y, s.group)
