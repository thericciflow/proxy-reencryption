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
