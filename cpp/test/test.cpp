#include "ecpy_native.h"
#include "ZZ_impl.h"
#include "FF_impl.h"
#include "EF_impl.h"
#include "EC_impl.h"
#include <stdexcept>
#include <boost/format.hpp>
using namespace std;
using namespace g_object;

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

TEST(ec) {
  {
    auto E = AS_OBJECT(EC_create("0", "1", "FF"));
    ES_ASSERT_EQ_FM(to_std_string(E), "Elliptic Curve on Finite Field: y^2 = x^3 + 1", "str(E)");
    auto P = AS_OBJECT(EP_FF_create(to_EC_FF(E), "0", "1", "1", "7"));
    auto x = AS_OBJECT(FF_create_from_mpz_class(3, 7));
    auto y = AS_OBJECT(FF_create_from_mpz_class(0, 7));
    auto z = AS_OBJECT(FF_create_from_mpz_class(1, 7));
    auto Q = AS_OBJECT(EP_FF_create_with_FF(to_EC_FF(E), to_FF(x), to_FF(y), to_FF(z)));
    ES_ASSERT_NEQ_FM(equals(P, Q), true, "P != Q");
    ES_ASSERT_EQ_FM(equals(P, P), true, "P == P");
    ES_ASSERT_EQ_FM(equals(Q, Q), true, "Q == Q");
    destroy(x);
    destroy(y);
    destroy(z);
    destroy(P);
    destroy(Q);
    destroy(E);
  }
  {
    auto E = AS_OBJECT(EC_create("0", "1", "EF"));
    ES_ASSERT_EQ_FM(to_std_string(E), "Elliptic Curve on Extended Finite Field: y^2 = x^3 + 1", "str(E)");

    auto P = AS_OBJECT(EP_EF_create(to_EC_EF(E), "39", "39", "3", "0", "1", "0", "41", "x^2+x+1"));
    auto x = AS_OBJECT(EF_create_from_mpz_class(5, 5, 41, IrreduciblePolynomialType::X2_X_1));
    auto y = AS_OBJECT(EF_create_from_mpz_class(9, 0, 41, IrreduciblePolynomialType::X2_X_1));
    auto z = AS_OBJECT(EF_create_from_mpz_class(1, 0, 41, IrreduciblePolynomialType::X2_X_1));
    auto Q = AS_OBJECT(EP_EF_create_with_EF(to_EC_EF(E), to_EF(x), to_EF(y), to_EF(z)));
    ES_ASSERT_NEQ_FM(equals(P, Q), true, "P != Q");
    ES_ASSERT_EQ_FM(equals(P, P), true, "P == P");
    ES_ASSERT_EQ_FM(equals(Q, Q), true, "Q == Q");
    destroy(x);
    destroy(y);
    destroy(z);
    destroy(P);
    destroy(Q);
    destroy(E);
  }
}

TEST(ef) {
  {
    auto x = AS_OBJECT(EF_create("3", "0", "7", "x^2+1"));
    auto y = AS_OBJECT(EF_create_from_mpz_class(0, 5, 7, IrreduciblePolynomialType::X2_1));

    ES_ASSERT_EQ_FM(to_std_string(x),
        "(3, 0) over Extended Field GF(7^2), Irreducible Polynomial: x^2 + 1",
        "str(x)");

    {
      auto z = add(x, y);
      auto t = AS_OBJECT(EF_create_from_mpz_class(3, 5, 7, IrreduciblePolynomialType::X2_1));
      ES_ASSERT_EQ_FM(equals(z, t), true, "x+y = 3+5i");
      destroy(z);
      destroy(t);
    }

    {
      auto z = neg(x);
      auto t = AS_OBJECT(EF_create_from_mpz_class(4, 0, 7, IrreduciblePolynomialType::X2_1));
      ES_ASSERT_EQ_FM(equals(z, t), true, "-x = 4");
      destroy(z);
      destroy(t);
    }

    {
      auto z = mul(x, y);
      auto t = AS_OBJECT(EF_create_from_mpz_class(0, 1, 7, IrreduciblePolynomialType::X2_1));
      ES_ASSERT_EQ_FM(equals(z, t), true, "x*y = i");
      destroy(z);
      destroy(t);
    }

    {
      auto z = inv(y);
      auto t = AS_OBJECT(EF_create_from_mpz_class(0, 4, 7, IrreduciblePolynomialType::X2_1));
      ES_ASSERT_EQ_FM(equals(z, t), true, "1/y = 4i");
      destroy(z);
      destroy(t);
    }

    destroy(x);
    destroy(y);
  }
  {
    auto x = AS_OBJECT(EF_create("61", "0", "41", "x^2 + x + 1"));
    auto y = AS_OBJECT(EF_create_from_mpz_class(0, 20, 41, IrreduciblePolynomialType::X2_X_1));
    {
      auto t = AS_OBJECT(EF_create_from_mpz_class(20, 0, 41, IrreduciblePolynomialType::X2_X_1));
      ES_ASSERT_EQ_FM(equals(x, t), true, "modulo check");
      destroy(t);
    }
    {
      auto z = add(x, y);
      auto t = AS_OBJECT(EF_create_from_mpz_class(20, 20, 41, IrreduciblePolynomialType::X2_X_1));
      ES_ASSERT_EQ_FM(equals(z, t), true, "x+y = 30+20w");
      destroy(z);
      destroy(t);
    }
    {
      auto z = neg(x);
      auto t = AS_OBJECT(EF_create_from_mpz_class(21, 0, 41, IrreduciblePolynomialType::X2_X_1));
      ES_ASSERT_EQ_FM(equals(z, t), true, "-x = 21");
      destroy(z);
      destroy(t);
    }
    {
      auto z = mul(x, y);
      auto t = AS_OBJECT(EF_create_from_mpz_class(0, 31, 41, IrreduciblePolynomialType::X2_X_1));
      ES_ASSERT_EQ_FM(equals(z, t), true, "x*y = 31w");
      destroy(z);
      destroy(t);
    }
    {
      auto z = inv(y);
      auto t = AS_OBJECT(EF_create_from_mpz_class(2, 2, 41, IrreduciblePolynomialType::X2_X_1));
      ES_ASSERT_EQ_FM(equals(z, t), true, "1/y = 2+2w");
      destroy(z);
      destroy(t);
    }

    destroy(x);
    destroy(y);
  }
}

TEST(ff) {
  auto x = AS_OBJECT(FF_create("3", "7"));
  auto y = AS_OBJECT(FF_create_from_mpz_class(13, 7));

  ES_ASSERT_EQ_M(to_ZZ(to_FF(x)->x)->x, 3, "x");
  // check modulo
  ES_ASSERT_EQ_M(to_ZZ(to_FF(y)->x)->x, 6, "y");
  ES_ASSERT_NEQ_M(to_ZZ(to_FF(y)->x)->x, 13, "y");
  ES_ASSERT_EQ_FM(to_std_string(x), "3 modulo 7", "str(x)");
  {
    auto z = add(x, y);
    ES_ASSERT_EQ_M(to_ZZ(to_FF(z)->x)->x, 2, "x+y");
    destroy(z);
  }
  {
    auto z = neg(x);
    ES_ASSERT_EQ_M(to_ZZ(to_FF(z)->x)->x, 4, "-x");
    destroy(z);
  }
  {
    auto z = mul(x, y);
    ES_ASSERT_EQ_M(to_ZZ(to_FF(z)->x)->x, 4, "x*y");
    destroy(z);
  }
  {
    auto z = div(x, y);
    ES_ASSERT_EQ_M(to_ZZ(to_FF(z)->x)->x, 4, "x/y");
    destroy(z);
  }
  destroy(x);
  destroy(y);
}

TEST(zz) {
  auto x = AS_OBJECT(ZZ_create("3"));
  auto y = AS_OBJECT(ZZ_create_from_mpz_class(9));
  ES_ASSERT_EQ(to_ZZ(x)->x, 3);
  ES_ASSERT_EQ(to_ZZ(y)->x, 9);
  ES_ASSERT_EQ_FM(to_std_string(x), "3", "str(x)==3");
  {
    auto z = add(x, y);
    ES_ASSERT_EQ_M(to_ZZ(z)->x, 12, "x+y");
    destroy(z);
  }
  {
    auto z = neg(x);
    ES_ASSERT_EQ_M(to_ZZ(z)->x, -3, "x-y");
    destroy(z);
  }
  {
    auto z = mul(x, y);
    ES_ASSERT_EQ_M(to_ZZ(z)->x, 27, "x*y");
    destroy(z);
  }
  {
    auto z = div(y, x);
    ES_ASSERT_EQ_M(to_ZZ(z)->x, 3, "y/x");
    destroy(z);
  }
  {
    auto a = AS_OBJECT(ZZ_create_from_mpz_class(24));
    auto b = AS_OBJECT(ZZ_create_from_mpz_class(17));
    auto z = mod(a, b);
    ES_ASSERT_EQ_M(to_ZZ(z)->x, 7, "a%b");
    destroy(a);
    destroy(b);
    destroy(z);
  }
  {
    auto a = ZZ_create_from_mpz_class(7);
    auto b = ZZ_create_from_mpz_class(17);
    auto z = ZZ_modinv(a, b);
    ES_ASSERT_EQ_FM(z->x, 5, "1/a mod b");
    destroy(AS_OBJECT(a));
    destroy(AS_OBJECT(b));
    destroy(AS_OBJECT(z));
  }
  {
    auto a = ZZ_create_from_mpz_class(12345);
    auto b = ZZ_create_from_mpz_class(331);
    auto z = ZZ_legendre(a, b);
    ES_ASSERT_EQ_FM(z, -1, "(12345|331) = -1");
    destroy(AS_OBJECT(a));
    destroy(AS_OBJECT(b));
  }
  {
    auto a = ZZ_create_from_mpz_class(1001);
    auto b = ZZ_create_from_mpz_class(9907);
    auto z = ZZ_jacobi(a, b);
    ES_ASSERT_EQ_FM(z, -1, "(1001|9907) = -1");
    destroy(AS_OBJECT(a));
    destroy(AS_OBJECT(b));
  }
  destroy(x);
  destroy(y);
}

void exec_test() {
  zz_test();
  ff_test();
  ef_test();
  ec_test();
  cout << boost::format("[+] %d Test(s) finished. %d Test(s) success, %d Test(s) fail.")
    % (ac_count + wa_count)
    % ac_count
    % wa_count
    << endl;
}

int main(int ac, char **av) {
  exec_test();
  return wa_count;
}
