#pragma once
#include <ecpy.h>

template <class Field>
auto h(
    const EllipticCurvePoint<Field>& P, 
    const EllipticCurvePoint<Field>& Q, 
    const EllipticCurvePoint<Field>& R)
  -> typename Field::Element {
  using Element = typename Field::Element;
  if ((P == Q && P.y == 0) || (P != Q && P.x * Q.z == Q.x * P.z)) {
    return (R.x * P.z - P.x * R.z) / (P.z * R.z);
  }
  Field f = P.curve->f;
  auto L = P.line_coeff(Q);
  auto prz = P.z * R.z;
  /**
   * auto p = R.y / R.z - P.y / P.z - L * (R.x / R.z - P.x / P.z);
   * auto q = R.x / R.z + P.x / P.z + Q.x / Q.z - L * L;
   */
  auto pbar = R.y * P.z - P.y * R.z - L * (R.x * P.z - P.x * R.z);
  auto qbar = (P.x * Q.z * R.z) + (Q.x * prz) + (R.x * P.z * Q.z) - (L * L * prz * Q.z);
  return (pbar * Q.z) / qbar;
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
    if (((m >> i) & 1) == 1) {
      f = f * h(T, P, Q);
      T += P;
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
  auto fpqs = miller(E, P, Q+S, m);
  auto fps  = miller(E, P, S,   m);
  auto fqps = miller(E, Q, P-S, m);
  auto fqs  = miller(E, Q, -S,  m);
  return (fpqs * fqs) / (fps * fqps);
}

FiniteFieldElement tate_pairing(
    EllipticCurve<FiniteField>& E,
    const EllipticCurvePoint<FiniteField>& P,
    const EllipticCurvePoint<FiniteField>& Q,
    const mpz_class& m,
    const mpz_class& k);
