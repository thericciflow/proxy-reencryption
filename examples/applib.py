from ecpy import *
import hashlib
import random

def MapToPoint(E, y):
  x = cubic_root(y**2 - 1)
  Q = E(x, y)
  return 6*Q

def hash_to_point(E, m, l):
  h = E.field(int(hashlib.sha512(m).hexdigest(), 16))
  return MapToPoint(E, h)

def hash_to_field(F, m):
  d = hex(m)[2:].replace("L", "")
  if len(d) % 2 == 1:
    d = "0" + d
  return int(F(int(hashlib.sha512(d.decode("hex")).hexdigest(), 16)))

def H2(x):
  return x.x * x.field.p + x.y

def gen_supersingular_ec():
  def _next_prime(n):
    while not util.is_prime(n):
      n += 1
    return n
  try:
    from gmpy import next_prime
  except:
    next_prime = _next_prime
  def gen_prime():
    while True:
      p = int(next_prime(random.randint(2**511, 2**512)))
      if util.is_prime(p*6-1):
        break
    return p*6-1, p

  p, l = gen_prime()
  F = ExtendedFiniteField(p, "x^2+x+1")
  return EllipticCurve(F, 0, 1), F, l

def get_point(E, l):
  i = 3
  while True:
    r = E.get_corresponding_y(i)
    if r != None:
      P = E(i, r)
      pl = P*l
      if pl.is_infinity():
        return P
    i += 1

def modified_pairing(E, P, Q, m):
  return tate_pairing(E, P, Q.distortion_map(), m, 2)

