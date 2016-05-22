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
    cout << (P-Q).normalize() << endl;
  } catch (const char *message) {
    cerr << message << endl;
  }
  return 0;
}
