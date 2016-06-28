#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <sstream>

#include <gmp.h>
#include <gmpxx.h>
#include <Python.h>

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using py_object = boost::python::object;

class ZZ {
  mpz_class x;
  public:
    ZZ();
    ZZ(py_object);
    ZZ(PyObject*);
    ZZ(mpz_class&);
    ZZ(mpz_class&&);

    PyObject *to_object() const;
    mpz_class to_mpz_class() const;

    ZZ operator+(const ZZ& rhs) const;
    ZZ operator-(const ZZ& rhs) const;
    ZZ operator*(const ZZ& rhs) const;
    ZZ operator/(const ZZ& rhs) const;
    ZZ operator%(const ZZ& rhs) const;

    template <class T>
    bool operator==(const T& rhs) const;

    friend std::ostream& operator<<(std::ostream&, const ZZ&);
    friend ZZ operator+(const mpz_class& lhs, const ZZ& rhs);
    friend ZZ operator-(const mpz_class& lhs, const ZZ& rhs);
    friend ZZ operator*(const mpz_class& lhs, const ZZ& rhs);
    friend ZZ operator/(const mpz_class& lhs, const ZZ& rhs);

    template <class T>
    friend bool operator==(const T& lhs, const ZZ& rhs);
};

template <class T>
bool ZZ::operator==(const T& rhs) const {
  return x == rhs;
}

template <class T>
bool operator==(const T& lhs, const ZZ& rhs) {
  return lhs == rhs.x;
}

struct ZZPoint {
  ZZ x, y, z;
  template <class T>
  ZZPoint(T&& _x, T&& _y, T&& _z) :
    x(_x), y(_y), z(_z) {}
  template <class T>
  ZZPoint(T&& _x, T&& _y) :
    x(_x), y(_y), z(1) {}

  std::string to_string() const;
};

struct ZZPoint_to_python;
struct ZZPoint_from_python;
void initializeZZPointConverter();
py_object to_py_object(PyObject *);
