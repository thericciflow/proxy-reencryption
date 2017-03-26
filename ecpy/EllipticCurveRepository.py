from collections import namedtuple as _namedtuple

_ECData = _namedtuple('ECData', ['p', 'a', 'b', 'n', 'Gx', 'Gy'])
_repo = {
  'secp192k1': _ECData(
    p=2**192 - 2**32 - 2**12 - 2**8 - 2**7 - 2**6 - 2**3 - 1, 
    a=0, b=3,
    Gx=0xDB4FF10EC057E9AE26B07D0280B7F4341DA5D1B1EAE06C7D, 
    Gy=0x9B2F2F6D9C5628A7844163D015BE86344082AA88D95E2F9D,
    n=0xFFFFFFFFFFFFFFFFFFFFFFFE26F2FC170F69466A74DEFD8D
  )
}

def EllipticCurveRepository(name):
  '''
  Elliptic Curve Repository - standard curves repository function
  Args:
    name: name of the curve (e.g. 'secp256k1') case insensitive
  Return: Tuple (F, E, G, n)
    F: Base field object of E
    E: Elliptic curve corresponding to `name`
    G: Base point given by `name`
    n: Order of `G`
  '''
  from ecpy.structure import FiniteField, EllipticCurve
  name = name.lower()
  if name in _repo.keys():
    data = _repo[name]
    F = FiniteField(data.p)
    E = EllipticCurve(F, data.a, data.b)
    G = E(data.Gx, data.Gy)
    n = data.n
    return (F, E, G, n)
  else:
    raise ValueError('Invalid Elliptic Curve Name: %r' % name)

