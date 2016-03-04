import fractions

class ModinvNotFoundError(ArithmeticError):
  pass

def gcd(x, y):
  while y != 0:
    t = x % y
    x, y = y, t
  return x

def gcd_multiple(*a):
  g = a[0]
  for x in a[1:]:
    g = gcd(g, x)
  return g

def egcd(x, y):
  if x == 0:
    return (y, 0, 1)
  a, b = x, y
  g, y, x = egcd(y%x, x)
  return (g, x-(b/a)*y, y)

def modinv(a, m):
  if a % m == 0:
    raise ModinvNotFoundError()
  a %= m
  return egcd(a, m)[1] % m

def lcm(*a):
  return reduce(lambda x,y: x*y, a, 1)/gcd_multiple(*a)

def crt(a, n):
  N = reduce(lambda x,y: x*y, n, 1)
  if gcd_multiple(*n) > 1:
    # TODO: implement in gcd(modulo > 0
    print "Warning: gcd(modulo) > 1"
    return None
  nk = map(lambda x: N/x, n)
  ik = map(lambda x: modinv(x[0], x[1]), zip(nk, n))
  return sum(map(lambda x: (x[0]*x[1]*x[2]) % N, zip(a, nk, ik))) % lcm(*n)
