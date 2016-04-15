#include <ecpy.h>
using namespace std;

bool is_prime(const mpz_class& x) {
  return mpz_probab_prime_p(MPZ_T(x), 1000);
}
