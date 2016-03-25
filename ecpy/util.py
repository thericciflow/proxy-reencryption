import random

class ModinvNotFoundError(ArithmeticError):
  pass

def memoize(f):
  cache = {}
  def helper(*args):
    if args not in cache:
      cache[args] = f(*args)
    return cache[args]
  return helper

def gcd(x, y):
  while y != 0:
    t = x % y
    x, y = y, t
  return x

def gcd_multiple(*a):
  return reduce(gcd, a)

def egcd(a, b):
  x, y, u, v = 0, 1, 1, 0
  while a != 0:
    q, r = b // a, b % a
    m, n = x - u * q, y - v * q
    b, a, x, y, u, v = a, r, u, v, m, n
  g = b
  return (g, x, y)

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

try:
  import gmpy
  is_prime = gmpy.is_prime
  print "[+] found gmpy! use gmpy.is_prime"
except:
  is_prime = miller_rabin

