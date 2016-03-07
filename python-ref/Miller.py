from ecpy import *

def miller(E, P, Q, m):
  def h(P, Q, R):
    p = R.y - P.y - P.line_coeff(Q) * (R.x - P.x)
    q = R.x + P.x + Q.x - P.line_coeff(Q) ** 2
    return E.field(p) / E.field(q)

  if P == Q:
    return 1

  b = map(int, bin(m)[2:][::-1])
  s = len(b) - 1
  assert b[s] == 1
  f = 1
  T = E(P.x, P.y)
  i = s-1
  while i >= 0:
    f = f * f * h(T, T, Q)

    T = 2*T
    if b[i] == 1:
      f = f * h(T, P, Q)
      T = T + P
    i -= 1
  return f

if __name__ == "__main__":
  F = FiniteField(631)
  E = EllipticCurve(F, 30, 34)
  m = 5

  P = E(36, 60)
  Q = E(121, 387)
  S = E(0, 36)

  print miller(E, P, Q+S, m)
  print miller(E, P, S, m)
  print miller(E, Q, P-S, m)
  print miller(E, Q, -S, m)
