#include <ecpy.h>
using namespace std;

bool is_prime(mpz_class x) {
  return mpz_probab_prime_p(MPZ_T(x), 1000);
}
