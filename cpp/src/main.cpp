#include <ecpy.h>
using namespace std;

int main(int ac, char **av) {
  auto F = FiniteField(127);
  auto x = F(4);
  auto y = F(128);
  auto z = F(-1);
  cout << x << endl;
  cout << y << endl;
  cout << z << endl;
  cout << x+y << endl;
  cout << y+z << endl;
  cout << F << endl;
  auto a = F(0);
  auto b = F(1);

  EllipticCurve<FiniteField> E(F, 0, 1);

  auto P = E(58, 101);
  auto Q = E(107, 111);
  cout << P << endl;
  cout << Q << endl;
  cout << P+Q << endl;
  cout << Q+Q << endl;
  return 0;
}
