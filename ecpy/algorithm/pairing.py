def miller(E, P, Q, m):
  """
  Calculate f_P(Q)
  """
  def h(P, Q, R):
    # if \lambda is infinity
    if (P == Q and P.y == 0) or (P != Q and P.x == Q.x):
      return R.x - P.x
    L = P.line_coeff(Q)
    p = R.y - P.y - L * (R.x - P.x)
    q = R.x + P.x + Q.x - L * L
    return p / q
  if P == Q:
    return 1
  b = map(int, bin(m)[2:])
  f = 1
  T = P
  for i in b[1:]:
    f = f * f * h(T, T, Q)
    T = T + T
    if i:
      f = f * h(T, P, Q)
      T = T + P
  return f


def weil_pairing(E, P, Q, m, S=None):
  """
  Calculate Weil Pairing
  """
  if S is None:
    S = E.random_point()
  fpqs = miller(E, P, Q + S, m)
  fps = miller(E, P, S, m)
  fqps = miller(E, Q, P - S, m)
  fqs = miller(E, Q, -S, m)
  return E.field._inv(fps * fqps) * fpqs * fqs


def tate_pairing(E, P, Q, m, k=2):
  """
  Calculate Tate Pairing
  """
  f = miller(E, P, Q, m)
  return f ** (((E.field.p ** k) - 1) / m)
