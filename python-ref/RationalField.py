from Field import Field, FieldElement
from util import gcd, lcm

class RealField(Field):
  def __init__(s):
    Field.__init__(s, RationalFieldElement)

  def __repr__(s):
    return "RationalField()"

  def __str__(s):
    return "RationalField()"

  def _add(s, a, b):
    if a[1] == b[1]:
      return s.element_class(s, a[0]+b[0], a[1])
    else:
      l = lcm(a[1], b[1])
      p1 = a[0] * l/a[1]
      p2 = b[0] * l/b[1]
      return s.element_class(s, p1+p2, l)

  def _mul(s, a, b):
    return s.element_class(s, a[0]*b[0], a[1]*b[1])

  def _neg(s, a):
    return s.element_class(s, -a[0], a[1])

  def _inv(s, a):
    return s.element_class(s, a[1], a[0])

  def _equ(s, a, b):
    return a[0] == b[0] and a[1] == b[1]

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

  def __add__(s, rhs):
    if isinstance(rhs, RationalFieldElement):
      d = (rhs.p, rhs.q)
    else:
      d = s.field(rhs, 1)
      d = (s.p, s.q)
    return s.field._add(s.x, d)

  def __sub__(s, rhs):
    if isinstance(rhs, RationalFieldElement):
      d = ((-rhs).p, (-rhs).q)
    else:
      d = s.field(-rhs, 1)
      d = (s.p, s.q)
    return s.field._add(s.x, d)

  def __neg__(s):
    return s.field._neg((s.p, s.q))

  def __mul__(s, rhs):
    if isinstance(rhs, RationalFieldElement):
      d = (rhs.p, rhs.q)
    else:
      d = s.field(rhs, 1)
      d = (d.p, d.q)
    return s.field._mul(d, (s.p, s.q))

  def __div__(s, rhs):
    if isinstance(rhs, RationalFieldElement):
      d = (rhs.p, rhs.q)
    else:
      d = s.field(rhs, 1)
      d = (d.p, d.q)
    d = s.field._inv(d)
    return s.field._mul((d.p, d.q), (s.p, s.q))

  def __rdiv__(s, lhs):
    if isinstance(lhs, RationalFieldElement):
      d = (lhs.p, lhs.q)
    else:
      d = s.field(lhs, 1)
      d = (d.p, d.q)
    sf = s.field._inv((s.p, s.q))
    return s.field._mul(d, (sf.p, sf.q))

  def __mod__(s, rhs):
    return s.field(s.p % rhs, s.q % rhs)

  def __str__(s):
    if s.q == 1:
      return "%s" % s.p
    return "%s/%s" % (s.p, s.q)

def QQ(p, q):
  return RealField()(p, q)

if __name__ == "__main__":
  q = QQ(1, 2)
  print q
  print repr(q)
  print q.int()
  print q.real()
  print 1/q
