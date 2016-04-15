#pragma once
#include "ec.h"
#include "finitefield.h"

template <class Field>
inline auto h(
    EllipticCurvePoint<Field> P, 
    EllipticCurvePoint<Field> Q, 
    EllipticCurvePoint<Field> R)
  -> typename Field::Element {
  if ((P == Q && P.y == 0) || (P != Q && P.x == Q.x)) {
    return R.x - P.x;
  }
  Field f = P.curve->f;
  auto L = P.line_coeff(Q).get_mpz_class();
  typename Field::Element p, q, t;
  f.sub(p, R.y, P.y);
  f.sub(t, R.x, P.x);
  f.mul(t, L, t);
  f.sub(p, p, t);

  f.add(q, R.x, P.x);
  f.add(q, q, Q.x);
  f.mul(t, L, L);
  f.sub(q, q, t);
  f.div(t, p, q);
  return t;
}

template <class Field>
auto miller(
    EllipticCurve<Field>& E,
    EllipticCurvePoint<Field> P,
    EllipticCurvePoint<Field> Q,
    mpz_class m)
  -> typename Field::Element {
  if (P == Q) {
    return E.f(1);
  }
  auto f = E.f(1);
  auto T = EllipticCurvePoint<Field>(P);
  uint32_t n = 0;
  for(;(m >> n) != 0; n++);
  for(int i = n - 2; i >= 0; i--) {
    E.f.mul(f, f, f);
    E.f.mul(f, f, h(T, T, Q));
    T += T;
    if (((m >> i) & 1) == 1) {
      E.f.mul(f, f, h(T, P, Q));
      T += P;
    }
  }
  return f;
}

template <class Field>
auto weil_pairing(
    EllipticCurve<Field>& E,
    const EllipticCurvePoint<Field>& P,
    const EllipticCurvePoint<Field>& Q,
    EllipticCurvePoint<Field>& S,
    mpz_class m)
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
    mpz_class m,
    mpz_class k);
