#include "ecpy_native.h"

using namespace std;
using py_object = boost::python::object;

py_object add(py_object a, py_object b) {
  return a + b;
}

BOOST_PYTHON_MODULE(ecpy_native) {
  using namespace boost::python;

  def("add", &add);
}
