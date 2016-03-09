from ecpy import *
from random import randint

ac_count = 0
wa_count = 0

def _assert(a, b, msg, cond):
  global ac_count, wa_count
  msg = msg.ljust(16)
  print ("[+] %s..." % (msg)).ljust(30),
  var = {"a":a, "b":b}
  if eval("a %s b" % cond, var):
    print "\x1b[33m[  OK  ]\x1b[0m %r" % (b, )
    ac_count += 1
  else:
    print "\x1b[31m[ Fail ]\x1b[0m Expected: %r, Result: %r" % (b, a)
    wa_count += 1

def assert_neq(a, b, m):
  _assert(a, b, m, "!=")

def assert_eq(a, b, m):
  _assert(a, b, m, "==")

if __name__ == "__main__":
  F = FiniteField(101)
  x = F(2)
  y = F(203)
  assert_neq(x, 1, "x != 1")
  assert_eq(x, 2, "x == 2")

  F = FiniteField(5)
  x = F(3)
  y = F(7) # = 2
  print F
  print "[+] x, y = %s, %s" % (x, y)
  assert_eq(x+y, F(0), "x+y == F(0)")
  assert_eq(x+y, 0, "x+y == 0")
  assert_eq(x-y, 1, "x-y == 1")
  assert_eq(x*y, 1, "x*y == 1")
  x = F(2)
  y = F(3)
  print "[+] x, y = %s, %s" % (x, y)
  # commutive!
  assert_eq(1/x, y, "1/x == y")
  assert_eq(util.modinv(x.x, F.p), y, "modinv(x) == y")
  assert_eq(1/y, x, "1/y == x")

  assert_eq(x**3, y, "x^3 == y")

  assert_eq(util.crt([3, 4], [4, 9]), 31, "CRT Test")
  #assert_eq(util.crt([7, 13], [12, 18]), 31, "CRT Test 2")

  assert_eq(F.order(), 4, "|F| = 4")
  assert_eq(x.order(), 4, "|x| = 4")

  F = FiniteField(17)
  E = EllipticCurve(F, 1, 0)

  P = E(1, 6)
  Q = E(11, 4)
  print "P, Q = %r, %r" % (P, Q)
  assert_eq(P+Q, E(3, 8), "P+Q")
  assert_eq(P+P, E(0, 0), "P+P")
  assert_eq(P*2, E(0, 0), "P*2")
  assert_eq(2*P, E(0, 0), "2*P")
  assert_eq(P.order(), 4, "|P| = 4")

  print "Random Test: "
  i = 0
  while i < 10:
    while True:
      r = randint(-50, 50)
      if r != 0:
        break
    print "[+] random 1 = %d" % r
    assert_eq((util.modinv(r, 101) * r) % 101, 1, "modinv")
    while True:
      q = randint(-50, 50)
      if q != 0:
        break
    print "[+] random 2 = %d" % q
    assert_eq(r*(q*P), q*(r*P), "ECDH test")
    i += 1

  # The arithmetic of elliptic curves: p.397 example of miller algorithm
  F = FiniteField(631)
  E = EllipticCurve(F, 30, 34)
  m = 5

  P = E(36, 60)
  Q = E(121, 387)
  S = E(0, 36)

  print "P, Q, S = %r, %r, %r" % (P, Q, S)
  assert_eq(E.embedding_degree(m), 1, "embed degree")
  assert_eq(miller(E, P, Q+S, m), 103, "miller(P, Q+S)")
  assert_eq(miller(E, P, S, m), 219, "miller(P, S)")
  assert_eq(miller(E, Q, P-S, m), 284, "miller(Q, P-S)")
  assert_eq(miller(E, Q, -S, m), 204, "miller(Q, -S)")
  assert_eq(weil_pairing(E, P, Q, m, S), 242, "weil_pairing")
  assert_eq(tate_pairing(E, P, Q, m), 279, "tate_pairing")

  print "[+] SSSA-Attack"
  F = FiniteField(16857450949524777441941817393974784044780411511252189319)

  A = 16857450949524777441941817393974784044780411507861094535
  B = 77986137112576

  E = EllipticCurve(F, A, B)

  print "Random Point"
  for x in xrange(10):
    print E.random_point()


  P = E(5732560139258194764535999929325388041568732716579308775, 14532336890195013837874850588152996214121327870156054248)
  Q = E(2609506039090139098835068603396546214836589143940493046, 8637771092812212464887027788957801177574860926032421582)

  assert_eq(SSSA_Attack(F, E, P, Q), 6418297401790414921245298353021109182464447715712434176, "SSSA-Attack")

  z = CC(1, 2) # 1+2i
  w = CC(5, 1) # 5+i
  print "z, w = %r, %r" % (z, w)
  assert_eq(z+w, CC(6, 3), "z+w")
  assert_eq(z-w, CC(-4, 1), "z-w")
  assert_eq(z*w, CC(3, 11), "z*w")
  assert_eq(z/w, CC(0.2692307692307693, 0.34615384615384615), "z/w")

  F = ExtendedFiniteField(59)
  a = F(0, 1)
  E = EllipticCurve(F, 1, 0)
  P = E(25, 30)
  assert_eq(tuple(P), (25, 30, 1), "extended field EC")
  Q = P.distortion_map()
  assert_eq(tuple(Q), (F(34), F(0, 30), 1), "extended field EC 2")
  assert_eq(Q.distortion_map(), P, "distortion map")

  F = ExtendedFiniteField(353, "x^2+x+1")

  E = EllipticCurve(F, 0, 1)
  while True:
    P = E.random_point()
    o = P.order()
    print o
    if o == 11:
      break
  Q = P.distortion_map()
  print P, Q
  m = 11
  g = tate_pairing(E, P, Q, m)

  assert_eq(tate_pairing(E,   P, 2*Q, m), g**2, "e(P, 2Q) == g^2")
  assert_eq(tate_pairing(E, 2*P,   Q, m), g**2, "e(2P, Q) == g^2")
  assert_eq(tate_pairing(E,   P,   Q, m)**2, g**2, "e(P, Q)^2 == g^2")

  print "[+] %d Test(s) finished. %d Test(s) success, %d Test(s) fail." % (ac_count + wa_count, ac_count, wa_count)
