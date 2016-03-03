from AdditiveGroup import AdditiveGroup, AdditiveGroupElement

class EllipticCurve(AdditiveGroup):
  def __init__(s, field, a, b):
    AdditiveGroup.__init__(s, EllipticCurvePoint)
    s.field = field
    s.a = a
    s.b = b

  def is_on_curve(s, point):
    return point.y ** 2 == point.x**3 + s.a*point.x + s.b

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


class EllipticCurvePoint(AdditiveGroupElement):
  def __init__(s, group, x, y, z=None):
    AdditiveGroupElement.__init__(s, group, x)
    s.y = y
    if not z == None:
      s.z = z
    else:
      s.z = None
    if not s.group.is_on_curve(s):
      if z == None:
        raise ArithmeticError("Invalid Point: (%s, %s)" % (s.x, s.y))
      else:
        raise ArithmeticError("Invalid Point: (%s, %s, %s)" % (s.x, s.y, s.z))

  def _add(s, a, b):
    if a[2] == None: # is euclidean space?
      Px, Py, _ = a
      Qx, Qy, _ = b
      return s.group(s, 0, 0, 0)


  def __add__(s, rhs):
    if isinstance(rhs, AdditiveGroupElement):
      d = (rhs.x, rhs.y, rhs.z)
    elif isinstance(rhs, tuple):
      d = rhs
    else:
      raise ArithmeticError("Invalid Parameter")
    return s.group._add((s.x, s.y, s.z), d)

  def __sub__(s, rhs):
    if isinstance(rhs, AdditiveGroupElement):
      d = (-rhs.x, -rhs.y, -rhs.z)
    elif isinstance(rhs, tuple):
      d = -rhs
    else:
      raise ArithmeticError("Invalid Parameter")
    return s.group._add(s.x, d)

  def __neg__(s):
    return s.group._neg((s.x, s.y, s.z))

  def __radd__(s, lhs):
    return s + lhs

  def __rsub__(s, lhs):
    return -s + lhs

  def __ne__(s, rhs):
    return not (s == rhs)

  def __eq__(s, rhs):
    if isinstance(rhs, AdditiveGroupElement):
      d = (rhs.x, rhs.y, rhs.z)
    elif isinstance(rhs, tuple):
      d = rhs
    else:
      raise ArithmeticError("Invalid Parameter")
    return s.group._equ(s.x, d)

  def __repr__(s):
    if s.z == None:
      return "%r(%r, %r)" % (s.group, s.x, s.y)
    else:
      return "%r(%r, %r, %r)" % (s.group, s.x, s.y, s.z)

  def __str__(s):
    if s.z == None:
      return "Point (%s, %s) on %s (Euclidean Space)" % (s.x, s.y, s.group)
    else:
      return "Point (%s : %s : %r) on %s (Projective Space)" % (s.x, s.y, s.z, s.group)
