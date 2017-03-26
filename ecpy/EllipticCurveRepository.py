from collections import namedtuple

_ECData = namedtuple('ECData', ['p', 'a', 'b', 'n', 'Gx', 'Gy'])
_repo = {}

def EllipticCurveRepository(name):
  '''
  Elliptic Curve Repository - standard curves repository function
  Args:
    name: name of the curve (e.g. 'secp256k1') case insensitive
  Return: Tuple (F, E, G)
    F: Base field object of E
    E: Elliptic curve corresponding to `name`
    G: Base point given by `name`
  '''
  from ecpy.structure import FiniteField, EllipticCurve
  if name in _repo.keys():
    data = _repo[name]
    F = FiniteField(data.p)
    E = EllipticCurve(F, data.a, data.b)
    G = E(data.Gx, data.Gy)
    return (F, E, G)
  else:
    raise ValueError('Invalid Elliptic Curve Name: %r' % name)

