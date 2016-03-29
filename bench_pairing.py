from ecpy import *
from timeit import timeit
from random import randint


def show_results(name, result, count):
    per_pass = 1000000 * (result / count)
    print '%s: %.2f usec/pass' % (name, per_pass)


def main():
  l = 2201426263
  p = 6 * l - 1
  F = ExtendedFiniteField(p, "x^2+x+1")
  E = EllipticCurve(F, 0, 1)
  i = 3
  while True:
    y = E.get_corresponding_y(i)
    if y != None:
      P = E(i, y)
      if (l * P).is_infinity():
        break
    i += 1
  print P

  rand = [randint(2**31, 2**32) for _ in xrange(10)]
  count = 20

  weil_time = []
  print "[+] Weil Pairing: "
  for x in rand:
    weil_expr = "weil_pairing(%r, %r, %r.distortion_map(), %r)" % (E, P, x * P, l)
    r = timeit(weil_expr, setup="from ecpy import EllipticCurve, ExtendedFiniteField, weil_pairing", number=count)
    weil_time += [r]
    show_results("weil", r, count)

  print "[+] Tate Pairing: "
  tate_time = []
  for x in rand:
    tate_expr = "tate_pairing(%r, %r, %r.distortion_map(), %r, 2)" % (E, P, x*P, l)
    r = timeit(tate_expr, setup="from ecpy import EllipticCurve, ExtendedFiniteField, tate_pairing", number=count)
    tate_time += [r]
    show_results("tate", r, count)

  print "=" * 64
  show_results("weil", sum(weil_time), count * 10)
  show_results("tate", sum(tate_time), count * 10)
if __name__ == "__main__":
  main()
