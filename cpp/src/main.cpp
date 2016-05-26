#include <ecpy.h>
using namespace std;

constexpr int CYCLE = 10000;

#define ES_TIME_TEST(init, x) do {\
  clock_t start, end;\
  cout << "Start (" << #x << ")... " << endl; \
  init; \
  start = clock(); \
  for (int i = 0; i < CYCLE; i++) { x; } \
  end = clock(); \
  cout << "\tFinished." << endl << "Time: " << ((double)end - start)/CLOCKS_PER_SEC/CYCLE * 1e+6 << "usec" << endl; \
} while (0)

int main(int ac, char **av) {
  try {
    auto F = FiniteField(631);
    auto E = EllipticCurve<FiniteField>(F, 30, 34);
    auto P = E(36, 60);
    auto Q = E(121, 387);
    auto S = E(0, 36);
    cout << weil_pairing(E, P, Q, S, 5) << endl;
    cout << tate_pairing(E, P, Q, 5, 1) << endl;
    ES_TIME_TEST(FiniteField F(631); EllipticCurve<FiniteField> E(F, 30, 34); auto P = E(36, 60); auto Q = E(121, 387); auto S = E(0, 36);, weil_pairing(E, P, Q, S, 5));
    ES_TIME_TEST(FiniteField F(631); EllipticCurve<FiniteField> E(F, 30, 34); auto P = E(36, 60); auto Q = E(121, 387); auto S = E(0, 36);, tate_pairing(E, P, Q, 5, 1));
  } catch (const char *message) {
    cerr << message << endl;
  }
  return 0;
}
