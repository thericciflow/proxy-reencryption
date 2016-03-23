from ecpy import *
import hashlib
import random
import gmpy

secret = 0xcafebabe

def hash_to_point(E, m, l):
  h = int(E.field(int(hashlib.sha1(m).hexdigest(), 16)))
  while True:
    r = E.get_corresponding_y(h)
    if r != None:
      P = E(h, r)
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
      p = int(gmpy.next_prime(random.randint(2**31, 2**32)))
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
      if (P*l).is_infinity():
        return P
    i += 1

def get_user_public(E, id, l):
  return hash_to_point(E, id, l)

def get_user_secret(E, id, l):
  global secret
  return get_user_public(E, id, l) * secret

def encrypt(E, P, sP, pubkey, m, l):
  assert isinstance(m, int) or isinstance(m, long)
  r = random.randint(2**30, 2**31)
  return r*P, m * modified_pairing(E, pubkey, sP, l) ** r

def master_point(E, l):
  global secret
  P = get_point(E, l)
  return P, secret * P

def decrypt(E, K, c, l):
  return c[1] / modified_pairing(E, c[0], K, l)

def modified_pairing(E, P, Q, m):
  return weil_pairing(E, P, Q.distortion_map(), m)

if __name__ == "__main__":
  E, F, l = gen_supersingular_ec()
  print E
  P, sP = master_point(E, l)
  C = encrypt(E, P, sP, get_user_public(E, "@elliptic_shiho", l), 12345, l)
  C1, C2 = C
  print C1
  print C2
  print decrypt(E, get_user_secret(E, "@elliptic_shiho", l), C, l)

