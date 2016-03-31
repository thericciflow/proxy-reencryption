from Zmod import Zmod, ZmodElement


class FiniteField(Zmod):
  """
  Finite Field Class
  """
  def __init__(s, p):
    """
    Constructor of FiniteField
    p should be prime
    """
    Zmod.__init__(s, p)
    if s.element_class != FiniteFieldElement:
      raise ArithmeticError("Invalid Prime : %d" % p)
    s.p = p

  def __str__(s):
    return Zmod.__str__(s, "p")


class FiniteFieldElement(ZmodElement):
  def __init__(s, field, x):
    ZmodElement.__init__(s, field, x)
