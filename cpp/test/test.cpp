#include "ecpy_native.h"
#include <boost/format.hpp>
#include <stdexcept>
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
  auto ff = FF(7);
  auto x = FF_create_elem(3);
  auto y = FF_create_elem(6);
  ES_ASSERT_EQ(x->v, 3);
  ES_ASSERT_EQ(y->v, 6);
  {
    auto t = ff.add(x, y);
    ES_ASSERT_EQ_FM(t->v, 2, "x+y");
    delete t;
  }
  {
    auto t = ff.sub(x, y);
    ES_ASSERT_EQ_FM(t->v, 4, "x-y");
    delete t;
  }
  {
    auto t = ff.mul(x, y);
    ES_ASSERT_EQ_FM(t->v, 4, "x*y");
    delete t;
  }
  {
    auto t = ff.div(x, y);
    ES_ASSERT_EQ_FM(t->v, 4, "x/y");
    delete t;
  }
  {
    auto t = ff.pow(x, y);
    ES_ASSERT_EQ_FM(t->v, 1, "x^y");
    delete t;
  }
  delete x;
  delete y;
}

void exec_test() {
  ff_test();
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
