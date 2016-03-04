from ecpy import *

def miller(E, P, Q, m):
  def ec_lambda(P, Q):
    if P.x == Q.x:
      l = (3*P.x**2 + E.a) * E.field._inv(2*P.y).x
    else:
      l = (Q.y-P.y) * E.field._inv(Q.x - P.x).x
    l = E.field(l).x
    return l

  def l(P, Q, R):
    return R.y - (ec_lambda(P, Q) * (R.x - P.x) + P.y)

  def v(P, Q):
    return Q.x - P.x

  def g(P, Q, R):
    return (l(P, Q, R) / v(P+Q, R))

  d = map(lambda x: x == "1",bin(m)[2:])
  f = E.field(1)
  T = P
  for x in d:
    f = f**2 * g(T, T, Q)
    T = 2*T
    if x:
      f = f * g(P, T, Q)
      T = T + P
  return f.x

if __name__ == "__main__":
  F = FiniteField(101)
  E = EllipticCurve(F, 1, 3)
  m = 29

  P = E(60, 23)
  Q = E(97, 6)

  print miller(E, P, Q, m)
