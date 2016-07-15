#include "ecpy_native.h"
#include <stdexcept>
#include <boost/format.hpp>
using namespace std;

static int ac_count = 0;
static int wa_count = 0;

#define ES_ASSERT_EQ_M(x,y,m) ES_ASSERTION(x == y, m " == " #y)

#define ES_ASSERT_NEQ_M(x,y,m) ES_ASSERTION(x != y, m " != " #y)

#define ES_ASSERT_EQ(x,y) ES_ASSERTION(x == y, #x " == " #y)

#define ES_ASSERT_NEQ(x,y) ES_ASSERTION(x != y, #x " != " #y)

#define ES_ASSERTION(cond, msg) do {\
  std::cout << "[+] "<< msg << "...";\
  try { \
    if(!(cond)) { \
      std::cout << "\033[31mFAILED\033[0m" << std::endl; \
      wa_count++; \
    } else { \
      std::cout << "\033[32mOK\033[0m" << std::endl; \
      ac_count++;\
    }\
  } catch (const std::runtime_error& e) { \
      std::cout << "\033[31mFAILED(EXCEPTION)\033[0m" << std::endl; \
      std::cerr << "[-] \033[31mAssertion Failed: <" \
          << __FILE__ << "> " << __FUNCTION__ << ":" << __LINE__ \
          << "(Exception occurerd!) -> !(" << #cond << ")\033[0m" \
          << "\n\t-> \033[01;04;31m" << e.what() << "\033[0m" << std::endl;\
      wa_count++; \
  } \
} while(0)

void zz_test() {
  auto x = ZZ_create("3");
  auto y = ZZ_create_from_mpz_class(9);
  ES_ASSERT_EQ(x->x, 3);
  ES_ASSERT_EQ(y->x, 9);
  ES_ASSERT_EQ(strcmp(ZZ_to_string(x), "3"), 0);
  {
    auto z = ZZ_add(x, y);
    ES_ASSERT_EQ_M(z->x, 12, "x+y");
    ZZ_destroy(z);
  }
  {
    auto z = ZZ_neg(x);
    ES_ASSERT_EQ_M(z->x, -3, "x-y");
    ZZ_destroy(z);
  }
  {
    auto z = ZZ_mul(x, y);
    ES_ASSERT_EQ_M(z->x, 27, "x*y");
    ZZ_destroy(z);
  }
  {
    auto z = ZZ_div(y, x);
    ES_ASSERT_EQ_M(z->x, 3, "y/x");
    ZZ_destroy(z);
  }
  {
    auto a = ZZ_create_from_mpz_class(24);
    auto b = ZZ_create_from_mpz_class(17);
    auto z = ZZ_mod(a, b);
    ES_ASSERT_EQ_M(z->x, 7, "a%b");
    ZZ_destroy(a);
    ZZ_destroy(b);
    ZZ_destroy(z);
  }
  {
    auto a = ZZ_create_from_mpz_class(7);
    auto b = ZZ_create_from_mpz_class(17);
    auto z = ZZ_modinv(a, b);
    ES_ASSERT_EQ_M(z->x, 5, "modinv(a, b)");
    ZZ_destroy(a);
    ZZ_destroy(b);
    ZZ_destroy(z);
  }
  ZZ_destroy(x);
}

void exec_test() {
  zz_test();
  cout << boost::format("[+] %d Test(s) finished. %d Test(s) success, %d Test(s) fail.")
    % (ac_count + wa_count)
    % ac_count
    % wa_count
    << endl;
}

int main(int ac, char **av) {
  exec_test();
  return 0;
}
