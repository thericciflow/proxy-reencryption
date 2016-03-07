from ecpy import *

def miller(E, P, Q, m):
  def h(P, Q, R):
    if (P == Q and P.y == 0) or (P != Q and P.x == Q.x): # if \lambda is infinity
      return R.x - P.x
    p = R.y - P.y - P.line_coeff(Q) * (R.x - P.x)
    q = R.x + P.x + Q.x - P.line_coeff(Q) ** 2
    return E.field(p) / E.field(q)

  if P == Q:
    return 1

  b = map(int, bin(m)[2:])
  s = len(b) - 1
  assert b[s] == 1
  f = E.field(1)
  T = P
  for i in xrange(1, len(b)):
    f = f * f * h(T, T, Q)
    T = 2*T
    if b[i] == 1:
      f = f * h(T, P, Q)
      T = T + P
    i -= 1
  return int(f)

if __name__ == "__main__":
  F = FiniteField(631)
  E = EllipticCurve(F, 30, 34)
  m = 5

  P = E(36, 60)
  Q = E(121, 387)
  S = E(0, 36)

  print miller(E, P, Q+S, m), 103
  print miller(E, P, S, m), 219
  print miller(E, Q, P-S, m), 284
  print miller(E, Q, -S, m), 204
