from sage.all import *

def distortion_map(E, F, P):
  w = F.gen()
  x, y = P.xy()
  return E(w*x, y)

def modified_pairing(E, F, P, Q, m):
  Q = distortion_map(E, F, Q)
  return P.weil_pairing(Q, m)

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
  print P._miller_(distortion_map(E, F, P), m)
  g = modified_pairing(E, F, P, P, m)
  print "base g = %r" % g
  assert_eq(modified_pairing(E, F,   P, 2*P, m), g^2, "e(P, 2P) == g^2")
  assert_eq(modified_pairing(E, F, 2*P,   P, m), g^2, "e(2P, P) == g^2")
  assert_eq(modified_pairing(E, F,   P,   P, m)^2, g^2, "e(P, P)^2== g^2")
  print g^2

  prev_r = g

  for x in xrange(10):
    r1 = ZZ.random_element(0, p)
    r2 = ZZ.random_element(0, p)
    r = r1 + F.gen() * r2
    """
    print "[+] r = %s" % r
    print "r^(p^2) == r?: %s" % (r^p^2 == r)
    print "[+] r/prev_r = %s" % (r/prev_r)
    """
    vr = vector(r)
    vpr= vector(prev_r)
    vrpr=vector(r/prev_r)
    print "assert_eq(F(%s, %s)/F(%s, %s), F(%s, %s), \"r/prev_r test: %d\")" % (vr[0], vr[1], vpr[0], vpr[1], vrpr[0], vrpr[1], x)
    prev_r = r

  w = F.gen()
  x = 338714 + 3*w
  print "[+] x = %s" % x
  vx = vector(x)
  vix= vector(1/x)
  print "assert_eq(1/F(%s, %s), F(%s, %s), \"1/x\")" % (vx[0], vx[1], vix[0], vix[1])


