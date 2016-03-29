from applib import EllipticCurve, ExtendedFiniteField, modified_pairing
from applib import hash_to_field, get_point, H2
import random

secret = 0xdeadbeef

p = int("501794446334189957604282155189438160845433783392772743395579628617109"
        "929160215221425142482928909270259580854362463493326988807453595748573"
        "76419559953437557")

l = (p + 1) / 6

F = ExtendedFiniteField(p, "x^2+x+1")
E = EllipticCurve(F, 0, 1)

P = E(3, int("1418077311270457886139292292020587683642898636677353664354101171"
             "7684401801069777797699258667061922178009879315047772033936311133"
             "535564812495329881887557081"))
sP = E(int("129862491850266001914601437161941818413833907050695770313188660767"
           "152646233571458109764766382285470424230719843324368007925375351295"
           "39576510740045312772012"),
       int("452543250979361708074026409576755302296698208397782707067096515523"
           "033579018123253402743775747767548650767928190884624134827869137911"
           "24188897792458334596297"))


def get_user_public(E, P, id, l):
  return P * hash_to_field(E.field, int(id.encode("hex"), 16))


def get_user_secret(E, pubkey, l):
  global secret
  return pubkey * secret


def encrypt(E, P, sP, pubkey, m, l):
  assert isinstance(m, (int, long))
  r = random.randint(2**30, 2**31)
  return r * P, m ^ H2(modified_pairing(E, pubkey, sP, l) ** r)


def master_point(E, l):
  global secret
  P = get_point(E, l)
  return P, secret * P


def decrypt(E, K, c, l):
  return c[1] ^ H2(modified_pairing(E, c[0], K, l))


def main():
  global P, sP, l
  print "Please tell me your ID :",
  ID = raw_input().strip()
  Q = get_user_public(E, P, ID, l)
  sQ = get_user_secret(E, Q, l)
  while True:
    print "What to do?"
    print "Encryption -> e, Decryption -> d, Quit -> q :",
    t = raw_input().strip().lower()
    if t == "e":
      print "[+] Message? :",
      m = int(raw_input().strip().encode("hex"), 16)
      C = encrypt(E, P, sP, Q, m, l)
      print "[+] Your Encrypted Message: %s" % (
          ":".join(map(lambda x: str(x[0]) + ";" + str(x[1]),
                       map(tuple, tuple(C[0])[:-1]))) + ":" + str(C[1]))
    elif t == "d":
      print "Ciphertext? :",
      d = raw_input().strip().split(":")
      x, y = map(lambda x: tuple(map(int, x.split(";"))), d[:-1])
      c = int(d[-1])
      C1 = E(x, y)
      C2 = c
      C = (C1, C2)
      m = decrypt(E, sQ, C, l)
      m = hex(m)[2:-1]
      if len(m) % 2 == 1:
        m = "0" + m
      m = m.decode("hex")
      print "[+] Your Message :", m
    elif t == "q":
      print "[+] Quit"
      break


if __name__ == "__main__":
  main()
