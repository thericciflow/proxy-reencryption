from library import *
from ZZ_Native import *
from EC_Native import *
from EP_Native import *
from EF_Native import *
from FF_Native import *

def modinv(a, m):
  res = cast(lib.ZZ_modinv(LPZZ(ZZ_create(a)), LPZZ(ZZ_create(m))), LPZZ).contents
  return int(str(res))

def jacobi(a, n):
  res = lib.ZZ_jacobi(LPZZ(ZZ_create(a)), LPZZ(ZZ_create(n)))
  return int(res)

def legendre(a, p):
  res = lib.ZZ_legendre(LPZZ(ZZ_create(a)), LPZZ(ZZ_create(p)))
  return int(res)

def miller(P, Q, m):
  f = lib.EP_miller(LPEP(P), LPEP(Q), LPZZ(ZZ_create(m)))
  if P.type == 0: # LPFF
    return cast(f, LPFF).contents
  if P.type == 1: # LPEF
    return cast(f, LPEF).contents
  return f
