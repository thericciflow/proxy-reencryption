def is_number(x):
  return isinstance(x, int) or isinstance(x, long) or isinstance(x, float)

class Variable:
  def __init__(s, name):
    s.name = name

  def __repr__(s):
    return "Variable(%r)" % s.name

  def __str__(s):
    return s.name

  def __add__(s, rhs):
    if is_number(rhs):
      return Polynomial([Monomial(1, [s], [1]), Monomial(rhs, [], [])])
    elif isinstance(rhs, Monomial):
      return Polynomial([Monomial(1, [s], [1]), rhs])
    elif isinstance(rhs, Variable):
      if rhs == s:
        return Monomial(2, [s], [1])
      return Polynomial([Monomial(1, [s], [1]), Monomial(1, [rhs], [1])])
    else:
      return None

  def __mul__(s, rhs):
    if is_number(rhs):
      return Polynomial([Monomial(rhs, [s], [1])])
    elif isinstance(rhs, Monomial):
      return Polynomial([Monomial(1, [s], [1]), rhs])
    elif isinstance(rhs, Variable):
      if rhs == s:
        return Polynomial([Monomial(1, [s], [2])])
      else:
        return Polynomial([Monomial(1, [s, rhs], [1, 1])])
    else:
      return None

  def __eq__(s, rhs):
    if isinstance(rhs, Variable):
      return rhs.name == s.name
    elif isinstance(rhs, Monomial):
      return rhs.coeff == 1 and rhs.variables == [s] and s.powers == [1]

class Monomial:
  def __init__(s, coeff, variables, powers):
    s.coeff = coeff
    s.variables = variables
    s.powers = powers


  def __repr__(s):
    return "Monomial(%r, %r, %r)" % (s.coeff, s.variables, s.powers)

  def __str__(s):
    if s.coeff == 0:
      return ""
    res = ""
    for x,y in zip(s.variables, s.powers):
      if y != 0:
        res += str(x)
        if y > 1:
          res += "^%s" % y
    if s.coeff != 1:
      if s.coeff == -1:
        res = "-" + res
      else:
        res = str(s.coeff) + res
    return res

class Polynomial:
  def __init__(s, monomials):
    s.monomials = monomials

  def __repr__(s):
    return "Polynomial(%r)" % s.monomials

  def __str__(s):
    return "+".join(map(str, s.monomials))

if __name__ == "__main__":
  x = Variable("x")
  y = Variable("y")
  print x
  print Monomial(3, [x], [1])
  print Monomial(2, [x, y], [2, 1])
  print x+y
  print Monomial(-2, [x], [1])
  print Monomial(-1, [x], [1])
  print x+x
