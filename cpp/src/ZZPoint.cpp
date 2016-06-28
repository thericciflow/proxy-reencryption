#include "ecpy_native.h"

std::string ZZPoint::to_string() const {
  std::stringstream ret;
  ret << "ZZPoint (" << x << ", " << y << ", " << z << ")";
  return ret.str();
}

struct ZZPoint_to_python {
  static PyObject *convert(ZZPoint const & s) {
    PyObject *ret = PyList_New(3);
    PyList_SetItem(ret, 0, s.x.to_object());
    PyList_SetItem(ret, 0, s.y.to_object());
    PyList_SetItem(ret, 0, s.z.to_object());
    return ret;
  }
};

struct ZZPoint_from_python {
  ZZPoint_from_python() {
    boost::python::converter::registry::push_back(
      &convertible,
      &construct,
      boost::python::type_id<ZZPoint>()
    );
  }

  static void *convertible(PyObject *obj) {
    if (!PyList_Check(obj) && !PyTuple_Check(obj)) {
      return nullptr;
    } else {
      return obj;
    }
  }

  static void construct(PyObject *obj, boost::python::converter::rvalue_from_python_stage1_data *data) {
    uint32_t len = PyList_Size(obj);
    assert(len == 2 || len == 3);
    PyObject *x, *y, *z;
    if (len >= 2) {
      x = PyList_GetItem(obj, 0);
      y = PyList_GetItem(obj, 1);
      if (len == 3) {
        z = PyList_GetItem(obj, 2);
      } else {
        z = 0;
      }
    }
    void *storage = ((boost::python::converter::rvalue_from_python_storage<ZZPoint>*) data) -> storage.bytes;
    new (storage) ZZPoint(x, y, z);
  }
};

void initializeZZPointConverter() {
  using namespace boost::python;

  //to_python_converter<ZZPoint, ZZPoint_to_python>();

  ZZPoint_from_python();
}
