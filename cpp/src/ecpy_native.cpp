#include "ecpy_native.h"

using namespace std;

bool is_prime(mpz_class x) {
  return mpz_probab_prime_p(x.get_mpz_t(), 20) > 0;
}

