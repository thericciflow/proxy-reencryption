
def gcd(x, y):
  while y != 0:
    t = x % y
    x, y = y, t

def egcd(x, y):
  if x == 0:
    return (y, 0, 1)
  g, x, y = egcd(y%x, x)
  x, y = y, x
  return (g, x-(b/a)*y, y)

