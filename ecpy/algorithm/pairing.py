def miller(E, P, Q, m):
  def h(P, Q, R):
    if (P == Q and P.y == 0) or (P != Q and P.x == Q.x): # if \lambda is infinity
      return R.x - P.x
    L = P.line_coeff(Q)
    p = R.y - P.y - L * (R.x - P.x)
    q = R.x + P.x + Q.x - L * L
    return p / q

  if P == Q:
    return 1

  b = map(int, bin(m)[2:])
  s = len(b) - 1
  assert b[0] == 1
  f = 1
  T = P
  for i in b[1:]:
    f = f * f * h(T, T, Q)
    T = T+T
    if i == 1:
      f = f * h(T, P, Q)
      T = T + P
  return f

def weil_pairing(E, P, Q, m, S = None):
  if S == None:
    S = E.random_point()
  fpqs = miller(E, P, Q+S, m)
  fps  = miller(E, P, S, m)
  fqps = miller(E, Q, P-S, m)
  fqs  = miller(E, Q, -S, m)
  u = E.field._inv(tuple(fqs))
  u = E.field._inv(tuple(u * fqps))
  return (E.field._inv(tuple(fps)) * fpqs) * u

def tate_pairing(E, P, Q, m, k = 2):
  f = E.field(miller(E, P, Q, m))
  return f ** (((E.field.p ** k) - 1) / m)
