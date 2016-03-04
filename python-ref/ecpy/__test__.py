import util
from FiniteField import FiniteField
from EllipticCurve import EllipticCurve
from random import randint

ac_count = 0
wa_count = 0

def _assert(a, b, cond):
  global ac_count, wa_count
  print ("[+] %r %s %r..." % (a, cond, b)).ljust(100),
  var = {"a":a, "b":b}
  if eval("a %s b" % cond, var):
    print "\x1b[33m[  OK  ]\x1b[0m"
    ac_count += 1
  else:
    print "\x1b[31m[ Fail ]\x1b[0m"
    wa_count += 1

def assert_neq(a, b):
  _assert(a, b, "!=")

def assert_eq(a, b):
  _assert(a, b, "==")

if __name__ == "__main__":
  F = FiniteField(101)
  x = F(2)
  y = F(203)
  assert_neq(x, 1)
  assert_eq(x, 2)
  assert_eq(x, 2)
  F = FiniteField(5)
  x = F(3)
  y = F(7) # = 2
  assert_eq(x+y, F(0))
  assert_eq(x+y, 0)
  assert_eq(x-y, 1)
  assert_eq(x*y, 1)
  x = F(2)
  y = F(3)
  # x is element of F, y is invert element of x
  assert_eq(x*y, 1)
  # commutive!
  assert_eq(1/x, y)
  assert_eq(1/y, x)

  assert_eq(x**3, y)

  assert_eq(util.crt([3, 4], [4, 9]), 31)
  #assert_eq(util.crt([7, 13], [12, 18]), 31)

  assert_eq(F.order(), 4)
  assert_eq(x.order(), 4)

  F = FiniteField(17)
  E = EllipticCurve(F, 1, 0)

  P = E(1, 6)
  Q = E(11, 4)
  assert_eq(P+Q, E(3, 8))
  assert_eq(P+P, E(0, 0))
  assert_eq(P*2, E(0, 0))
  assert_eq(2*P, E(0, 0))
  assert_eq(P.order(), 4)

  print "Random Test: modinv"
  i = 0
  while i < 10:
    r = randint(-50, 50)
    if r == 0:
      continue
    print "[+] Random: %d" % r
    assert_eq((util.modinv(r, 101) * r) % 101, 1)
    i += 1



  print "[+] %d Test(s) finished. %d Test(s) success, %d Test(s) fail." % (ac_count + wa_count, ac_count, wa_count)
