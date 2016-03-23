from ..structure.EllipticCurve import EllipticCurve
from ..structure.RationalField import QQ
from ..structure.RealField import RR
from ..structure.Zmod import Zmod
from ..util import modinv

def hensel_lift(curve, P):
  x, y, _ = map(int, tuple(P))
  t = (((x**3 + curve.a * x + curve.b) - y**2)/curve.field.p) % curve.field.p
  t *= modinv(2*y, curve.field.p) 
  t %= curve.field.p
  return (x, y + (curve.field.p * t))

def SSSA_Attack(F, E, P, Q):
  A = E.a
  B = E.b
  # lP, lQ, ... is "lifted" P, Q, ...

  x1, y1 = hensel_lift(E, P)
  x2, y2 = hensel_lift(E, Q)

  lF = Zmod(F.p ** 2)

  lA = ((y2**2 - y1**2 - (x2**3 - x1**3)) * modinv(x2-x1, lF.n)) % lF.n

  lB = (y1**2 - x1**3 - A*x1) % lF.n

  lE = EllipticCurve(lF, lA, lB)

  lP = lE(x1, y1)
  lQ = lE(x2, y2)

  #print "[+] Lifted Curve : %s" % lE
  #print "[+] Lifted P : %r" % lP
  #print "[+] Lifted Q : %r" % lQ

  #print "[+] Calculate U..."
  lU = (F.p - 1) * lP
  #print "[+] Calculate V..."
  lV = (F.p - 1) * lQ

  dx1 = ((int(lU.x) - int(lP.x)) / F.p) % F.p
  dx2 = QQ((int(lU.y) - int(lP.y)), F.p)
  dy1 = (int(lV.x) - int(lQ.x))
  dy2 = (int(lV.y) - int(lQ.y)) % F.p

  m = dy1 * modinv(dx1, F.p) * dx2 * modinv(dy2, F.p)
  m %= F.p

  return m.int()
