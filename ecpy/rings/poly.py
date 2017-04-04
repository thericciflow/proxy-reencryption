from copy import deepcopy
from ecpy.fields import QQ
from ecpy.utils import memoize
import itertools

class UnivariatePolynomial(object):
  '''
  Univariate Polynomial Ring
  '''
  def __init__(s, field):
    '''
    Args:
      field : A base field class
    '''
    super(UnivariatePolynomial, s).__init__()
    s.field = field
    s.element_class = PolynomialElem

  def gen(s):
    return PolynomialElem(s, [1, 0])

  def __str__(s):
    return 'Univariate Polynomial Ring over %s' % s.field


class PolynomialElem(object):
  def __init__(s, *args):
    """
    coeffs = [x^0, x^1, x^2, ... degree's coefficient]
    """
    super(Polynomial, s).__init__()
    if len(args) == 1:
      v = args[0]
      if isinstance(v, Polynomial):
        s.coeffs = v.coeffs
      elif hasattr(v, "__iter__"):
        s.coeffs = list(v)
      else:
        s.coeffs = [v + 0]
    else:
      s.coeffs = args
    s.trim()

  def __add__(s, rhs):
    if isinstance(rhs, Polynomial):
      A = deepcopy(s.coeffs)
      B = rhs.coeffs
      ret = []
      for x, y in itertools.izip_longest(A, B, fillvalue=0):
        ret += [(x + y)]
      return Polynomial(ret)
    else:
      return Polynomial([s.coeffs[0] + rhs] + s.coeffs[1:])

  def __divmod__(s, rhs):
    assert rhs != 0
    if isinstance(rhs, Polynomial):
      q = 0
      r = Polynomial(s)
      d = rhs.degree()
      c = rhs[-1]
      while r.degree() >= d:
        t = Polynomial([r[-1] / c]).shift(r.degree() - d)
        q = q + t
        r = r - t * rhs
      return q, r
    else:
      q = Polynomial(map(lambda x: x / rhs, s.coeffs))
      r = Polynomial(map(lambda x: x % rhs, s.coeffs))
      return q, r

  def __div__(s, rhs):
    return divmod(s, rhs)[0]

  def __eq__(s, rhs):
    if isinstance(rhs, Polynomial):
      return all([x == y for x, y in zip(s, rhs)])
    else:
      return len(s.coeffs) == 1 and s[0] == rhs

  def __getitem__(s, idx):
    return s.coeffs[idx]

  def __iter__(s):
    return iter(s.coeffs)

  def __len__(s):
    return len(s.coeffs)

  def __mul__(s, rhs):
    if isinstance(rhs, Polynomial):
      if rhs.apply(1) * s.apply(1) == 0:
        return Polynomial(0)
      if rhs.degree() == 0:
        return s * rhs[0]
      elif s.degree() == 0:
        return rhs * s[0]
      if s.apply(0) == 0 and s.apply(1) == 1:
        return rhs.shift(s.degree())
      elif rhs.apply(0) == 0 and rhs.apply(1) == 1:
        return s.shift(rhs.degree())
      else:
        ret = [0]*(len(s)+len(rhs) - 1)
        for x, y in enumerate(s.coeffs):
          for u, v in enumerate(rhs.coeffs):
            ret[x + u] += y * v
        return Polynomial(ret)
    else:
      return Polynomial(map(lambda x: rhs * x, s.coeffs))

  def __ne__(s, rhs):
    return not (s == rhs)

  def __neg__(s):
    return Polynomial(map(lambda x: -x, s.coeffs))

  def __mod__(s, rhs):
    return divmod(s, rhs)[1]

  def __pow__(s, rhs, mod=0):
    if rhs == 0:
      return Polynomial(1)
    d = int(rhs)
    if d < 0:
      x = 1 / s
      d = -d
    else:
      x = s
    bits = map(int, bin(d)[2:])[::-1]
    if bits[0]:
      res = x
    else:
      res = Polynomial(1)
    b = 0
    for cur in bits[1:]:
      print "[+] bit = %d" % b
      b += 1
      x *= x
      if mod > 0:
        x %= mod
        x.trim()
      if cur:
        res *= x
        if mod > 0:
          res %= mod
          res.trim()
    return res

  def __radd__(s, lhs):
    if isinstance(lhs, Polynomial):
      return s + lhs
    else:
      return Polynomial([s.coeffs[0] + lhs] + s.coeffs[1:])

  def __repr__(s):
    return "Polynomial(%s)" % s.coeffs

  def __rmul__(s, lhs):
    return s * lhs

  def __rsub__(s, lhs):
    return lhs + (-1*s)

  def __str__(s):
    if len(s.coeffs) == 0:
      return "0"
    res = []
    deg = 0
    for x in s:
      r = ""
      if x != 0:
        if (x != -1 and x != 1) or deg == 0:
          r += str(x)
        elif x == -1:
          r += "-"
        if deg > 0:
          r += "x"
          if deg > 1:
            r += "^%d" % deg
      if r != "":
        res += [r]
      deg += 1
    return "+".join(res[::-1]).replace("+-", "-")

  def __sub__(s, rhs):
    return s + (-1*rhs)

  @memoize
  def apply(s, x0):
    """
    Using Horner's method
    """
    ret = 0
    for x in s.coeffs[::-1]:
      ret = ret * x0 + x
    return ret

  def degree(s):
    return len(s.coeffs) - 1

  def monic(s):
    return s / s[-1]

  def shift(s, i):
    if i > 0:
      return Polynomial([0] * abs(i) + s.coeffs)
    else:
      return Polynomial(s.coeffs[abs(i):])

  def trim(s):
    i = 0
    for x in s.coeffs[::-1]:
      if x != 0:
        break
      i += 1
    if i > 0:
      s.coeffs = s.coeffs[:-i]

if __name__ == "__main__":
  poly = Polynomial([1, 2, 3])
  print poly
  print poly.apply(2)

  print Polynomial([1, 0, QQ(3)]) + Polynomial([0, 2, 0, 4])
  print Polynomial([1, 0, 3, 4, 2, 2, 2]) + Polynomial([0, 2, 0, 4, 0, -2, -2])
  print Polynomial([1, 0, 3]) + 1
  print 1 - Polynomial([1, 0, 3])
  print Polynomial([-3, 1]) * Polynomial([3, 1])
  print repr(Polynomial([-3, 1]) * Polynomial([3, 1]))
  print map(str, (Polynomial([-3, 1]) * Polynomial([3, 1])) / Polynomial([-3, 1]))
  print Polynomial([1, 0, 0, 3]).monic()
  print Polynomial([1, 0, 0, QQ(3)]).monic()
  f = Polynomial(1).shift(10)
  g = Polynomial(1).shift(20)
  print f * g
