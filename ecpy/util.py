import random

try:
  import gmpy
  is_prime = gmpy.is_prime
  print "[+] found gmpy! use gmpy.is_prime"
except:
  is_prime = miller_rabin

class ModinvNotFoundError(ArithmeticError):
  pass

def gcd(x, y):
  while y != 0:
    t = x % y
    x, y = y, t
  return x

def gcd_multiple(*a):
  return reduce(gcd, a)

def egcd(x, y):
  if x == 0:
    return (y, 0, 1)
  a, b = x, y
  g, y, x = egcd(y%x, x)
  return (g, x-(b/a)*y, y)

def modinv(a, m):
  if gcd(a, m) != 1:
    raise ModinvNotFoundError()
  a %= m
  return egcd(a, m)[1] % m

def lcm(*a):
  return reduce(lambda x,y: x*y, a, 1)/gcd_multiple(*a)

def crt(a, n):
  N = reduce(lambda x,y: x*y, n, 1)
  if gcd_multiple(*n) > 1:
    # TODO: implement in gcd(modulo) > 0
    print "Warning: gcd(modulo) > 1"
    return None
    print new_n
    n = new_n
  nk = map(lambda x: N/x, n)
  ik = map(lambda x: modinv(x[0], x[1]), zip(nk, n))
  return sum(map(lambda x: (x[0]*x[1]*x[2]) % N, zip(a, nk, ik))) % lcm(*n)

def legendre_symbol(a, p):
  assert is_prime(p)
  if gcd(a, p) != 1:
    return 0
  d = pow(a, ((p-1)/2), p)
  if d == p-1:
    return -1
  return 1

def jacobi_symbol(a, n):
  if is_prime(n):
    return legendre_symbol(a, n)
  if a == 0:
    if n == 1:
      return 1
    else:
      return 0
    if a % 2 == 1:
        return (-1) ** ( (a-1)*(n-1) / 4) * jacobi_symbol(n%a, a)
    else:
        return (-1) ** ( (n*n-1) / 8) * jacobi_symbol(a/2, n)

def miller_rabin(x):
  s = 0
  while (x-1) % 2**(s+1) == 0:
    s += 1
  d = x / (2**s)
  prime = 0
  for i in xrange(10): # k = 10
    a = random.randint(1, x-1)
    if not (pow(a, d, x) != 1 and all([pow(a, 2**r * d, x) != x-1 for r in xrange(0, s)])):
      prime += 1
  return prime > 6

def modular_square_root(a, m, force=False):
  from .structure.ExtendedFiniteField import ExtendedFiniteFieldElement
  if isinstance(a, ExtendedFiniteFieldElement) and not force:
    return modular_square_roots_extended(a)
  if is_prime(m):
    return tonelli_shanks(a, m)
  if m == 2:
    return a
  if m % 4 == 3:
    r = pow(a, (m+1) / 4, m)
    return [r, m-r]
  if m % 8 == 5:
    v = pow(2*a, (m-5) / 8, m)
    i = pow(2*a*v, 2, m)
    r = a*v*(i-1) % m
    return [r, m-r]
  if m % 8 == 1:
    e = 0
    while True:
      k = 2**(e+1)
      if (m-1) % k != 0:
        break
      e += 1
    q = (m-1)/2**e
    while True:
      x = random.randint(1, m)
      z = pow(x, q, m)
      if pow(z, 2**(e-1), m) != 1:
        break
    y = z
    r = e
    x = pow(a, (q-1)/2, m)
    v = a*x % m
    w = v*x % m
    while True:
      if w == 1:
        return [v, m-v]
      k = 0
      while True:
        k += 1
        if pow(w, 2**k, m) == 1:
          break
      d = pow(y, 2**(r-k-1), m)
      y = pow(d, 2, m)
      r = k
      v = d*v % m
      w = w*y % m

def tonelli_shanks(n, p):
  assert is_prime(p)
  assert legendre_symbol(n, p)
  if p % 4 == 3:
    r = pow(n, (p+1)/4, p)
    return [r, p-r]
  s = 0
  while True:
    k = 2**(s+1)
    if (p-1) % k != 0:
      break
    s += 1
  q = (p-1)/2**s
  while True:
    z = random.randint(1, p)
    if legendre_symbol(z, p) == -1:
      break
  c = pow(z, q, p)
  r = pow(n, (q+1)/2, p)
  t = pow(n, q, p)
  m = s
  while True:
    if t % p == 1:
      return [r, p-r]
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
  m = a.field.degree()
  p = a.field.p
  b = pow(a, sum([p**i for i in xrange(0, m)]), p)
  return legendre_symbol(b, p)

def modular_square_roots_extended(x):
  print "[+] start"
  if extended_legendre_symbol(x) != 1:
    return []
  a = x

  m = a.field.degree()
  p = a.field.p
  q = p**(m/2)
  if m % 2 == 0:
    if pow(q, m/2, 4) == 1:
      c0 = 1
      while c0 == 1:
        c = x.field(random.randint(0, q**2), random.randint(0, q**2))
        c0 = extended_legendre_symbol(c)
      d = pow(c, (q-1)/2)
      e = 1/(c*d)
      f = (c*d)**2

      b = pow(a, (q-1)/4)
      a0 = (b**2) ** q * b**2
      if a0 == -1:
        return []
      if b ** q * b == 1:
        x0 = modular_square_root(b**2 * a, q, force=True)[0]
        x = x0 * b ** q
      else:
        x0 = modular_square_root(b**2*a*f, q, force=True)[0]
        x = x0 * b ** q * e
      print "[+] end"
      return [x]
    elif pow(q, m/2, 4) == 3:
      pass
    else:
      pass
  else:
    raise NotImplementedError("If you want to use function, please implement Shanks, Atkin, Kong et al, Mueller, Tonelli-Shanks, ... algorithm.")
  raise NotImplementedError()
