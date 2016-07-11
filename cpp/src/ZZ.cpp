#include "ecpy_native.h"

ZZ::ZZ() : x(0) {}

ZZ::ZZ(py_object _x) 
  : x(boost::python::extract<char const *>(boost::python::str(_x))) {}
ZZ::ZZ(PyObject *_x)
  : x(boost::python::extract<char const *>(boost::python::str(to_py_object(_x)))) {}
ZZ::ZZ(mpz_class& _x)
  : x(_x) {}
ZZ::ZZ(mpz_class&& _x)
  : x(std::forward<mpz_class>(_x)) {}

PyObject *ZZ::to_object() const {
  return PyInt_FromString(const_cast<char *>(x.get_str().c_str()), nullptr, 10);
}

ZZ ZZ::operator+(const ZZ& rhs) const {
  return std::move(ZZ(x + rhs.x));
}

ZZ ZZ::operator-(const ZZ& rhs) const {
  return std::move(ZZ(x - rhs.x));
}

ZZ ZZ::operator*(const ZZ& rhs) const {
  return std::move(ZZ(x * rhs.x));
}

ZZ ZZ::operator/(const ZZ& rhs) const {
  return std::move(ZZ(x / rhs.x));
}

ZZ ZZ::operator%(const ZZ& rhs) const {
  ZZ ret {x % rhs.x};
  if (ret.x < 0) {
    ret.x += rhs.x;
  }
  return std::move(ret);
}

mpz_class ZZ::to_mpz_class() const {
  return x;
}

std::ostream& operator<<(std::ostream& os, const ZZ& z) {
  os << z.x;
  return os;
}

ZZ operator+(const mpz_class& lhs, const ZZ& rhs) {
  return ZZ(lhs + rhs.x);
}

ZZ operator-(const mpz_class& lhs, const ZZ& rhs) {
  return ZZ(lhs - rhs.x);
}

ZZ operator*(const mpz_class& lhs, const ZZ& rhs) {
  return ZZ(lhs * rhs.x);
}

ZZ operator/(const mpz_class& lhs, const ZZ& rhs) {
  return ZZ(lhs / rhs.x);
}

ZZ operator%(const mpz_class & lhs, const ZZ& rhs) {
  ZZ ret {lhs % rhs.x};
  if (ret.x < 0) {
    ret.x += rhs.x;
  }
  return std::move(ret);
}


struct ZZ_to_python {
  static PyObject *convert(ZZ const & s) {
    return s.to_object();
  }
};

struct ZZ_from_python {
  ZZ_from_python() {
    boost::python::converter::registry::push_back(
      &convertible,
      &construct,
      boost::python::type_id<ZZ>()
    );
  }

  static void *convertible(PyObject *obj) {
    if (!(PyInt_Check(obj) || PyLong_Check(obj))) {
      return nullptr;
    }
    return obj;
  }

  static void construct(PyObject *obj, boost::python::converter::rvalue_from_python_stage1_data *data) {
    void *storage = ((boost::python::converter::rvalue_from_python_storage<ZZ>*) data) -> storage.bytes;
    new (storage) ZZ(obj);
    data->convertible = storage;
  }
};

void initializeZZConverter() {
  using namespace boost::python;

  to_python_converter<ZZ, ZZ_to_python>();

  ZZ_from_python();
}
