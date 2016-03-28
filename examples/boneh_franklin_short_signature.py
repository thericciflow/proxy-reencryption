from applib import *

secret = 0xdeadbeef

p = 51913085418121682294001018392922882479990360896585434465224709506184017104633180621645036239902771873890716081492327513618482373167152353369121135029626237
l = (p + 1) / 6
F = ExtendedFiniteField(p, "x^2+x+1")
E = EllipticCurve(F, 0, 1)

P = E(6, (13223256612935805456685486619785988828211352545760979145826773809338317294785343911938620915300032787305564224073179749632175004531990130007962275678599731, 26446513225871610913370973239571977656422705091521958291653547618676634589570687823877241830600065574611128448146359499264350009063980260015924551357199462))

sP = secret * P


def sign(m):
  global secret, E, l
  return secret * hash_to_point(E, m, l)


def verify(sig, msg):
  global secret, E, l, P, sP
  a = modified_pairing(E, P, sig, l)
  b = modified_pairing(E, sP, hash_to_point(E, msg, l), l)
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
