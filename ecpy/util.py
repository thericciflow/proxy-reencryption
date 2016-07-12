import random
import sys

def memoize(f):
  """
  Auto-Memoize Decorator.
  """
  cache = {}

  def helper(*args):
    if args not in cache:
      cache[args] = f(*args)
    return cache[args]

  return helper


def gcd(x, y):
  """
  Calculate Greatest Common Divisor using Euclidean Algorithm.
  """
  while y != 0:
    t = x % y
    x, y = y, t
  return x


def gcd_multiple(*a):
  """
  Apply gcd to some variables.
  """
  return reduce(gcd, a)


def egcd(a, b):
  """
  Calculate x, y satisfy ax + by = gcd(a, b) using Extended Euclidean Algorithm
  """
  x, y, u, v = 0, 1, 1, 0
  while a != 0:
    q, r = b // a, b % a
    m, n = x - u * q, y - v * q
    b, a, x, y, u, v = a, r, u, v, m, n
  return (b, x, y)


def __modinv(a, m):
  """
  Calculate Modular Inverse. i.e. Find x satisfy ax \equiv 1 \mod m.
  """
  if gcd(a, m) != 1:
    return 0
  if a < 0:
    a += m
  return egcd(a, m)[1] % m


def lcm(*a):
  """
  Calculate Least Common Multiple.
  """
  return reduce(lambda x, y: x * y, a, 1) / gcd_multiple(*a)


def crt(a, n):
  """
  Calculate Chinese-Reminder-Theorem. by proof of the gauss.
  """
  N = reduce(lambda x, y: x * y, n, 1)
  if gcd_multiple(*n) > 1:
    # TODO: implement in gcd(modulo) > 0
    print "Warning: gcd(modulo) > 1"
    return None
  nk = map(lambda x: N / x, n)
  ik = map(lambda x: modinv(x[0], x[1]), zip(nk, n))
  return sum(map(lambda x: (x[0] * x[1] * x[2]) % N, zip(a, nk, ik))) % lcm(*n)


def legendre_symbol(a, p):
  """
  Calculate Legendre Symbol using Euler's criterion
  """
  if gcd(a, p) != 1:
    return 0
  d = pow(a, ((p - 1) / 2), p)
  if d == p - 1:
    return -1
  return 1


def jacobi_symbol(a, n):
  """
  Calculate Jacobi Symbol.
  """
  if is_prime(n):
    return legendre_symbol(a, n)
  if a == 0:
    if n == 1:
      return 1
    else:
      return 0
    if a % 2 == 1:
        return (-1) ** ((a - 1) * (n - 1) / 4) * jacobi_symbol(n % a, a)
    else:
        return (-1) ** ((n * n - 1) / 8) * jacobi_symbol(a / 2, n)


def miller_rabin(x):
  """
  Miller-Rabin Primarity test implementation.
  """
  s = 0
  while (x - 1) % 2**(s + 1) == 0:
    s += 1
  d = x / (2**s)
  prime = 0
  for i in xrange(10):  # k = 10
    a = random.randint(1, x - 1)
    if not (pow(a, d, x) != 1 and all([pow(a, 2**r * d, x) != x - 1
            for r in xrange(0, s)])):
      prime += 1
  return prime > 6


try:
  import gmpy
  _is_prime = gmpy.is_prime
  sys.stderr.write("[+] found gmpy! use gmpy.is_prime\n")
except:
  _is_prime = miller_rabin

try:
  import ecpy_native
  sys.stderr.write("[+] Native Module Enabled.\n")
  _modinv = ecpy_native.modinv
  _modular_square_root = ecpy_native.modular_square_root
  enable_native_module = True
except:
  from ecpy.algorithm.root import __modular_square_root
  _modinv = __modinv
  _modular_square_root = __modular_square_root
  enable_native_module = False

@memoize
def is_prime(x):
  """
  Is x prime?
  """
  return _is_prime(x)

def modinv(a, n):
  return _modinv(a, n)
