from .Ring import Ring, RingElement

class QuotientRing(Ring):
  def __init__(s, base_ring, modulo):
    assert isinstance(base_ring, Ring)
    Ring.__init__(s, QuotientRingElement)
    s.base_ring = base_ring
    s.modulo = modulo

  def _add(s, A, B):
    print A, B

  def __str__(s):
    return 'Quotient ring of %s modulo %s' % (s.base_ring, s.modulo)

def QuotientRingElement(RingElement):
  def __init__(s, ring, x):
    RingElement.__init__(s, ring, x)
