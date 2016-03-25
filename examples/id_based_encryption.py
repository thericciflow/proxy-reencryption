from ecpy import *
import hashlib
import random

secret = 0xdeadbeef

def hash_to_point(E, m, l):
  h = int(E.field(int(hashlib.sha512(m).hexdigest(), 16)))
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
  print "[+] p = %d" % p
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

def get_user_public(E, P, id, l):
  return P * hash_to_field(E.field, int(id.encode("hex"), 16))

def get_user_secret(E, pubkey, l):
  global secret
  return pubkey * secret

def encrypt(E, P, sP, pubkey, m, l):
  assert isinstance(m, (int, long))
  r = random.randint(2**30, 2**31)
  return r*P, m ^ H2(modified_pairing(E, pubkey, sP, l) ** r)

def master_point(E, l):
  global secret
  P = get_point(E, l)
  return P, secret * P

def decrypt(E, K, c, l):
  return c[1] ^ H2(modified_pairing(E, c[0], K, l))

def modified_pairing(E, P, Q, m):
  return tate_pairing(E, P, Q.distortion_map(), m, 2)

def main():
  E, F, l = gen_supersingular_ec()
  P, sP = master_point(E, l)
  ID = "@elliptic_shiho"
  Q = get_user_public(E, P, ID, l)
  sQ = get_user_secret(E, Q, l)
  C = encrypt(E, P, sP, Q, 12345, l)
  print C
  C1, C2 = C
  r = 1143
  g = modified_pairing(E, P, Q, l)
  print g
  print decrypt(E, sQ, C, l)


if __name__ == "__main__":
  main()
