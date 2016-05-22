#pragma once
#include <ecpy.h>

template <class Field>
auto h(
    const EllipticCurvePoint<Field>& P, 
    const EllipticCurvePoint<Field>& Q, 
    const EllipticCurvePoint<Field>& R)
  -> typename Field::Element {
  using Element = typename Field::Element;
  if ((P == Q && P.y == 0) || (P != Q && P.x == Q.x)) {
    return R.x - P.x;
  }
  Field f = P.curve->f;
  auto L = P.line_coeff(Q).get_mpz_class();
  auto p = R.y - P.y - L * (R.x - P.x);
  auto q = R.x + P.x + Q.x - L * L;
  return p / q;
}

template <class Field>
auto miller(
    EllipticCurve<Field>& E,
    const EllipticCurvePoint<Field>& P,
    const EllipticCurvePoint<Field>& Q,
    const mpz_class& m)
  -> typename Field::Element {
  if (P == Q) {
    return E.f(1);
  }
  auto f = E.f(1);
  EllipticCurvePoint<Field> T(P);
  uint32_t n = 0;
  for(;(m >> n) != 0; n++);
  for(int i = n - 2; i >= 0; i--) {
    f = f * f * h(T, T, Q);
    T += T;
    T = T.normalize();
    if (((m >> i) & 1) == 1) {
      f = f * h(T, P, Q);
      T += P;
      T = T.normalize();
    }
  }
  return f;
}

template <class Field>
auto weil_pairing(
    EllipticCurve<Field>& E,
    EllipticCurvePoint<Field>& P,
    EllipticCurvePoint<Field>& Q,
    EllipticCurvePoint<Field>& S,
    const mpz_class& m)
  -> typename Field::Element {
  auto fpqs = miller(E, P, (Q+S).normalize(), m);
  auto fps  = miller(E, P, S,   m);
  auto fqps = miller(E, Q, (P-S).normalize(), m);
  auto fqs  = miller(E, Q, -S,  m);
  return (fpqs * fqs) / (fps * fqps);
}

FiniteFieldElement tate_pairing(
    EllipticCurve<FiniteField>& E,
    const EllipticCurvePoint<FiniteField>& P,
    const EllipticCurvePoint<FiniteField>& Q,
    const mpz_class& m,
    const mpz_class& k);
