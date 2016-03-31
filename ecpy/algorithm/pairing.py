from root import cubic_root
from ..util import is_prime
from random import randint


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


def MapToPoint(E, y):
  """
  MapToPoint Function: Given by Boneh-Durfee's ID-based Encryption Paper.

  E: Elliptic Curve
  y: Any Value (should be E.field element)

  return: Point (corresponding x, y) on E
  """
  x = cubic_root(y**2 - 1)
  Q = E(x, y)
  return 6 * Q


def gen_supersingular_ec(bits=70):
  """
  Generate Super-Singluar Elliptic Curve
  bits: Security Parameter -> log_2 p = bits
  """
  from ..structure import EllipticCurve, ExtendedFiniteField

  def _next_prime(n):
    """
    return next prime of n
    """
    while not is_prime(n):
      n += 1
    return n

  """
  If you have gmpy, use gmpy.next_prime
  in other hand, use slow function
  """
  try:
    from gmpy import next_prime
  except:
    next_prime = _next_prime

  def gen_prime():
    """
    Generate Prime for Super Singular Elliptic Curve
    """
    while True:
      p = int(next_prime(randint(2**(bits - 1), 2**bits)))
      if is_prime(p * 6 - 1):
        break
    return p * 6 - 1, p

  p, l = gen_prime()
  F = ExtendedFiniteField(p, "x^2+x+1")
  return EllipticCurve(F, 0, 1), F, l


def find_point_by_order(E, l):
  """
  Find a Elliptic Curve Point P, that point has order l.
  """
  i = 3
  while True:
    r = E.get_corresponding_y(i)
    if r != None:
      P = E(i, r)
      if (P * l).is_infinity():
        return P
    i += 1


def symmetric_weil_pairing(E, P, Q, m):
  """
  Symmetric Weil Pairing
  \hat{e}(P, Q) = e(P, \phi(Q)) (\phi is Distortion Map)
  """
  return tate_pairing(E, P, Q.distortion_map(), m, 2)


def symmetric_tate_pairing(E, P, Q, m, k=2):
  """
  Symmetric Tate Pairing
  \hat{e}(P, Q) = e(P, \phi(Q)) (\phi is Distortion Map)
  """
  return tate_pairing(E, P, Q.distortion_map(), m)
