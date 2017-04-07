from .polynomial_uni import UnivariatePolynomialElement
from .polynomial_uni import UnivariatePolynomialRing
from .Ring import Ring, RingElement
from six.moves import map, xrange, zip_longest
from ecpy.utils import memoize
from ecpy.fields import QQ
from copy import deepcopy
import itertools

class BivariatePolynomialRing(Ring):
  '''
  Bivariate Polynomial Ring
  '''
  def __init__(s, field, gens):
    '''
    Args:
      field : A base field class
      gens : generator names

    Example:
      PR = BivariatePolynomialRing(RR, ['x', 'y'])
    '''
    Ring.__init__(s, BivariatePolynomialElement)
    assert len(gens) == 2
    s.field = field
    s.gen_names = gens

  def gens(s):
    ret = []
    ret += [s.element_class(s, [[0, 1]])]
    ret += [s.element_class(s, [[0], [1]])]
    return ret

  def __str__(s):
    return 'Bivariate Polynomial Ring over %s' % s.field

  def __repr__(s):
    return '%s(%r)' % (s.__class__.__name__, s.field)


class BivariatePolynomialElement(RingElement):
  def __init__(s, poly_ring, args):
    RingElement.__init__(s, poly_ring, args)
    if isinstance(args, BivariatePolynomialElement):
      s.coeffs = args.coeffs
    else:
      s.coeffs = args
    s.trim()
    s.coeffs = list(map(lambda y: list(map(s.ring.field, y)), s.coeffs))

  def __add__(s, rhs):
    if isinstance(rhs, BivariatePolynomialElement):
      A = list(s.coeffs)
      B = list(rhs.coeffs)
      ret = []
      for x, y in zip_longest(A, B, fillvalue=[0]):
        t = []
        for xs, ys in zip_longest(x, y, fillvalue=0):
          t += [xs + ys]
        ret += [t]
      return BivariatePolynomialElement(s.ring, ret)
    else:
      return BivariatePolynomialElement(s.ring, [[s.coeffs[0][0] + rhs] + s.coeffs[0][1:]] + s.coeffs[1:])

  def __eq__(s, rhs):
    if isinstance(rhs, BivariatePolynomialElement):
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
    if isinstance(rhs, BivariatePolynomialElement):
      if rhs.degree() == 0:
        return s * rhs[0][0]
      elif s.degree() == 0:
        return rhs * s[0][0]
      else:
        PR = UnivariatePolynomialRing(s.ring.field)
        A = s.coeffs
        B = rhs.coeffs
        deg_total_1 = max([len(X) + len(Y) - 1 for X, Y in itertools.product(A, B)])
        deg_total_2 = len(list(itertools.product(A, B)))
        ret = [[0] * deg_total_1 for _ in xrange(deg_total_2)]
        deg1 = 0
        for X in A:
          deg2 = 0
          for Y in B:
            for x, y in enumerate(X):
              for u, v in enumerate(Y):
                ret[deg1 + deg2][x + u] += y * v
            deg2 += 1
          deg1 += 1
        return BivariatePolynomialElement(s.ring, ret)
    else:
      return BivariatePolynomialElement(s.ring, map(lambda y: map(lambda x: rhs * x, y), s.coeffs))

  def __ne__(s, rhs):
    return not (s == rhs)

  def __neg__(s):
    return BivariatePolynomialElement(s.ring, map(lambda y: map(lambda x: -x, y), s.coeffs))

  def __mod__(s, rhs):
    return divmod(s, rhs)[1]

  def __pow__(s, rhs, mod=0):
    if rhs == 0:
      return BivariatePolynomialElement(s.ring, [[1]])
    d = rhs
    if d < 0:
      x = 1 / s
      d = -d
    else:
      x = s
    bits = list(map(int, bin(d)[2:]))[::-1]
    if bits[0]:
      res = x
    else:
      res = BivariatePolynomialElement(s.ring, [[1]])
    b = 0
    for cur in bits[1:]:
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
    if isinstance(lhs, BivariatePolynomialElement):
      return s + lhs
    else:
      return BivariatePolynomialElement(s.ring, [[lhs + s.coeffs[0][0]] + s.coeffs[0][1:]] + s.coeffs[1:])

  def __rsub__(s, lhs):
    return lhs + (-1*s)

  def __str__(s):
    if len(s.coeffs) == 0:
      return "0"
    res = []
    deg1 = 0
    for y in s.coeffs:
      deg2 = 0
      res2 = []
      for x in y:
        r = ""
        if x != 0:
          if (x != -1 and x != 1) or deg2 == 0:
            r += str(x)
          elif x == -1:
            r += "-"
          if deg2 > 0:
            r += s.ring.gen_names[0]
            if deg2 > 1:
              r += "^%d" % deg2
        if r != "":
          res2 += [r]
        deg2 += 1
      r = "+".join(res2[::-1]).replace("+-", "-")
      if len(r) != 0:
        if deg1 > 0:
          if deg2 > 2:
            r = '(' + r + ')'
          if deg1 == 1:
            r += s.ring.gen_names[1]
          else:
            r += '%s^%d' % (s.ring.gen_names[1], deg1)
          if r[:2] == '1y':
            r = r.lstrip('1')
      if r != "":
        res += [r]
      deg1 += 1
    if len(res) == 0:
      return '0'
    return "+".join(res).replace("+-", "-")

  def __sub__(s, rhs):
    return s + (-rhs)

  @memoize
  def apply(s, x0, y0):
    """
    Using Horner's method
    """
    ret = 0
    for y in s.coeffs[::-1]:
      t = 0
      for x in y[::-1]:
        t = t * x0 + x
      ret = ret * y0 + t
    return ret

  def degree(s):
    return max([len(x) + i - 1 for i, x in enumerate(s.coeffs)])

  def monic(s):
    return s / s[-1]

  def trim(s):
    res = []
    for y in s.coeffs:
      i = 0
      r = list(y)
      for x in r[::-1]:
        if x != 0:
          break
        i += 1
      if 0 < i < len(r):
        r = r[:-i]
      elif i == len(r):
        r = [0]
      res += [r]
    i = 0
    for r in res[::-1]:
      if not (len(r) == 1 and r[0] == 0):
        break
      i += 1
    if i < len(res):
      if 0 < i:
        s.coeffs = res[:-i]
      else:
        s.coeffs = res
    elif i == len(res):
      s.coeffs = [[0]]

  def __hash__(s):
    return hash(''.join(map(str, s.coeffs)) + str(s.ring))
