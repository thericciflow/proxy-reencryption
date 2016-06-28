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
    ZZ(mpz_class&&);

    PyObject *to_object() const;

    ZZ operator+(const ZZ& rhs) const;
    ZZ operator-(const ZZ& rhs) const;
    ZZ operator*(const ZZ& rhs) const;
    ZZ operator/(const ZZ& rhs) const;

    friend std::ostream& operator<<(std::ostream&, const ZZ&);
};

struct ZZPoint {
  ZZ x, y, z;
  ZZPoint(PyObject* _x, PyObject* _y, PyObject* _z) :
    x(_x), y(_y), z(_z) {}
  ZZPoint(PyObject* _x, PyObject* _y) :
    x(_x), y(_y) {}
  ZZPoint(py_object _x, py_object _y, py_object _z) :
    x(_x), y(_y), z(_z) {}
  ZZPoint(py_object _x, py_object _y) :
    x(_x), y(_y) {}

  std::string to_string() const;
};

struct ZZPoint_to_python;
struct ZZPoint_from_python;
void initializeZZPointConverter();
py_object to_py_object(PyObject *);
