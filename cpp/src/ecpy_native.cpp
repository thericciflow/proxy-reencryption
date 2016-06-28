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
}
