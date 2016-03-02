import math

class FiniteField:
  def __init__(s, p):
    s.p = p

  def __repr__(s):
    return "FiniteField(%s)" % s.p

  def __str__(s):
    return "Fp : p = %d" % s.p

  def __call__(s, x):
    return FiniteFieldElement(s, x)

class FiniteFieldElement:
  def __init__(s, field, x):
    s.field = field
    s.x = x % field.p

  def __add__(s, rhs):
    return FiniteFieldElement(s.field, s.x + rhs.x)

  def __radd__(s, lhs):
    return FiniteFieldElement(s.field, s.x + lhs.x)

  def __sub__(s, rhs):
    return FiniteFieldElement(s.field, s.x + (s.field.p-rhs.x))

  def __rsub__(s, lhs):
    return FiniteFieldElement(s.field, (s.field.p-lhs.x) + s.x)

  def __mul__(s, rhs):
    return FiniteFieldElement(s.field, s.x + (s.field.p-rhs.x))

  def __repr__(s):
    return "%r(%s)" % (s.field, s.x)

  def __str__(s):
    return "%s" % s.x

if __name__ == "__main__":
  F = FiniteField(101)
  print "%r" % F
  print "%s" % F

  a = F(4)
  b = F(405)
  print "%r" % a
  print "%r" % b
  print "%r" % (a+b)
