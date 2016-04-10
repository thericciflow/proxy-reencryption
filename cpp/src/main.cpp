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
  return 0;
}
