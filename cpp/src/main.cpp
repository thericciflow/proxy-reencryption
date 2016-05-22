#include <ecpy.h>
using namespace std;

int main(int ac, char **av) {
  try {
    auto F = FiniteField(7);
    auto x = F(4);
    auto y = F(5);
    cout << x << endl;
    cout << y << endl;
    cout << x + y << endl;
    return 0;
  } catch (const char *message) {
    cerr << message << endl;
  }
}
