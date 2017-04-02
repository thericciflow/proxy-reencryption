from __future__ import print_function, division
from ecpy import *
import math
import gmpy

def calculate_order(E):
  return None

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
      return (pp0 * (pp2 * pm1**2 - pm2 * pp1**2)) / 2 * y
    else:
      n = (n-1) // 2
      psi = torsion_polynomial
      pp2 = psi(n + 2, E, x, y)
      pp1 = psi(n + 1, E, x, y)
      pp0 = psi(n, E, x, y)
      pm1 = psi(n - 1, E, x, y)
      return (pp2 * pp0**3 - pp1**3 * pm1)
  if n == -1:
    return -1
  elif n == 0:
    return 0
  elif n == 1:
    return 1
  elif n == 2:
    return 2 * y
  elif n == 3:
    return 3 * X**4 + 6 * E.a * x**2 + 12 * E.b * x - E.a**2
  elif n == 4:
    return 4 * y * (x**6  + 5 * E.a * x**4 + 20 * E.b * x**3  - 5 * E.a ** 2 * x ** 2 - 4 * E.a * E.b * x - 8 * E.b**2 - E.a**3)

def schoof(F, E):
  p = [5]
  N = 5
  t = {}
  while N < math.sqrt(F.n) * 4:
    np = int(gmpy.next_prime(p[-1]))
    p += [np]
    N *= np
  q = F.p ** F.degree()
  for l in p:
    P = find_point_by_order(E, l)
    qx, qy = (P.x ** q, P.y ** q)
    q2x, q2y = (P.x ** (q**2), P.y ** (q**2))
    qbar = q % l - ((l-1)//2)
    qbarP = qbar * P
    qbarx, qbary = (qbarP.x, qbarP.y)
    if q2x != qbarx:
      X, Y, _ = tuple(E(q2x, q2y) + qbarP)
      for tbar in range(1, (l-1)//2):
        tbarP = tbar * P
        tqx, tqy = (tbarP.x**q, tbarP.y**q)
        if X == tqx:
          if Y == tqy:
            t[l] = tbar
            break
          else:
            t[l] = -tbar
            break
    elif legendre_symbol(q % l, l):
      w = modular_square_root(q % l, l)[0]
      wqx, wqy, _ = w * E(qx, qy)
      if wqx == q2x:
        if wqy == q2y:
          t[l] = 2*int(w)
        elif wqy == -q2y:
          t[l] = -2*int(w)
        else:
          t[l] = 0
      else:
        t[l] = 0
    else:
      t[l] = 0
  nk = sorted(t.keys())
  ak = [t[n] for n in nk]
  print(t)
  t = crt(ak, nk)
  print(N)
  print(t)
  return q + 1 - t



if __name__ == '__main__':
  p = 182687704666362864775460604089535377456991567941
  F = FiniteField(p)
  E = EllipticCurve(F, 4, 1)
  print(schoof(F, E))

