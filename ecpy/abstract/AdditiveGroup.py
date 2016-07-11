class AdditiveGroup(object):
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

  def order(s):
    return 0

  def _ord(s, a):
    return 0


class AdditiveGroupElement(object):
  def __init__(s, group, x):
    s.group = group
    s.x = x

  def change_group(s, _group):
    return s.__class__(_group, *tuple(s))

  def order(s):
    return s.group._ord(tuple(s))

  def __add__(s, rhs):
    return s.group._add(tuple(s), s._to_tuple(rhs))

  def __sub__(s, rhs):
    return s.group._add(tuple(s), s._to_tuple(-rhs))

  def __neg__(s):
    return s.group._neg(s.x)

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
