import random

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
  if a % m == 0:
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
  if gcd(a, p) != 1:
    return 0
  d = pow(a, (p-1)/2, p)
  if d == p-1:
    return -1
  return 1

def is_prime(x):
    return miller_rabin(x)

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
