from .CommutativeRing import CommutativeRing, CommutativeRingElement

class Integer(CommutativeRing):
  def __init__(s):
    CommutativeRing.__init__(s, IntegerElement)

  def _add(s, a, b):
    return a[0] + b[0]

  def _neg(s, a):
    return -a[0]

  def _equ(s, a, b):
    return len(a) == len(b) == 1 and a[0] == b[0]

class IntegerElement(CommutativeRingElement):
  def __init__(s, ring, x):
    CommutativeRingElement.__init__(s, ring, x)

  def __mul__(s, rhs, mod=None):
    import six
    if isinstance(rhs, IntegerElement):
      return s.__class__(s.ring, s.x * rhs.x)
    elif isinstance(rhs, six.integer_types):
      return s.__class__(s.ring, s.x * rhs)
    else:
      raise ValueError('Invalid Value: %r' % rhs)



ZZ = Integer()
