#include "ecpy_native.h"

using namespace std;

bool is_prime(mpz_class x) {
  return mpz_probab_prime_p(x.get_mpz_t(), 20) > 0;
}


mpz_class modinv(mpz_class a, mpz_class m) {
  a %= m;
  if (a < 0) {
    a += m;
  }
  mpz_class t;
  mpz_invert(t.get_mpz_t(), a.get_mpz_t(), m.get_mpz_t());
  if (t < 0) {
    t += m;
  }
  return t;
}
