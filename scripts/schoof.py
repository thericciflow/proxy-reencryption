from __future__ import print_function, division
from ecpy import *
import math
import gmpy

def y_div(pol, div):
  '''
  Calculate pol / div

  Args:
    pol : Polynomial for x, y
    div = Polynomial for y
  '''
  assert all([len(t) == 1 for t in div])
  res = list(pol.coeffs)
  if len(div) == 1 and len(div[0]) == 1:
    return pol.ring.element_class(pol.ring, map(lambda y: map(lambda x: x / div[0][0], y), res))
  for i, t in enumerate(div):
    assert len(pol[0]) == 1 and pol[0][0] == 0
    t = t[0]
    if t == 0:
      continue
    res = res[i:]
    res = map(lambda y: list(map(lambda x: x / t, y)), res)
  return pol.ring.element_class(pol.ring, res)

@memoize
def torsion_polynomial(n, E, x, y):
  if n > 4:
    if n % 2 == 0:
      n = n // 2
      psi = torsion_polynomial
      pp2 = psi(n + 2, E, x, y)
      pp1 = psi(n + 1, E, x, y)
      pp0 = psi(n, E, x, y)
      pm1 = psi(n - 1, E, x, y)
      pm2 = psi(n - 2, E, x, y)
      R, S = pp0, 2 * y
      X, Y = (pp2 * pm1**2, pm2 * pp1**2)
      return y_div(R * (X - Y), S)
    else:
      n = (n-1) // 2
      psi = torsion_polynomial
      pp2 = psi(n + 2, E, x, y)
      pp1 = psi(n + 1, E, x, y)
      pp0 = psi(n, E, x, y)
      pm1 = psi(n - 1, E, x, y)
      X,  Y  = pp2 * pp0**3, pp1**3 * pm1
      return X - Y
  if n == -1:
    return x - 1 - x
  elif n == 0:
    return x - x
  elif n == 1:
    return x + 1 - x
  elif n == 2:
    return 2 * y
  elif n == 3:
    return 3 * x**4 + 6 * E.a * x**2 + 12 * E.b * x - E.a**2
  elif n == 4:
    return 4 * y * (x**6  + 5 * E.a * x**4 + 20 * E.b * x**3  - 5 * E.a ** 2 * x ** 2 - 4 * E.a * E.b * x - 8 * E.b**2 - E.a**3)

def schoof(F, E):
  def y2_reduce(pol, x, y, Fx):
    '''
    Reduce `pol` modulo (y^2 - Fx)
      <=> pol over F_q[x, y] / (y^2 - Fx)
    '''
    if pol.degree() < 2:
      return pol
    assert all([len(t) == 1 and t[0] == 0 for t in pol[1::2]])
    for i in xrange(1, (len(pol) - 1) // 2 + 1):
      y2 = (sum(map(lambda t: x**t[0] * t[1], enumerate(pol[2 * i]))))
      pol = pol - (y**(2*i) * y2)  + (y2 * Fx ** i)
    return pol


  @memoize
  def division_polynomial(ell, x, y):
    pol = torsion_polynomial(ell, E, x, y)
    pol.trim()
    return pol

  def get_polynomial(ell, x0, y0):
    if ell == 0:
      return PU.element_class(PU, [0])
    pp0 = division_polynomial(ell, x0, y0)
    pm1 = division_polynomial(ell - 1, x0, y0)
    pp1 = division_polynomial(ell + 1, x0, y0)
    pol_p = x - (pm1 * pp1)
    pol_q = pp0 ** 2
    pol_p = y2_reduce(pol_p, x, y, Fx)
    pol_q = y2_reduce(pol_q, x, y, Fx)
    pol_p = (pol_p.apply(xs, 0))
    pol_q = (pol_q.apply(xs, 0))
    pol = pol_p / pol_q
    if not isinstance(pol, UnivariatePolynomialRing):
      return PU.element_class(PU, [pol])
    return pol


  L = 2
  N = 1
  t = {}
  factors = {}
  q = F.p ** F.degree()
  PR = BivariatePolynomialRing(F, ['x', 'y'])
  PU = UnivariatePolynomialRing(F, 'xs')
  x, y = PR.gens()
  Fx = x**3 + E.a*x + E.b
  xs = PU.gen()

  while N < math.sqrt(F.n) * 4:
    L = int(gmpy.next_prime(L))
    qbar = q % L
    mod_poly = division_polynomial(L, x, y).apply(xs, 0)
    poly1 = get_polynomial(qbar + 1, x, y) % mod_poly
    for tbar in xrange(0, L):
      poly2 = get_polynomial(tbar, x, y) % mod_poly
      if poly1 == poly2:
        factors[L] = tbar
        N *= L
        break
    print(factors)

  t = crt(map(lambda x: factors[x], factors.keys()), factors.keys())
  if t > N // 2:
    t = - (N - t)
  return q + 1 - t


class ECPoly(BivariatePolynomialRing):
  def __init__(s, E):
    from ecpy.elliptic_curve.EllipticCurve import FiniteFieldEllipticCurve
    from ecpy.rings.polynomial_multi import BivariatePolynomialElement
    assert isinstance(E, FiniteFieldEllipticCurve)
    s.field = E.field
    BivariatePolynomialRing.__init__(s, s.field, ['x', 'y'])
    s.element_class = BivariatePolynomialElement
    s.PR = BivariatePolynomialRing(s.field, ['xs', 'ys'])
    xs, ys = s.PR.gens()
    s.curve_poly = xs**3 + E.a * xs + E.b

  def y2_reduce(s, pol):
    '''
    Reduce `pol` modulo (y^2 - Fx)
      <=> pol over F_q[x, y] / (y^2 - Fx)
    '''
    x, y = s.gens()
    xs, ys = s.PR.gens()
    pol = s.PR(tuple(pol))
    if pol.degree() < 2:
      return pol
    assert all([len(t) == 1 and t[0] == 0 for t in pol[1::2]])
    for i in xrange(1, (len(pol) - 1) // 2 + 1):
      y2 = (sum(map(lambda t: xs**t[0] * t[1], enumerate(pol[2 * i]))))
      pol = pol - (ys**(2*i) * y2)  + (y2 * s.curve_poly ** i)
    return pol

  def _add(s, A, B):
    res = BivariatePolynomialRing._add(s, A, B)
    return s.element_class(s, s.y2_reduce(res))

  def _mul(s, A, B):
    res = BivariatePolynomialRing._mul(s, A, B)
    return s.element_class(s, s.y2_reduce(res))

  def _equ(s, A, B):
    A = tuple(s.y2_reduce(s.element_class(s, A)))
    B = tuple(s.y2_reduce(s.element_class(s, B)))
    return s.element_class(s, BivariatePolynomialRing._equ(s, A, B))


if __name__ == '__main__':
  p = 137
  F = FiniteField(p)
  E = EllipticCurve(F, 2, 17)
  '''
  print(schoof(F, E))
  '''
  EP = ECPoly(E)
  x, y = EP.gens()
  print(y**2 - x**3 - 2*x - 17)
