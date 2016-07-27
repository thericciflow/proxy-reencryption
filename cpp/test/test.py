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
    r = repr(b)[:64]
    if repr(b) != r:
      r +=  "..."
    print "\x1b[33m[  OK  ]\x1b[0m %s" % (r, )
    ac_count += 1
  else:
    print "\x1b[31m[ Fail ]\x1b[0m Expected: %r, Result: %r" % (b, a)
    wa_count += 1

def assert_neq(a, b, m):
  _assert(a, b, m, "!=")

def assert_eq(a, b, m):
  _assert(a, b, m, "==")

def main():
  ff = FF(31)
  FE = lambda x: FF_elem(ff, x)
  x = FF_elem(ff, 25)
  y = FF_elem(ff, 41)
  assert_eq(int(x+y), 4, "x+y")
  assert_eq(int(x-y), 15, "x+y")
  assert_eq(int(x*y), 2, "x+y")
  assert_eq(int(x/y), 18, "x/y")
  assert_eq(int(x**y), 5, "x^y")
  assert_eq(int(x**FE(10)), 25, "x^(y % p)")
  print "[+] %d Test(s) finished. %d Test(s) success, %d Test(s) fail." % (
        ac_count + wa_count, ac_count, wa_count)
  sys.exit(wa_count)

if __name__ == "__main__":
  main()
