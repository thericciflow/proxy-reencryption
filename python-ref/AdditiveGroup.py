import math
from util import egcd, gcd, modinv

class AdditiveGroup:
  def __init__(s, element_class):
    s.element_class = element_class

  def __repr__(s):
    return "AdditiveGroup()"

  def __str__(s):
    return "AdditiveGroup"

  def __call__(s, x):
    return s.element_class(s, x)

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

class AdditiveGroupElement:
  def __init__(s, group, x):
    s.group = group
    s.x = x

  def change_group(s, _group):
    return s.__class__(_group, s.x)

  def order(s):
    return s.group._ord(s.x)

  def __add__(s, rhs):
    if isinstance(rhs, AdditiveGroupElement):
      return s.group._add(s.x, rhs.x)
    else:
      return s.group._add(s.x, rhs)

  def __sub__(s, rhs):
    if isinstance(rhs, AdditiveGroupElement):
      return s.group._add(s.x, (-rhs).x)
    else:
      return s.group._add(s.x, -rhs)

  def __neg__(s):
    return s.group._neg(s.x)

  def __radd__(s, lhs):
    return s + lhs

  def __rsub__(s, lhs):
    return -s + lhs

  def __ne__(s, rhs):
    return not (s == rhs)

  def __eq__(s, rhs):
    if isinstance(rhs, AdditiveGroupElement):
      return s.group._equ(s.x, rhs.x)
    else:
      return s.group._equ(s.x, rhs)

  def __repr__(s):
    return "%r(%s)" % (s.group, s.x)

  def __str__(s):
    return "%s" % s.x
