from ..structure.EllipticCurve import EllipticCurve
from ..structure.FiniteField import FiniteField

def miller(E, P, Q, m):
  def h(P, Q, R):
    if (P == Q and P.y == 0) or (P != Q and P.x == Q.x): # if \lambda is infinity
      return R.x - P.x
    p = R.y - P.y - int(P.line_coeff(Q)) * (R.x - P.x)
    q = R.x + P.x + Q.x - int(P.line_coeff(Q)) ** 2
    #print "[+] p, q = %s, %s" % (p, q)
    return E.field(p) / q

  if P == Q:
    return 1

  b = map(int, bin(m)[2:])
  s = len(b) - 1
  assert b[0] == 1
  f = E.field(1)
  T = P
  pr_f = 1
  for i in b[1:]:
    #print "[+] f, f^2, h_T(T, Q) = %s, %s, %s" % (f, f**2, h(T, T, Q))
    f = f ** 2 * h(T, T, Q)
    T = 2*T
    if i == 1:
      #print "[+] h_T(P, Q), f = %s, %s" % (h(T, P, Q), f)
      f = f * h(T, P, Q)
      #print "[+] f = %s" % f
      T = T + P
  #print "[+] final f = %s" % f
  #print "[+] prev f = %s" % pr_f
  return f

def weil_pairing(E, P, Q, m, S = None):
  if S == None:
    S = E.random_point()
  fpqs = miller(E, P, Q+S, m)
  fps  = miller(E, P, S, m)
  fqps = miller(E, Q, P-S, m)
  fqs  = miller(E, Q, -S, m)
  return (E.field._inv([fps]) * fpqs) * E.field._inv(tuple(E.field._inv([(fqs)]) * fqps))

def tate_pairing(E, P, Q, m, k = None):
  if k == None:
    k = E.embedding_degree(m)
  f = E.field(miller(E, P, Q, m))
  return f ** (((E.field.p ** k) - 1) / m)
