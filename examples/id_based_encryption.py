from applib import *
import random

secret = 0xdeadbeef

p = 50179444633418995760428215518943816084543378339277274339557962861710992916021522142514248292890927025958085436246349332698880745359574857376419559953437557

l = (p+1)/6

F = ExtendedFiniteField(p, "x^2+x+1")
E = EllipticCurve(F, 0, 1)

P = E(3, 14180773112704578861392922920205876836428986366773536643541011717684401801069777797699258667061922178009879315047772033936311133535564812495329881887557081)
sP = E(12986249185026600191460143716194181841383390705069577031318866076715264623357145810976476638228547042423071984332436800792537535129539576510740045312772012, 45254325097936170807402640957675530229669820839778270706709651552303357901812325340274377574776754865076792819088462413482786913791124188897792458334596297)

def get_user_public(E, P, id, l):
  return P * hash_to_field(E.field, int(id.encode("hex"), 16))

def get_user_secret(E, pubkey, l):
  global secret
  return pubkey * secret

def encrypt(E, P, sP, pubkey, m, l):
  assert isinstance(m, (int, long))
  r = random.randint(2**30, 2**31)
  return r*P, m ^ H2(modified_pairing(E, pubkey, sP, l) ** r)

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
      print "[+] Your Encrypted Message: %s" % (":".join(map(lambda x: str(x[0]) + ";" + str(x[1]), map(tuple, tuple(C[0])[:-1]))) + ":" + str(C[1]))
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
