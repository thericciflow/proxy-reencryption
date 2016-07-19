from ecpy.abstract.AdditiveGroup import AdditiveGroup, AdditiveGroupElement

class Field(AdditiveGroup):
  def __init__(s, element_class):
    s.element_class = element_class

  def _mul(s, a, b):
    raise NotImplementedError()

  def _inv(s, a):
    raise NotImplementedError()

  def _mod(s, a, b):
    raise NotImplementedError()

  def _div(s, a, b):
    return s._mul(a, (s._inv(b)))

  def degree(s):
    return 1


class FieldElement(AdditiveGroupElement):
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
