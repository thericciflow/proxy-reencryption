from .Miller import miller
from ..structure.EllipticCurve import EllipticCurve

def weil_pairing(E, P, Q, m, S = None):
  if S == None:
    S = E.random_point()
  fpqs = miller(E, P, Q+S, m)
  fps  = miller(E, P, S, m)
  fqps = miller(E, Q, P-S, m)
  fqs  = miller(E, Q, -S, m)
  return int((E.field._inv(fps) * fpqs) * E.field._inv(int(E.field._inv(fqs)) * fqps))

