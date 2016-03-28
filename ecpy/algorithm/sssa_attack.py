from ..util import modinv


def hensel_lift(curve, P):
  x, y, _ = map(int, tuple(P))
  t = (((x * x * x + curve.a * x + curve.b) - y * y) / curve.field.p)
  t %= curve.field.p
  t *= modinv(2 * y, curve.field.p)
  t %= curve.field.p
  return (x, y + (curve.field.p * t))


def SSSA_Attack(F, E, P, Q):
  from ..structure.EllipticCurve import EllipticCurve
  from ..structure.RationalField import QQ
  from ..structure.Zmod import Zmod
  A = E.a
  # lP, lQ, ... is "lifted" P, Q, ...
  x1, y1 = hensel_lift(E, P)
  x2, y2 = hensel_lift(E, Q)
  lF = Zmod(F.p ** 2)
  lA = (y2 * y2 - y1 * y1 - (x2 * x2 * x2 - x1 * x1 * x1))
  lA *= modinv(x2 - x1, lF.n)
  lA %= lF.n
  lB = (y1 * y1 - x1 * x1 * x1 - A * x1) % lF.n
  lE = EllipticCurve(lF, lA, lB)
  lP = lE(x1, y1)
  lQ = lE(x2, y2)
  lU = (F.p - 1) * lP
  lV = (F.p - 1) * lQ
  dx1 = ((int(lU.x) - int(lP.x)) / F.p) % F.p
  dx2 = QQ((int(lU.y) - int(lP.y)), F.p)
  dy1 = (int(lV.x) - int(lQ.x))
  dy2 = (int(lV.y) - int(lQ.y)) % F.p
  m = dy1 * modinv(dx1, F.p) * dx2 * modinv(dy2, F.p)
  m %= F.p
  return int(m)
