#include "ecpy_native.h"
#include <stdexcept>
#include <boost/format.hpp>
using namespace std;

static int ac_count = 0;
static int wa_count = 0;

#define ES_ASSERT_EQ_FM(x,y,m) ES_ASSERTION(x == y, m)
#define ES_ASSERT_NEQ_FM(x,y,m) ES_ASSERTION(x != y, m)
#define ES_ASSERT_EQ_M(x,y,m) ES_ASSERT_EQ_FM(x, y, m " == " #y)
#define ES_ASSERT_NEQ_M(x,y,m) ES_ASSERT_NEQ_FM(x, y, m " != " #y)
#define ES_ASSERT_EQ(x,y) ES_ASSERT_EQ_M(x, y, #x)
#define ES_ASSERT_NEQ(x,y) ES_ASSERT_NEQ_M(x, y, #x)

#define ES_ASSERTION(cond, msg) do {\
  cout << boost::format("[+] %-16s...%-8s") % msg % "";\
  try { \
    if(!(cond)) { \
      cout << "\033[31m[ FAILED ]\033[0m" << endl; \
      wa_count++; \
    } else { \
      cout << "\033[33m[   OK   ]\033[0m" << endl; \
      ac_count++;\
    }\
  } catch (const runtime_error& e) { \
      cout << "\033[31mFAILED(EXCEPTION)\033[0m" << endl; \
      cerr << "[-] \033[31mAssertion Failed: <" \
          << __FILE__ << "> " << __FUNCTION__ << ":" << __LINE__ \
          << "(Exception occurerd!) -> !(" << #cond << ")\033[0m" \
          << "\n\t-> \033[01;04;31m" << e.what() << "\033[0m" << endl;\
      wa_count++; \
  } \
} while(0)

#define TEST(name) void _ ## name ## _test(); void name ## _test() { \
  clock_t start, end; \
  double time;\
  cout << boost::format("Start Test: %s\n") % #name; \
  start = clock();\
  _ ## name ## _test(); \
  end = clock();\
  time = ((double)(end - start) / CLOCKS_PER_SEC); \
  cout << boost::format("Test Finished. Time: %s sec (%s usec)\n") % time % (time * 1e+6); \
} \
void _ ## name ## _test()

TEST(ff) {
  auto x = FF_create("3", "7");
  auto y = FF_create_from_mpz_class(13, 7);

  ES_ASSERT_EQ(x->x->x, 3);
  // check modulo
  ES_ASSERT_EQ(y->x->x, 6);
  ES_ASSERT_NEQ(y->x->x, 13);
  ES_ASSERT_EQ_FM(FF_to_string_as_std_string(x), "3 modulo 7", "str(x)");
  {
    auto z = FF_add(x, y);
    ES_ASSERT_EQ_M(z->x->x, 2, "x+y");
    FF_destroy(z);
  }
  {
    auto z = FF_neg(x);
    ES_ASSERT_EQ_M(z->x->x, 4, "-x");
    FF_destroy(z);
  }
  {
    auto z = FF_mul(x, y);
    ES_ASSERT_EQ_M(z->x->x, 4, "x*y");
    FF_destroy(z);
  }
  {
    auto z = FF_div(x, y);
    ES_ASSERT_EQ_M(z->x->x, 4, "x/y");
    FF_destroy(z);
  }
  FF_destroy(x);
  FF_destroy(y);
}

TEST(zz) {
  auto x = ZZ_create("3");
  auto y = ZZ_create_from_mpz_class(9);
  ES_ASSERT_EQ(x->x, 3);
  ES_ASSERT_EQ(y->x, 9);
  ES_ASSERT_EQ_FM(ZZ_to_string_as_std_string(x), "3", "str(x)==3");
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
    ES_ASSERT_EQ_FM(z->x, 5, "1/a mod b");
    ZZ_destroy(a);
    ZZ_destroy(b);
    ZZ_destroy(z);
  }
  {
    auto a = ZZ_create_from_mpz_class(12345);
    auto b = ZZ_create_from_mpz_class(331);
    auto z = ZZ_legendre(a, b);
    ES_ASSERT_EQ_FM(z, -1, "(12345|331) = -1");
    ZZ_destroy(a);
    ZZ_destroy(b);
  }
  {
    auto a = ZZ_create_from_mpz_class(1001);
    auto b = ZZ_create_from_mpz_class(9907);
    auto z = ZZ_jacobi(a, b);
    ES_ASSERT_EQ_FM(z, -1, "(1001|9907) = -1");
    ZZ_destroy(a);
    ZZ_destroy(b);
  }
  ZZ_destroy(x);
  ZZ_destroy(y);
}

void exec_test() {
  zz_test();
  ff_test();
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
