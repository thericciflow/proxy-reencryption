from AdditiveGroup import AdditiveGroup, AdditiveGroupElement

class EllipticCurve(AdditiveGroup):
  def __init__(s, field, a, b):
    AdditiveGroup.__init__(s, EllipticCurvePoint)
    s.a = a
    s.b = b

  def is_on_curve(s, point):
    return point.y ** 2 == point.x**3 + a*point.x + b


