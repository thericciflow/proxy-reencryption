#include "ecpy_native.h"

using namespace std;

py_object to_py_object(PyObject *obj) {
  return py_object(boost::python::handle<>(boost::python::borrowed(obj)));
}

py_object add(py_object a, py_object b) {
  ZZ x {a};
  ZZ y {b};
  return to_py_object((x + y).to_object());
}

ZZ modinv(ZZ _a, ZZ _modulo) {
  auto a = _a.to_mpz_class();
  auto modulo = _modulo.to_mpz_class();
  mpz_class t;
  mpz_invert(t.get_mpz_t(), a.get_mpz_t(), modulo.get_mpz_t());
  return ZZ{t};
}

#define ZZPOINT_Is_Infinity(P) ((P).x == 0 && (P).y == 1 && (P).z == 0)

bool EC_Equals_Mod(ZZPoint P, ZZPoint Q, ZZ a, ZZ b, ZZ modulo) {
  return (P.x * Q.y) % modulo == (P.y * Q.x) % modulo;
}

ZZPoint EC_Add_Mod(ZZPoint P, ZZPoint Q, py_object _a, py_object _b, py_object _modulo) {
  ZZ a { _a };
  ZZ b { _b };
  ZZ modulo { _modulo };
  if (ZZPOINT_Is_Infinity(P)) {
    return Q;
  } else if (ZZPOINT_Is_Infinity(Q)) {
    return P;
  } else if (P.x == Q.x && (P.y + Q.y) % modulo == 0) {
    return ZZPoint(0, 1, 0);
  }
  if (EC_Equals_Mod(P, Q, a, b, modulo)) {
    auto u = (3 * P.x * P.x + a * P.z * P.z) % modulo;
    auto v = (P.y * P.z) % modulo;
    auto yv = (P.y * v) % modulo;
    auto yv4 = (4 * yv) % modulo;
    auto w = (u * u - 2 * P.x * yv4) % modulo;
    auto Rx = (2 * v * w) % modulo;
    auto Ry = ((u * ((P.x * yv4 - w) % modulo)) - 8 * yv * yv) % modulo;
    auto Rz = (8 * v * v * v) % modulo;
    return ZZPoint(Rx, Ry, Rz);
  } else {
    auto u = (Q.y * P.z - P.y * Q.z) % modulo;
    auto v = (Q.x * P.z - P.x * Q.z) % modulo;
    auto v2 = (v * v) % modulo;
    auto v3 = (v2 * v) % modulo;
    auto w = (u * u * P.z * P.z - v3 - 2 * v2 * P.x * Q.z) % modulo;
    auto Rx = (v * w) % modulo;
    auto Ry = (u * (v2 * P.x * Q.z - w) - v3 * P.y * Q.z) % modulo;
    auto Rz = (v3 * P.z * Q.z) % modulo;
    return ZZPoint(Rx, Ry, Rz);
  }
}

ZZPoint EC_Normalize_Mod(ZZPoint P, py_object _modulo) {
  ZZ modulo {_modulo};
  auto inv = static_cast<ZZ>(modinv(P.z, modulo));
  return ZZPoint((P.x * inv) % modulo, (P.y * inv) % modulo, ZZ(1));
}

BOOST_PYTHON_MODULE(ecpy_native) {
  using namespace boost::python;

  initializeZZPointConverter();

  def("add", &add);

  class_<ZZPoint>("ZZPoint", init<py_object, py_object>())
    .def(init<py_object, py_object, py_object>())
    .def_readwrite("x", &ZZPoint::x)
    .def_readwrite("y", &ZZPoint::y)
    .def_readwrite("z", &ZZPoint::z)
    .def("__str__", &ZZPoint::to_string)
  ;

  class_<ZZ>("ZZ", init<py_object>())
    .def("__int__", &ZZ::to_object)
  ;

  def("EC_Add_Mod", &EC_Add_Mod);
  def("EC_Normalize_Mod", &EC_Normalize_Mod);
}
