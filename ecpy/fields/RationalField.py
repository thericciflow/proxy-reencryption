from ecpy.fields.FractionField import FractionField, FractionFieldElement
from ecpy.rings.Integer import ZZ


class RationalField(FractionField):
  def __init__(s):
    FractionField.__init__(s, ZZ)
    s.element_class = RationalFieldElement

class RationalFieldElement(FractionFieldElement):
  def __int__(s):
    if s.p % s.q != 0:
      raise ValueError('Can\'t divide a value: %s' % s)
    return s.p // s.q

QQ = RationalField()
