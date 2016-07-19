from ecpy.native import *
from ecpy import *
import time
import sys

ac_count = 0
wa_count = 0

def _assert(a, b, msg, cond):
  global ac_count, wa_count
  msg = msg.ljust(16)
  print ("[+] %s..." % (msg)).ljust(30),
  var = {"a": a, "b": b}
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

def main():
  ZZ = ZZ_create
  x = ZZ(2)
  y = ZZ(8)
  a = ZZ(12345)
  b = ZZ(331)
  c = ZZ(1001)
  d = ZZ(9907)
  assert_eq(x+y, ZZ(10), "x+y")
  assert_eq(x-y, ZZ(-6), "x-y")
  assert_eq(x*y, ZZ(16), "x*y")
  assert_eq(y/x, ZZ(4), "y/x")
  assert_eq(modinv(7, 65537), 18725, "modinv(7, 65537)")
  assert_eq(legendre(a, b), -1, "(12345|331) = -1")
  assert_eq(jacobi(c, d), -1, "(1001|9907) = -1")

  FF = lambda x: FF_create(x, p)
  p = 31
  x = FF(25)
  y = FF(41)
  assert_eq(y, FF(10), "y modulo check")
  assert_eq(x+y, FF(4), "x+y")
  assert_eq(x-y, FF(15), "x-y")
  assert_eq(x*y, FF(2), "x*y")
  assert_eq(x/y, FF(18), "x/y")

  modulo = 7
  EF = lambda x, y: EF_create(x, y, modulo, "x^2+1")
  x = EF(3, 0)
  y = EF(0, 5)
  assert_eq(x+y, EF(3, 5), "x+y")
  assert_eq(x*y, EF(0, 1), "x*y")

  modulo = 31
  EF = lambda x, y: EF_create(x, y, modulo, "x^2+x+1")
  x = EF(61, 0)
  y = EF(0, 20)
  assert_eq(x, EF(30, 0), "x modulo check")
  assert_eq(x+y, EF(30, 20), "x+y")
  assert_eq(x*y, EF(0, 11), "x*y")
  print "[+] %d Test(s) finished. %d Test(s) success, %d Test(s) fail." % (
        ac_count + wa_count, ac_count, wa_count)
  sys.exit(wa_count)

if __name__ == "__main__":
  main()
