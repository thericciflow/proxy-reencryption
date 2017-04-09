from ecpy.utils import gcd, lcm
from .Field import Field, FieldElement


class RationalField(Field):
  def __init__(s):
    super(RationalField, s).__init__(RationalFieldElement)

  def _add(s, a, b):
    if a[1] == b[1]:
      return s.element_class(s, a[0] + b[0], a[1])
    else:
      l = lcm(a[1], b[1])
      p1 = a[0] * l / a[1]
      p2 = b[0] * l / b[1]
      return s.element_class(s, p1 + p2, l)

  def _mul(s, a, b):
    return s.element_class(s, a[0] * b[0], a[1] * b[1])

  def _neg(s, a):
    return s.element_class(s, -a[0], a[1])

  def _inv(s, a):
    return s.element_class(s, a[1], a[0])

  def _equ(s, a, b):
    return a[0] == b[0] and a[1] == b[1]

  def _mod(s, a, b):
    return s.element_class(s, a[0] % b[0], a[1])


class RationalFieldElement(FieldElement):
  def __init__(s, *args):
    p, q = None, None
    if len(args) >= 1:
      if isinstance(args[0], s.__class__):
        v = args[0]
        p = v.p
        q = v.q
        field = v.field
      elif len(args) >= 2:
        field = args[0]
        p = args[1]
        q = 1
        if len(args) == 3:
          q = args[2]
    if p == None or q == None:
      raise TypeError("Invalid Argument: '%s'" % args)
    super(RationalFieldElement, s).__init__(field, p)
    s.y = q
    s.p = int(p) / gcd(p, q)
    s.q = int(q) / gcd(p, q)

  def int(s):
    return int(s.real())

  def real(s):
    if s.q == 1:
      return s.p
    return s.p * 1.0 / s.q

  def __repr__(s):
    return "%r(%r, %r)" % (s.field, s.p, s.q)

  def __str__(s):
    if s.q == 1:
      return "%s" % s.p
    return "%s/%s" % (s.p, s.q)

  def __iter__(s):
    return (s.p, s.q).__iter__()

  def _to_tuple(s, d):
    if isinstance(d, s.__class__):
      return tuple(d)
    elif isinstance(d, tuple):
      return d
    else:
      return (d, 1)

QQ = RationalField()
