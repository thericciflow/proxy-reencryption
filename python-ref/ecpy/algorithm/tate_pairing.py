from ..structure.EllipticCurve import EllipticCurve
from ..structure.FiniteField import FiniteField
from .Miller import miller

def tate_pairing(E, P, Q, m):
  k = E.embedding_degree(m)
  f = E.field(miller(E, P, Q, m))
  return f ** (((E.field.p ** k) - 1) / m)

