from sage.all import *

def distortion_map(E, F, P):
  w = F.gen()
  x, y = P.xy()
  return E(w*x, y)

def modified_pairing(E, F, P, Q, m):
  Q = distortion_map(E, F, Q)
  return P.tate_pairing(Q, m, 2)

def assert_eq(a, b, m):
  print "[+] %s..."%m, 
  if a == b:
    print "  OK."
  else:
    print "Fail."

if __name__ == "__main__":
  l = 56453
  m = l
  p = l*6-1
  F = GF(p^2, "w", modulus=x^2+x+1)
  print F
  E = EllipticCurve(F, [0, 1])
  print E
  i = 3
  while True:
    y_square = F(i)^3+1
    if y_square.is_square():
      y = y_square.square_root()
      P = E(i, y)
      if l*P == E(0, 1, 0):
        break
  print P
  print distortion_map(E, F, P)
  g = modified_pairing(E, F, P, P, m)
  print "base g = %r" % g
  assert_eq(modified_pairing(E, F,   P, 2*P, m), g^2, "e(P, 2P) == g^2")
  assert_eq(modified_pairing(E, F, 2*P,   P, m), g^2, "e(2P, P) == g^2")
  assert_eq(modified_pairing(E, F,   P,   P, m)^2, g^2, "e(P, P)^2== g^2")
  print g^2
