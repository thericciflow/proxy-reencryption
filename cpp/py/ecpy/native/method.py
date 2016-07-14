from library import *
from ZZ_Native import *

def modinv(a, m):
  res = cast(lib.ZZ_modinv(LPZZ(ZZ_create(a)), LPZZ(ZZ_create(m))), LPZZ).contents
  return int(str(res))

