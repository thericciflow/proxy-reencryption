#include "ecpy_native.h"

using namespace std;

py_object to_py_object(PyObject *obj) {
  return py_object(boost::python::handle<>(boost::python::borrowed(obj)));
}

ZZ modinv(ZZ _a, ZZ _modulo) {
  auto a = _a.to_mpz_class();
  auto modulo = _modulo.to_mpz_class();
  mpz_class t;
  mpz_invert(t.get_mpz_t(), a.get_mpz_t(), modulo.get_mpz_t());
  return ZZ{t};
}


BOOST_PYTHON_MODULE(ecpy_native) {
  using namespace boost::python;

  initializeZZPointConverter();
  initializeZZConverter();

  class_<ZZPoint>("ZZPoint", init<py_object, py_object>())
    .def(init<py_object, py_object, py_object>())
    .def_readwrite("x", &ZZPoint::x)
    .def_readwrite("y", &ZZPoint::y)
    .def_readwrite("z", &ZZPoint::z)
    .def("__str__", &ZZPoint::to_string)
    .def("__repr__", &ZZPoint::to_raw_string)
  ;

  class_<ZZ>("ZZ", init<py_object>())
    .def("__int__", &ZZ::to_object)
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
}
