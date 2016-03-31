from ecpy import EllipticCurve, ExtendedFiniteField, symmetric_tate_pairing
import hashlib
import random
import cPickle

# PKI secret
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


def H(x):
  return x.x * x.field.p + x.y


def get_user_public(E, P, id, l):
  v = int(hashlib.sha512(id).hexdigest().encode("hex"), 16)
  return P * v


def get_user_secret(E, pubkey, l):
  global secret
  return pubkey * secret


def encrypt(E, P, sP, pubkey, m, l):
  assert isinstance(m, (int, long))
  # r = rand()
  r = random.randint(2**30, 2**31)
  # r*P, m xor e_l(secret * P, Q)^r = e_l(P, Q) ^ (secret * r)
  return (r * P,
          m ^ H(E.field(symmetric_tate_pairing(E, sP, pubkey, l) ** r)))


def decrypt(E, K, c, l):
  # c1, c2 = r*P, m xor e_l(secret * P, Q) ^ r = e_l(P, Q) ^ (secret * r)
  # a = e_l(c1, K) = e_l(r*P, secret * Q) = e_l(P, Q) ^ (secret * r)
  return c[1] ^ H(E.field(symmetric_tate_pairing(E, c[0], K, l)))


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
      t = tuple(C[0])
      c = cPickle.dumps((t[0], t[1], C[1])).encode("zlib").encode("base64")
      c = c.replace("\n", "")
      print "[+] Your Encrypted Message: %s" % c
    elif t == "d":
      print "Ciphertext? :",
      d = raw_input().strip().decode("base64").decode("zlib")
      x, y, c = cPickle.loads(d)
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
