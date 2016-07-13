from ..util import modinv

def hensel_lift(curve, P):
  """
  Calculate Lifted Point using Hensel's Lemma
  Args:
    curve: The Elliptic Curve
    P: A point on curve
  Returns:
    The "lifted" Point
  """
  x, y, _ = map(int, tuple(P))
  p = curve.field.p
  t = (((x * x * x + curve.a * x + curve.b) - y * y) / p) % p
  t = (t * modinv(2 * y, p)) % p
  return (x, y + (curve.field.p * t))


def SSSA_Attack(F, E, P, Q):
  """
  Solve ECDLP using SSSA(Semaev-Smart-Satoh-Araki) Attack.
  Args:
    F: The Base Field
    E: The Elliptic Curve
    P: A point on E
    Q: A point on E
  Returns:
    Return x where satisfies Q = xP.
  """
  from ..structure.EllipticCurve import EllipticCurve
  from ..structure.RationalField import QQ
  from ..structure.Zmod import Zmod
  A = E.a
  # lP, lQ, ... is "lifted" P, Q, ...
  x1, y1 = hensel_lift(E, P)
  x2, y2 = hensel_lift(E, Q)
  lF = Zmod(F.p ** 2)
  lA = (y2 * y2 - y1 * y1 - (x2 * x2 * x2 - x1 * x1 * x1))
  lA = (lA * modinv(x2 - x1, lF.n)) % lF.n
  lB = (y1 * y1 - x1 * x1 * x1 - A * x1) % lF.n
  lE = EllipticCurve(lF, lA, lB)
  lP = lE(x1, y1)
  lQ = lE(x2, y2)
  lU = (F.p - 1) * lP
  lV = (F.p - 1) * lQ
  dx1 = ((int(lU.x) - int(lP.x)) / F.p) % F.p
  dx2 = int(lU.y) - int(lP.y)
  dy1 = int(lV.x) - int(lQ.x)
  dy2 = (int(lV.y) - int(lQ.y)) % F.p
  m = dy1 * modinv(dx1, F.p) * dx2 * modinv(dy2, F.p)
  return int(QQ(m, F.p) % F.p)
