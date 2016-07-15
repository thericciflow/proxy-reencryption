from ecpy.native import *
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
  assert_eq(x+y, ZZ(10), "x+y")
  assert_eq(x-y, ZZ(-6), "x-y")
  assert_eq(x*y, ZZ(16), "x*y")
  assert_eq(y/x, ZZ(4), "y/x")
  assert_eq(modinv(7, 65537), 18725, "modinv(7, 65537)")

  FF = FF_create
  x = FF(25, 31)
  print "[+] %d Test(s) finished. %d Test(s) success, %d Test(s) fail." % (
        ac_count + wa_count, ac_count, wa_count)
  sys.exit(wa_count)

if __name__ == "__main__":
  main()
