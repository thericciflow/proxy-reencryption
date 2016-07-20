from ecpy.abstract.AdditiveGroup import AdditiveGroup, AdditiveGroupElement

class Field(object):
  def __init__(s, element_class):
    s.element_class = element_class

  def __repr__(s):
    return "%s()" % s.__class__.__name__

  def __str__(s):
    return s.__class__.__name__

  def __call__(s, *x):
    return s.element_class(s, *x)

  def _add(s, a, b):
    raise NotImplementedError()

  def _neg(s, a, b):
    raise NotImplementedError()

  def _equ(s, a, b):
    raise NotImplementedError()

  def _mul(s, a, b):
    raise NotImplementedError()

  def _inv(s, a):
    raise NotImplementedError()

  def _mod(s, a, b):
    raise NotImplementedError()

  def _div(s, a, b):
    return s._mul(a, (s._inv(b)))

  def order(s):
    return 0

  def _ord(s, a):
    return 0


  def degree(s):
    return 1


class FieldElement(object):
  def __init__(s, field, x):
    s.group = s.field = field
    s.x = x

  def change_field(s, _field):
    return s.__class__(_field, *tuple(s))

  def __mul__(s, rhs):
    return s.field._mul(tuple(s), s._to_tuple(rhs))

  def __div__(s, rhs):
    return s.field._div(tuple(s), s._to_tuple(rhs))

  def __rdiv__(s, lhs):
    return s.field._div(s._to_tuple(lhs), tuple(s))

  def __pow__(s, rhs, mod=None):
    if rhs == 0:
      return s.__class__(s.field, 1)
    d = int(rhs)
    if d < 0:
      x = 1 / s
      d = -d
    else:
      x = s
    bits = map(int, bin(d)[2:])[::-1]
    if bits[0]:
      res = x
    else:
      res = s.field(1)
    for cur in bits[1:]:
      x *= x
      if cur:
        res *= x
    return res

  def __mod__(s, rhs):
    return s.field._mod(tuple(s), s._to_tuple(rhs))

  def __rmod__(s, lhs):
    return s.field._mod(s._to_tuple(lhs), tuple(s))

  def __rmul__(s, lhs):
    return s * lhs

  def __getitem__(s, idx):
    return s._to_tuple(s)[idx]

  def __len__(s):
    return 1

  def order(s):
    return s.group._ord(tuple(s))

  def int(s):
    return int(s.x)

  def __add__(s, rhs):
    return s.group._add(tuple(s), s._to_tuple(rhs))

  def __sub__(s, rhs):
    return s.group._add(tuple(s), s._to_tuple(-rhs))

  def __neg__(s):
    return s.group._neg(tuple(s))

  def __radd__(s, lhs):
    return s + lhs

  def __rsub__(s, lhs):
    return -s + lhs

  def __ne__(s, rhs):
    return not (s == rhs)

  def __eq__(s, rhs):
    return s.group._equ(tuple(s), s._to_tuple(rhs))

  def __repr__(s):
    return "%r(%s)" % (s.group, s.x)

  def __str__(s):
    return "%s" % s.x

  def _to_tuple(s, d):
    if isinstance(d, s.__class__):
      return tuple(d)
    elif isinstance(d, tuple):
      return d
    else:
      return (d, )

  def __iter__(s):
    return (s.x, ).__iter__()

  def __int__(s):
    return s.int()

  def __hash__(s):
    return s.x

