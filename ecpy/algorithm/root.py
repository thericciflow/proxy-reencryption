from ..util import is_prime, legendre_symbol
from ..structure.ExtendedFiniteField import ExtendedFiniteFieldElement
import random


def modular_square_root(a, m, force=False):
  """
  Calculate Quadratic Residue
  """
  if isinstance(a, ExtendedFiniteFieldElement) and not force:
    return modular_square_root_extended(a)
  if is_prime(m):
    return tonelli_shanks(a, m)
  if m == 2:
    return a
  if m % 4 == 3:
    r = pow(a, (m + 1) / 4, m)
    return [r, m - r]
  if m % 8 == 5:
    v = pow(2 * a, (m - 5) / 8, m)
    i = pow(2 * a * v, 2, m)
    r = a * v * (i - 1) % m
    return [r, m - r]
  if m % 8 == 1:
    e = 0
    while True:
      k = 2**(e + 1)
      if (m - 1) % k != 0:
        break
      e += 1
    q = (m - 1) / 2**e
    while True:
      x = random.randint(1, m)
      z = pow(x, q, m)
      if pow(z, 2**(e - 1), m) != 1:
        break
    y = z
    r = e
    x = pow(a, (q - 1) / 2, m)
    v = a * x % m
    w = v * x % m
    while True:
      if w == 1:
        return [v, m - v]
      k = 0
      while True:
        k += 1
        if pow(w, 2**k, m) == 1:
          break
      d = pow(y, 2**(r - k - 1), m)
      y = pow(d, 2, m)
      r = k
      v = d * v % m
      w = w * y % m


def tonelli_shanks(n, p):
  """
  Tonelli-Shanks calculate quadratic residue algorithm implementation
  """
  if p % 4 == 3:
    r = pow(n, (p + 1) / 4, p)
    return [r, p - r]
  s = 0
  while True:
    k = 2**(s + 1)
    if (p - 1) % k != 0:
      break
    s += 1
  q = (p - 1) / 2**s
  while True:
    z = random.randint(1, p)
    if legendre_symbol(z, p) == -1:
      break
  c = pow(z, q, p)
  r = pow(n, (q + 1) / 2, p)
  t = pow(n, q, p)
  m = s
  while True:
    if t % p == 1:
      return [r, p - r]
    i = 1
    while True:
      if pow(t, 2 ** i, p) == 1:
        break
      i += 1
    b = pow(c, 2 ** (m - i - 1), p)
    r = (r * b) % p
    t = (t * (b**2)) % p
    c = pow(b, 2, p)
    m = i


def extended_legendre_symbol(a):
  """
  Legendre Symbol on the Extended Field
  """
  m = a.field.degree()
  p = a.field.p
  b = pow(a, sum([p**i for i in xrange(0, m)]), p)
  return legendre_symbol(b, p)


def modular_square_root_extended(x):
  """
  Calculate Quadratic Residue on Extended Field
  """
  if extended_legendre_symbol(x) != 1:
    return []
  a = x

  m = a.field.degree()
  p = a.field.p
  q = p**(m / 2)
  if m % 2 == 0:
    if pow(q, m / 2, 4) == 1:
      c0 = 1
      while c0 == 1:
        c = x.field(random.randint(0, q**2), random.randint(0, q**2))
        c0 = extended_legendre_symbol(c)
      d = pow(c, (q - 1) / 2)
      e = 1 / (c * d)
      f = (c * d)**2

      b = pow(a, (q - 1) / 4)
      a0 = (b**2) ** q * b**2
      if a0 == -1:
        return []
      if b ** q * b == 1:
        x0 = modular_square_root(b**2 * a, q, force=True)[0]
        x = x0 * b ** q
      else:
        x0 = modular_square_root(b**2 * a * f, q, force=True)[0]
        x = x0 * b ** q * e
      return [x]
    elif pow(q, m / 2, 4) == 3:
      pass
    else:
      pass
  else:
    raise NotImplementedError("If you want to use function, please implement "
                              "Shanks, Atkin, Kong et al, Mueller, Tonelli-"
                              "Shanks, ... algorithm.")
  raise NotImplementedError()


def cubic_root(x):
  """
  Calculate Cubic Residue
  """
  F = x.field
  p = F.p
  m = F.degree()
  rho = 1
  pm = p**m
  r = (pm - 1) / 3
  while True:
    rho = F(random.randint(1, p - 1), random.randint(1, p - 1))
    if rho ** r != 1:
      break
  t = 1
  while True:
    r = 3**t
    if (pm - 1) % r == 0:
      t += 1
    else:
      t -= 1
      s = (pm - 1) / 3**t
      if (s + 1) % 3 == 0:
        l = (s + 1) / 3
        break
      elif (s - 1) % 3 == 0:
        l = (s - 1) / 3
        break
  a = rho ** s
  a_ = rho ** (3**(t - 1) * s)
  b = x ** s
  h = 1
  i = 1
  while i < t:
    d = b ** (3**(t - 1 - i))
    if d == 1:
      k = 0
    elif d == a_:
      k = 2
    else:
      k = 1
    b = b * (a * a * a)**k
    h = h * a**k
    a = a * a * a
    i += 1
  r = x**l * h
  if s == 3 * l + 1:
    r = 1 / r
  return r
