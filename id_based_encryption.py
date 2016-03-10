from ecpy import *
import hashlib
import random
import gmpy

secret = 0xdeadbeef

def hash_to_point(E, m, l):
  h = int(E.field(int(hashlib.sha1(m).hexdigest(), 16)))
  while True:
    r = E.get_corresponding_y(h)
    if r != None:
      P = E(h, r[0])
      if (P*l).is_infinity():
        return P
    h += 1

def hash_to_field(F, m):
  d = hex(m)[2:].replace("L", "")
  if len(d) % 2 == 1:
    d = "0" + d
  return int(E.field(int(hashlib.sha512(d.decode("hex")).hexdigest(), 16)))

def gen_supersingular_ec():
  def gen_prime():
    while True:
      p = int(gmpy.next_prime(random.randint(2**70, 2**71)))
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
      P = E(i, r[0])
      if (P*l).is_infinity():
        return P
    i += 1

def get_user_key(E, id):
  global secret
  P = hash_to_point(E, id)
  return P, secret * P

def get_public_key(E):
  global secret
  P = E((36, 0), (60, 0))
  return P, secret * P

def encrypt(E, P, sP, id, m):
  assert isinstance(m, int) or isinstance(m, long)
  r = random.randint(2**16, 2**32)
  return r*P, m ^ hash_to_field(E.field, int(E.field(weil_pairing(E, get_user_key(E, id)[0], sP, 5)) ** r))

def decrypt(E, P, sP, id, c):
  return c[1] ^ hash_to_field(E.field, weil_pairing(E, get_user_key(E, id)[1], c[0], 5))

def modified_pairing(E, P, Q, m):
  return tate_pairing(E, P, Q.distortion_map(), m)

if __name__ == "__main__":
  E, F, l = gen_supersingular_ec()
  print E
  P = get_point(E, l)
  Q = hash_to_point(E, "aaaa", l)
  print Q
  print modified_pairing(E, P, Q, l) ** 2
  print modified_pairing(E, 2*P, Q, l)
  print modified_pairing(E, P, 2*Q, l)

