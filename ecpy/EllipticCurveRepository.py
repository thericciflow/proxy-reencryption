from collections import namedtuple as _namedtuple

_ECData = _namedtuple('ECData', ['p', 'a', 'b', 'n', 'Gx', 'Gy'])
_repo = {
  'secp192k1': _ECData(
    p=2**192 - 2**32 - 2**12 - 2**8 - 2**7 - 2**6 - 2**3 - 1, 
    a=0, b=3,
    Gx=0xDB4FF10EC057E9AE26B07D0280B7F4341DA5D1B1EAE06C7D, 
    Gy=0x9B2F2F6D9C5628A7844163D015BE86344082AA88D95E2F9D,
    n=0xFFFFFFFFFFFFFFFFFFFFFFFE26F2FC170F69466A74DEFD8D
  ),
  'secp192r1': _ECData(
    p=2**192 - 2**64 - 1,
    a=0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFC,
    b=0x64210519E59C80E70FA7E9AB72243049FEB8DEECC146B9B1,
    Gx=0x188DA80EB03090F67CBF20EB43A18800F4FF0AFD82FF1012,
    Gy=0x07192B95FFC8DA78631011ED6B24CDD573F977A11E794811,
    n=0xFFFFFFFFFFFFFFFFFFFFFFFF99DEF836146BC9B1B4D22831
  ),
  'secp224k1': _ECData(
    p=2**224 - 2**32 - 2**12 - 2**11 - 2**9 - 2**7 - 2**4 - 2 - 1,
    a=0, b=5,
    Gx=0xA1455B334DF099DF30FC28A169A467E9E47075A90F7E650EB6B7A45C,
    Gy=0x7E089FED7FBA344282CAFBD6F7E319F7C0B0BD59E2CA4BDB556D61A5,
    n=0x010000000000000000000000000001DCE8D2EC6184CAF0A971769FB1F7
  ),
  'secp224r1': _ECData(
    p=2**224 - 2**96 + 1,
    a=0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFE,
    b=0xB4050A850C04B3ABF54132565044B0B7D7BFD8BA270B39432355FFB4,
    Gx=0xB70E0CBD6BB4BF7F321390B94A03C1D356C21122343280D6115C1D21,
    Gy=0xBD376388B5F723FB4C22DFE6CD4375A05A07476444D5819985007E34,
    n=0xFFFFFFFFFFFFFFFFFFFFFFFFFFFF16A2E0B8F03E13DD29455C5C2A3D
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

