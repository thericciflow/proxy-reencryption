import fractions

def gcd(x, y):
  while y != 0:
    t = x % y
    x, y = y, t
  return x

def egcd(x, y):
  if x == 0:
    return (y, 0, 1)
  a, b = x, y
  g, y, x = egcd(y%x, x)
  return (g, x-(b/a)*y, y)

def modinv(a, m):
  if a % m == 0:
    return None
  return egcd(a, m)[1] % m
