from ecpy import *

def miller(E, P, Q, m):
  def ec_lambda(P, Q):
    if P.x == Q.x:
      l = (3*P.x**2 + E.a)
      m = 2*P.y
      if m == 0:
        return Q.x - P.x
      l = (l * E.field._inv(m)).x
      return Q.y - P.y - l * (Q.x - P.x)
    else:
      return Q.x - P.x
    return l

  def l(P, Q, R):
    return R.y - (ec_lambda(P, Q) * (R.x - P.x) + P.y)

  def v(P, Q):
    return Q.x - P.x

  def g(P, Q, R):
    return l(P, Q, R) / v(P+Q, R)

  d = map(lambda x: x == "1",bin(m)[2:])
  f = E.field(1)
  T = P
  for x in d[1:]:
    f = f**2 * g(T, T, Q)
    T = 2*T
    if x:
      f = f * g(P, T, Q)
      T = T + P
    print repr(f)
  return f.x

if __name__ == "__main__":
  F = FiniteField(101)
  E = EllipticCurve(F, 1, 3)
  m = 29

  P = E(60, 23)
  Q = E(97, 6)

  print miller(E, P, Q, m)

  """
  in sage, 

  sage: F = GF(101)
  sage: E = EllipticCurve(F, [1, 3])
  sage: P = E(60, 23)
  sage: Q = E(97, 6)
  sage: P
  (60 : 23 : 1)
  sage: Q
  (97 : 6 : 1)
  sage: P._miller_(Q, P.order())
  60
  """
