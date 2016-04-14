#include <ecpy.h>

FiniteFieldElement tate_pairing(
    EllipticCurve<FiniteField>& E,
    const EllipticCurvePoint<FiniteField>& P,
    const EllipticCurvePoint<FiniteField>& Q,
    mpz_class m,
    mpz_class k) {
    auto f = miller(E, P, Q, m);
    return f ^ (((E.f(E.f.p) ^ k) - 1) / m);
}
