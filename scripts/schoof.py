from __future__ import print_function, division
from ecpy import *
import math
import gmpy

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
      pp2p, pp2q = psi(n + 2, E, x, y)
      pp1p, pp1q = psi(n + 1, E, x, y)
      pp0p, pp0q = psi(n, E, x, y)
      pm1p, pm1q = psi(n - 1, E, x, y)
      pm2p, pm2q = psi(n - 2, E, x, y)
      R, S = pp0p, (pp0q * 2 * y)
      X, Y = (pp2p * pm1p**2, pm2p * pp1p**2)
      X_, Y_ = (pp2q * pm1q**2, pm2q * pp1q**2)
      return y_div(R * (y_div(X, X_) - y_div(Y, Y_)), S), x**0
    else:
      n = (n-1) // 2
      psi = torsion_polynomial
      pp2p, pp2q = psi(n + 2, E, x, y)
      pp1p, pp1q = psi(n + 1, E, x, y)
      pp0p, pp0q = psi(n, E, x, y)
      pm1p, pm1q = psi(n - 1, E, x, y)
      X,  Y  = pp2p * pp0p**3, pp1p**3 * pm1p
      X_, Y_ = pp2q * pp0q**3, pp1q**3 * pm1q
      return y_div(X, X_) - y_div(Y, Y_), x**0
  if n == -1:
    return -1, x**0
  elif n == 0:
    return 0, x**0
  elif n == 1:
    return 1, x**0
  elif n == 2:
    return 2 * y, x**0
  elif n == 3:
    return 3 * x**4 + 6 * E.a * x**2 + 12 * E.b * x - E.a**2, x**0
  elif n == 4:
    return 4 * y * (x**6  + 5 * E.a * x**4 + 20 * E.b * x**3  - 5 * E.a ** 2 * x ** 2 - 4 * E.a * E.b * x - 8 * E.b**2 - E.a**3), x**0

def schoof(F, E):
  p = [5]
  N = 5
  t = {}
  while N < math.sqrt(F.n) * 4:
    np = int(gmpy.next_prime(p[-1]))
    p += [np]
    N *= np
  q = F.p ** F.degree()
  PR = BivariatePolynomialRing(F, ['x', 'y'])
  PU = UnivariatePolynomialRing(F, 'xs')
  x, y = PR.gens()
  Fx = x**3 + E.a*x + E.b
  xs = PU.gen()
  pol_p, pol_q = torsion_polynomial(13, E, x, y)
  pol_p.trim()
  pol_q.trim()
  pol = y_div(pol_p, pol_q)
  pol = y2_reduce(pol, x, y, Fx)
  print(pol)



if __name__ == '__main__':
  p = 182687704666362864775460604089535377456991567941
  F = FiniteField(p)
  E = EllipticCurve(F, 4, 1)
  print(schoof(F, E))

