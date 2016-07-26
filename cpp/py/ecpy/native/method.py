from library import *
from ZZ_Native import *

def modinv(a, m):
  res = cast(lib.ZZ_modinv(LPZZ(ZZ_create(a)), LPZZ(ZZ_create(m))), LPZZ).contents
  return int(str(res))

def jacobi(a, n):
  res = lib.ZZ_jacobi(LPZZ(ZZ_create(a)), LPZZ(ZZ_create(n)))
  return int(res)

def legendre(a, p):
  res = lib.ZZ_legendre(LPZZ(ZZ_create(a)), LPZZ(ZZ_create(p)))
  return int(res)

def miller(E, P, Q, m):
