#include "ecpy_native.h"

using namespace std;
static random_device rnd_dev;
static mt19937 rnd_mt(rnd_dev());

py_object to_py_object(PyObject *obj) {
  return py_object(boost::python::handle<>(boost::python::borrowed(obj)));
}

inline ZZ _find_power_divisor(ZZ base, ZZ x, ZZ modulo=mpz_class(-1)) {
  auto k = 0_mpz;
  auto m = base;
  while (x % m == 0) {
    k += 1;
    m = m * base;
    if (modulo != -1) {
      m = m % modulo;
    }
  }
  return k;
}

inline ZZ _find_power(ZZ base, ZZ x, ZZ crib, ZZ modulo=mpz_class(-1)) {
  auto k = 1_mpz;
  if (modulo != -1) {
    while (powermod(x, power(base, k), modulo) != crib) {
      k += 1;
    }
  } else {
    while (power(x, power(base, k)) != crib) {
      k += 1;
    }
  }
  return k;
}

vector<ZZ> modular_square_root(ZZ a, ZZ m) {
  if (mpz_probab_prime_p(MPZ_T(m.to_mpz_class()), 20) != 0) {
    return tonelli_shanks(a, m);
  }
  if (m == 2) {
    return vector<ZZ> {a};
  }
  mpz_class t;
  if (m % 4 == 3) {
    auto r = powermod(a, (m + 1) / 4, m);
    return vector<ZZ> {r, m - r};
  }
  if (m % 8 == 5) {
    auto v = powermod(2 * a, (m - 5) / 8, m);
    auto i = powermod(2 * a * v, 2_mpz, m);
    auto r = (a * v * (i - 1)) % m;
    return vector<ZZ> { r, m - r };
  }
  if (m % 8 == 1) {
    auto e = _find_power_divisor(2_mpz, m - 1);
    auto q = (m - 1) / power(2_mpz, e);
    auto z = ZZ(1_mpz);
    auto k = power(2_mpz, e-1);
    while (powermod(z, k, m) == 1) {
      auto x = (rnd_mt() + 1) % m;
      z = powermod(x, q, m);
    }
    auto y = z;
    auto r = e;
    auto x = powermod(a, (q - 1) / 2, m);
    auto v = (a * x) % m;
    auto w = (v * x) % m;
    while (1) {
      if (w == 1) {
        return vector<ZZ> {v, m - v};
      }
      k = _find_power(2_mpz, w, 1_mpz, m);
      auto d = powermod(y, power(2_mpz, r - k - 1), m);
      y = powermod(d, 2_mpz, m);
      r = k;
      v = (d * v) % m;
      w = (w * y) % m;
    }
  }
  return vector<ZZ> {};
}

vector<ZZ> tonelli_shanks(ZZ n, ZZ p) {
  if (p % 4 == 3) {
    auto r = powermod(n, (p + 1) / 4, p);
    return vector<ZZ> {r, p - r};
  }
  auto s = _find_power_divisor(ZZ(2_mpz), p-1);
  auto q = (p - 1) / power(ZZ(2_mpz), s);
  auto z = ZZ(0_mpz);
  while (mpz_legendre(MPZ_T(z.to_mpz_class()), MPZ_T(p.to_mpz_class())) != -1) {
    z = ZZ(rnd_mt() + 1) % p;
  }
  auto c = powermod(z, q, p);
  auto r = powermod(n, (q + 1) / 2, p);
  auto t = powermod(n, q, p);
  auto m = s;
  while (1) {
    if (t % p == 1) {
      return vector<ZZ> {r, p - r};
    }
    auto i = _find_power(2_mpz, t, 1_mpz, p);
    auto b = powermod(c, power(2_mpz, m - i - 1), p);
    r = (r * b) % p;
    t = (t * (b * b)) % p;
    c = powermod(b, 2_mpz, p);
    m = i;
  }
}

BOOST_PYTHON_MODULE(ecpy_native) {
  using namespace boost::python;

  initializeZZPointConverter();
  initializeZZConverter();
  initializeVectorConverter<ZZ>();

  class_<ZZPoint>("ZZPoint", init<py_object, py_object>())
    .def(init<py_object, py_object, py_object>())
    .def_readwrite("x", &ZZPoint::x)
    .def_readwrite("y", &ZZPoint::y)
    .def_readwrite("z", &ZZPoint::z)
    .def("__str__", &ZZPoint::to_string)
    .def("__repr__", &ZZPoint::to_raw_string)
  ;

  class_<EC_Mod>("EC_Mod", init<ZZ, ZZ, ZZ>())
    .def_readwrite("a", &EC_Mod::a)
    .def_readwrite("b", &EC_Mod::b)
    .def_readwrite("modulo", &EC_Mod::modulo)
    .def("add", &EC_Mod::add)
    .def("sub", &EC_Mod::sub)
    .def("scalarMult", &EC_Mod::scalarMult)
    .def("normalize", &EC_Mod::normalize)
    .def("__str__", &EC_Mod::to_string)
    ;

  def("EC_ZZPoint_Is_Infinity", &EC_ZZPoint_Is_Infinity);

  def("modinv", &modinv);
  def("modular_square_root", &modular_square_root);
}
