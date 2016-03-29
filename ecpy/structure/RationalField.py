from ..abstract.Field import Field, FieldElement
from ..util import gcd, lcm


class RationalField(Field):
  def __init__(s):
    Field.__init__(s, RationalFieldElement)

  def __repr__(s):
    return "RationalField()"

  def __str__(s):
    return "RationalField()"

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
    return s.element_class(s, a[0] % b[0], a[1] % b[0])


class RationalFieldElement(FieldElement):
  def __init__(s, field, p, q):
    FieldElement.__init__(s, field, p)
    s.y = q
    s.p = p / gcd(p, q)
    s.q = q / gcd(p, q)

  def int(s):
    return int(s.real())

  def real(s):
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
