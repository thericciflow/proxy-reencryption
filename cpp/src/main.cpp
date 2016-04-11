#include <ecpy.h>
using namespace std;

template <class Field>
typename Field::Element h(
    EllipticCurvePoint<Field> P, 
    EllipticCurvePoint<Field> Q, 
    EllipticCurvePoint<Field> R) {
  if ((P == Q && P.y == 0) || (P != Q && P.x == Q.x)) {
    return R.x - P.x;
  }
  mpz_class L = P.line_coeff(Q);
  auto p = R.y - P.y - L * (R.x - P.x);
  auto q = R.x + P.x + Q.x - L * L;
  return p/q;
}

template <class Field>
typename Field::Element miller(
    EllipticCurve<Field> E,
    EllipticCurvePoint<Field> P,
    EllipticCurvePoint<Field> Q,
    mpz_class m) {
  if (P == Q) {
    return E.f(1);
  }
  auto f = E.f(1);
  auto T = EllipticCurvePoint<Field>(P);
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

int main(int ac, char **av) {
  auto F = FiniteField(631);
  auto E = EllipticCurve<FiniteField>(F, 30, 34);
  auto P = E(36, 60);
  auto Q = E(121, 387);
  auto S = E(0, 36);
  cout << "P = " << P << endl;
  cout << "Q = " << Q << endl;
  cout << "S = " << S << endl;
  cout << "5P = " << 5*P << endl;
  cout << miller(E, P, Q+S, 5) << endl;
  cout << miller(E, P, S, 5) << endl;
  cout << miller(E, Q, P-S, 5) << endl;
  cout << miller(E, Q, -S, 5) << endl;
  return 0;
}
