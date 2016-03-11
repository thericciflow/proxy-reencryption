from ..abstract.AdditiveGroup import AdditiveGroup, AdditiveGroupElement
from ..util import modinv,ModinvNotFoundError,jacobi_symbol, gcd, modular_square_root
from random import randint
from RealField import RR
from FiniteField import FiniteField
from ExtendedFiniteField import ExtendedFiniteField


def EllipticCurve(field, a, b):
  if isinstance(field, FiniteField) or isinstance(field, ExtendedFiniteField):
    return FiniteFieldEllipticCurve(field, a, b)
  else:
    return GenericEllipticCurve(field, a, b)

class GenericEllipticCurve(AdditiveGroup):
  def __init__(s, field, a, b):
    AdditiveGroup.__init__(s, GenericEllipticCurvePoint)
    s.field = field
    s.a = a
    s.b = b
    s.O = s.element_class(s, 0, 1, 0)

  def is_on_curve(s, point):
    return s._is_on_curve(point.x, point.y)

  def _is_on_curve(s, x, y):
    return s.field(y) ** 2, s.field(x) ** 3 + s.a * s.field(x) + s.b

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
    Px, Py, Pz = map(int, P)
    Qx, Qy, Qz = map(int, Q)
    Rx, Ry, Rz = (0, 1, 0)
    try:
      if not s._equ(P, Q):
        u = Qy*Pz - Py*Qz
        v = Qx*Pz - Px*Qz
        w = u**2 * Pz * Qz - v**2 * (v + 2*Px*Qz)
        Rx = v*w
        Ry = u * (v**2 * Px * Qz - w) - v**3 * Py * Qz
        Rz = v**3 * Pz * Qz
      else:
        u = 3*Px**2 + s.a * Pz**2
        v = Py * Pz
        w = u**2 - 8*Px*Py*v
        Rx = 2*v*w
        Ry = u * (4 * Px * Py * v - w) - 8 * (Py * v) ** 2
        Rz = 8*v**3
      z = s.field._inv([Rz])
      Rx = Rx * z
      Ry = Ry * z
      Rz = Rz * z
      return s.element_class(s, int(Rx), int(Ry), int(Rz))
    except:
      return s.O

  def _equ(s, P, Q):
    return P[0] * Q[1] == P[1] * Q[0]

  def _neg(s, P):
    return s.element_class(s, P[0], -P[1])

class GenericEllipticCurvePoint(AdditiveGroupElement):
  def __init__(s, group, x, y, z = 1):
    if isinstance(x, tuple):
      AdditiveGroupElement.__init__(s, group, None)
      s.x = group.field(*x)
    else:
      AdditiveGroupElement.__init__(s, group, x)
      s.x = x
    if isinstance(y, tuple):
      s.y = group.field(*y)
    else:
      s.y = y
    if isinstance(z, tuple):
      s.z = group.field(*z)
    else:
      s.z = z
    if not (s.is_infinity() or s.group.is_on_curve(s)):
      raise ArithmeticError("Invalid Point: (%s, %s)" % (s.x, s.y))

  def is_infinity(s):
    return s.x == 0 and s.y == 1 and s.z == 0

  def order(s):
    i = 0
    t = s
    while i <= s.group.field.order() ** s.group.field.degree():
      t *= s
      if t.is_infinity():
        return i
      i += 1
    return 0

  def change_group(s, _group):
    return s.__class__(_group, *tuple(s))

  def line_coeff(s, Q):
    P = s
    F = s.group.field
    x1, y1, z1 = map(F, tuple(P))
    x2, y2, z2 = map(F, tuple(Q))
    if x1*z2 == x2*z1:
      l = (3*x1**2 + s.group.a) / (2*y1)
    else:
      l = (y2*z1-y1*z2) / (x2*z1-x1*z2)
    return l

  def __add__(s, rhs):
    if isinstance(rhs, GenericEllipticCurvePoint) and rhs.is_infinity():
        return s
    d = s._to_tuple(rhs)
    if s.is_infinity():
      return s.__class__(s.group, d[0], d[1])
    else:
      return s.group._add(tuple(s), d)

  def __sub__(s, rhs):
    return s.group._add(tuple(s), s._to_tuple(-rhs))

  def __mul__(s, rhs):
    if rhs == 0:
      return s.group.O
    d = int(rhs)
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
    return s.group._neg(tuple(s))

  def __rmul__(s, lhs):
    return s * lhs

  def __eq__(s, rhs):
    if rhs == None:
      return False
    return s.group._equ(tuple(s), s._to_tuple(rhs))

  def _to_tuple(s, d):
    if isinstance(d, s.__class__):
      return tuple(d)
    elif isinstance(d, tuple):
      return d
    else:
      raise ArithmeticError("Invalid Parameter")

  def __iter__(s):
    return (s.x, s.y, s.z).__iter__()

  def __repr__(s):
    if s.is_infinity():
      return "%r.O" % s.group
    return "%r(%r, %r, %r)" % (s.group, s.x, s.y, s.z)

  def __str__(s):
    if s.is_infinity():
      return "Infinity Point (0 : 1 : 0) on %s" % s.group
    return "Point (%s : %s : %s) on %s" % (s.x, s.y, s.z, s.group)

class FiniteFieldEllipticCurve(GenericEllipticCurve):
  def __init__(s, field, a, b):
    GenericEllipticCurve.__init__(s, field, a, b)
    AdditiveGroup.__init__(s, FiniteFieldEllipticCurvePoint)
    s.O = s.element_class(s, 0, 1, 0)

  def get_corresponding_y(s, x):
    y_square = int(s.field(x) ** 3 + s.a * x + s.b)
    if jacobi_symbol(y_square, s.field.p) == 1:
      return modular_square_root(y_square, s.field.p)
    return None

  def embedding_degree(s, m):
    k = 1
    while True:
      if (s.field.p ** (k * s.field.degree()) - 1) % m == 0:
        return k
      k += 1

  def random_point(s):
    while True:
      x = s.field(*tuple([randint(0, s.field.order()+1) for _ in xrange(s.field.degree())]))
      y = s.get_corresponding_y(x)
      if y != None:
        for i in [0, 1]:
          if s._is_on_curve(x, y[i]):
            return s.element_class(s, x, y[i])

class FiniteFieldEllipticCurvePoint(GenericEllipticCurvePoint):
  def __init__(s, group, x, y, z = 1):
    if isinstance(x, tuple):
      AdditiveGroupElement.__init__(s, group, None)
      s.x = group.field(*x)
    else:
      AdditiveGroupElement.__init__(s, group, x)
      s.x = group.field(x)
    if isinstance(y, tuple):
      s.y = group.field(*y)
    else:
      s.y = group.field(y)
    if isinstance(z, tuple):
      s.z = group.field(*z)
    else:
      s.z = z
    if not (s.is_infinity() or s.group.is_on_curve(s)):
      raise ArithmeticError("Invalid Point: (%r, %r)" % (s.x, s.y))

  def is_infinity(s):
    return s.x == 0 and s.y == 1 and s.z == 0

  def distortion_map(s):
    if s.group.field.t == 1:
      x = s.x
      y = s.y
      if isinstance(x, int) or isinstance(x, long):
        x = (x, 0)
      else:
        x = tuple(x)
      if isinstance(y, int) or isinstance(y, long):
        y = (y, 0)
      else:
        y = tuple(y)
      x = (-x[0], -x[1])
      y = (y[1], y[0])
      return s.__class__(s.group, x, y)
    elif s.group.field.t == 2:
      x = s.x
      y = s.y
      if isinstance(x, int) or isinstance(x, long):
        x = (x, 0)
      else:
        x = tuple(x)
      if isinstance(y, int) or isinstance(y, long):
        y = (y, 0)
      else:
        y = tuple(y)
      x = (x[1], x[0])
      y = (y[0], y[1])
      return s.__class__(s.group, x, y)

  def _to_tuple(s, d):
    if isinstance(d, s.__class__):
      return tuple(d)
    elif isinstance(d, tuple):
      return d
    else:
      raise ArithmeticError("Invalid Parameter")

  def order(s):
    r = s.change_group(s.group)
    i = 2
    while True:
      if r.is_infinity():
        return i
      r += s
      i += 1
    return 0
