from ecpy import EllipticCurve, FiniteField, gcd, modinv

# secp256k1 curve
p = 2**256 - 2**32 - 2**9 - 2**8 - 2**7 - 2**6 - 2**4 - 1
a = 0
b = 7
n = 0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141
secret = 0xdeadbeef

F = FiniteField(p)
E = EllipticCurve(F, a, b)
G = E(0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798, 0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8)
Q = secret * G

public_info = (E, n, G)
public_key = (Q, )
secret_key = (secret, )

def hash_int(m):
  '''
  Hashing message and convert to integer
  Args:
    m: the message(as string)
  Return:
    SHA-512 hashed message as integer
  '''
  import hashlib
  return int(hashlib.sha512(m).hexdigest(), 16)

def ECDSA_sign(m, public_info, secret_key):
  import random
  E, n, G = public_info
  dA,  = secret_key
  e = hash_int(m)
  z = e >> (e.bit_length() - n.bit_length())
  while True:
    k = random.randint(1, n - 1)
    if gcd(k, n) == 1:
      break
  r = int((k * G).x)
  s = (modinv(k, n) * ((z + r * dA) % n)) % n
  return (r, s)

def ECDSA_verify(m, signature, public_info, public_key):
  E, n, G = public_info
  Q, = public_key
  r, s = signature
  assert 1 <= r <= n-1
  assert 1 <= s <= n-1
  e = hash_int(m)
  z = e >> (e.bit_length() - n.bit_length())
  w = modinv(s, n)
  u1 = (z * w) % n
  u2 = (r * w) % n
  if (u1 * G + u2 * Q).x == r:
    return True
  else:
    return False

if __name__ == '__main__':
  import os
  print '=== ECDSA Tester ==='
  message = os.urandom(16).encode('base64')
  print '[+] to sign message: %s' % message
  signature = ECDSA_sign(message, public_info, secret_key)
  print '[+] Signature: %r' % (signature, )
  print '[+] Verify : Correct message => %s' % ECDSA_verify(message, signature, public_info, public_key)
  print '[+] Verify : Invalid message => %s' % ECDSA_verify('hogefugathisisinvalidmessage', signature, public_info, public_key)
