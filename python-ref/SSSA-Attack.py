from ecpy import *

def hensel_lift(curve, P):
  x, y = P.x,P.y
  t = (((x**3 + curve.a * x + curve.b) - y**2)/curve.field.p) % curve.field.p
  t *= util.modinv(2*y, curve.field.p) 
  t %= curve.field.p
  return (x, y + (curve.field.p * t))

def SSSA_Attack(F, E, P, Q):
  A = E.a
  B = E.b
  # lP, lQ, ... is "lifted" P, Q, ...

  x1, y1 = hensel_lift(E, P)
  x2, y2 = hensel_lift(E, Q)

  lF = Zmod(F.p ** 2)

  lA = ((y2**2 - y1**2 - (x2**3 - x1**3)) * util.modinv(x2-x1, lF.n)) % lF.n

  lB = (y1**2 - x1**3 - A*x1) % lF.n

  lE = EllipticCurve(lF, lA, lB)

  lP = lE(x1, y1)
  lQ = lE(x2, y2)

  print "[+] Lifted Curve : %s" % lE
  print "[+] Lifted P : %r" % lP
  print "[+] Lifted Q : %r" % lQ

  print "[+] Calculate U..."
  lU = (F.p - 1) * lP
  print "[+] Calculate V..."
  lV = (F.p - 1) * lQ

  dx1 = ((lU.x - lP.x) / F.p) % F.p
  dx2 = QQ((lU.y - lP.y), F.p)
  dy1 = (lV.x - lQ.x)
  dy2 = (lV.y - lQ.y) % F.p

  m = dy1 * util.modinv(dx1, F.p) * dx2 * util.modinv(dy2, F.p)
  m %= F.p

  return m.int()

# Sharif University CTF 2016: Crypto 350 British Elevetor

F = FiniteField(16857450949524777441941817393974784044780411511252189319)

A = 16857450949524777441941817393974784044780411507861094535
B = 77986137112576

E = EllipticCurve(F, A, B)

P = E(5732560139258194764535999929325388041568732716579308775, 14532336890195013837874850588152996214121327870156054248)
Q = E(2609506039090139098835068603396546214836589143940493046, 8637771092812212464887027788957801177574860926032421582)

"""

F = FiniteField(853)

A = 108
B = 4

E = EllipticCurve(F, A, B)

P = E(0, 2)
Q = E(563, 755)
"""

print SSSA_Attack(F, E, P, Q)
