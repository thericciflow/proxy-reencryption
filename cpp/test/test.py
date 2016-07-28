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
  F = FF(31)
  x = FF_elem(25)
  y = FF_elem(10)
  t = FF_elem(0)
  F.add(t, x, y)
  assert_eq(int(t), 4, "x+y")
  F.sub(t, x, y)
  assert_eq(int(t), 15, "-+y")
  F.mul(t, x, y)
  assert_eq(int(t), 2, "x*y")
  F.div(t, x, y)
  assert_eq(int(t), 18, "x/y")
  F.pow(t, x, y)
  assert_eq(int(t), 25, "x^y")
  print "[+] %d Test(s) finished. %d Test(s) success, %d Test(s) fail." % (
        ac_count + wa_count, ac_count, wa_count)
  sys.exit(wa_count)

if __name__ == "__main__":
  main()
