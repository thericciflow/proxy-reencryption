import operator as op
import random
import sys


def memoize(f):
  cache = {}

  def helper(*args):
    if args not in cache:
      cache[args] = f(*args)
    return cache[args]

  return helper


def gcd(x, y):
  """
  Calculate greatest common divisor
  """
  while y != 0:
      t = x % y
      x, y = y, t
  return x


def gcd_multiple(*a):
  """
  Apply gcd to some variables.
  Args: 
    a: args list
  """
  return reduce(gcd, a)


def egcd(a, b):
  """
  Calculate Extended-gcd
  """
  x, y, u, v = 0, 1, 1, 0
  while a != 0:
    q, r = b // a, b % a
    m, n = x - u * q, y - v * q
    b, a, x, y, u, v = a, r, u, v, m, n
  return (b, x, y)


def __modinv(a, m):
  if gcd(a, m) != 1:
    return 0
  if a < 0:
    a += m
  return egcd(a, m)[1] % m


def lcm(*a):
  """
  Calculate Least Common Multiple
  Args:
    *a: args list
  """
  return reduce(op.mul, a) / gcd_multiple(*a)

def crt(ak, nk):
  """
  Chinese-Reminders-Theorem Implementation
  using Gauss's proof and generalization on gcd(n1, n2) != 1
  Should be len(ak) == len(nk)
  Original: https://gist.github.com/elliptic-shiho/901d223135965308a5f9ff0cf99dd7c8
  Explanation: http://elliptic-shiho.hatenablog.com/entry/2016/04/03/020117

  Args:
    ak: A Numbers [a1, a2, ..., ak]
    nk: A Modulus [n1, n2, ..., nk]
  """
  assert len(ak) == len(nk)
  N = reduce(lambda x, y: x * y, nk, 1)
  l = lcm(*nk)
  s = 0
  for n, a in zip(nk, ak):
    m = N / n
    g, x, y = egcd(m, n)
    s += (m / g) * x * a
    s %= l
  return s


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
  j = 1
  while a != 0:
    while not a % 2:
      a /= 2
      if n & 7 == 3 or n & 7 == 5:
        j = -j
    a, n = n, a
    if a & 3 == 3 and n & 3 == 3:
      j = -j
    a %= n
  if n:
    return j
  return 0


def miller_rabin(x):
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
except ImportError:
  _is_prime = miller_rabin

@memoize
def is_prime(x):
  """
  Is x prime?
  Args:
    x: Test Number (should be positive integer)
  """
  return _is_prime(x)


def modinv(a, n):
  """
  Calculate Modular Inverse.
  - Find x satisfy ax \equiv 1 \mod m

  Args:
    a: target number
    n: modulus
  """
  return __modinv(a, n)
