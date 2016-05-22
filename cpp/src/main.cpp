#include <ecpy.h>
using namespace std;

int main(int ac, char **av) {
  try {
    /*
    auto F = FiniteField(7);
    auto x = F(4);
    auto y = F(5);
    cout << x << endl;
    cout << y << endl;
    cout << x + y << endl;
    cout << x * y << endl;
    */
    auto F = FiniteField(631);
    auto E = EllipticCurve<FiniteField>(F, 30, 34);
    auto P = E(36, 60);
    auto Q = E(121, 387);
    auto S = E(0, 36);
    cout << weil_pairing(E, P, Q, S, 5) << endl;
    cout << tate_pairing(E, P, Q, 5, 1) << endl;
  } catch (const char *message) {
    cerr << message << endl;
  }
  return 0;
}
