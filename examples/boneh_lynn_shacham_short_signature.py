from applib import ExtendedFiniteField, EllipticCurve, hash_to_point
from applib import modified_pairing
# some secret here
secret = 0xdeadbeef

p = int("519130854181216822940010183929228824799903608965854344652247095061840"
        "171046331806216450362399027718738907160814923275136184823731671523533"
        "69121135029626237")
l = (p + 1) / 6
F = ExtendedFiniteField(p, "x^2+x+1")
E = EllipticCurve(F, 0, 1)

P = E(6, (int("132232566129358054566854866197859888282113525457609791458267738"
              "093383172947853439119386209153000327873055642240731797496321750"
              "04531990130007962275678599731"),
          int("264465132258716109133709732395719776564227050915219582916535476"
              "186766345895706878238772418306000655746111284481463594992643500"
              "09063980260015924551357199462")))

sP = secret * P


def sign(m):
  global secret, E, l
  # Q = MapToPoint(m)
  # return secret * Q
  return secret * hash_to_point(E, m, l)


def verify(sig, msg):
  global secret, E, l, P, sP
  # a = e_l(P, secret * Q) = e_l(P, Q) ^ secret
  a = modified_pairing(E, P, sig, l)
  # Q = MapToPoint(m')
  # b = e_l(secret * P, Q) = e_l(P, Q) ^ secret
  b = modified_pairing(E, sP, hash_to_point(E, msg, l), l)
  # a = b?
  return a == b


def main():
  global secret, E, F, l, P, sP

  while True:
    print "What to do?"
    print "Sign -> s, Verify -> v, Quit -> q :",
    t = raw_input().strip().lower()
    if t == "s":
      print "[+] Message? :",
      m = raw_input().strip()
      sig = sign(m)
      print ":".join(map(lambda x: str(x[0]) + ";" + str(x[1]), map(tuple,
                     tuple(sig)[:-1])))
    elif t == "v":
      print "Signature? :",
      x, y = map(lambda x: tuple(map(int, x.split(";"))),
                 raw_input().strip().split(":"))
      sig = E(x, y)
      print "Message? :",
      m = raw_input().strip()
      if verify(sig, m):
        print "[+] Verify: Passed."
      else:
        print "[+] Verify: Invalid Signature!"
    elif t == "q":
      print "[+] Quit"
      break


if __name__ == "__main__":
  main()
