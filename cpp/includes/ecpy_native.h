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
    template <class T>
    bool operator!=(const T& rhs) const;

    friend std::ostream& operator<<(std::ostream&, const ZZ&);
    friend ZZ operator+(const mpz_class& lhs, const ZZ& rhs);
    friend ZZ operator-(const mpz_class& lhs, const ZZ& rhs);
    friend ZZ operator*(const mpz_class& lhs, const ZZ& rhs);
    friend ZZ operator/(const mpz_class& lhs, const ZZ& rhs);

    template <class T>
    friend bool operator==(const T& lhs, const ZZ& rhs);
    template <class T>
    friend bool operator!=(const T& lhs, const ZZ& rhs);
};

template <class T>
bool ZZ::operator==(const T& rhs) const {
  return x == rhs;
}

template <class T>
bool operator==(const T& lhs, const ZZ& rhs) {
  return lhs == rhs.x;
}

template <class T>
bool ZZ::operator!=(const T& rhs) const {
  return !(x == rhs);
}

template <class T>
bool operator!=(const T& lhs, const ZZ& rhs) {
  return !(lhs == rhs.x);
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
  std::string to_raw_string() const;
};

struct EC {
  ZZ a, b;
  EC(ZZ, ZZ);
  virtual ZZPoint add(ZZPoint, ZZPoint) const = 0;
  virtual ZZPoint sub(ZZPoint, ZZPoint) const = 0;
  virtual ZZPoint scalarMult(ZZPoint, ZZ) const = 0;
  virtual ZZPoint normalize(ZZPoint) const = 0;
  virtual std::string to_string() const;
};

struct EC_Mod : public EC {
  ZZ a, b, modulo;
  EC_Mod(ZZ, ZZ, ZZ);
  ZZPoint add(ZZPoint, ZZPoint) const override;
  ZZPoint sub(ZZPoint, ZZPoint) const override;
  ZZPoint scalarMult(ZZPoint, ZZ) const override;
  ZZPoint normalize(ZZPoint) const override;
  std::string to_string() const override;
};

void initializeZZPointConverter();
void initializeZZConverter();

py_object to_py_object(PyObject *);
ZZ modinv(ZZ _a, ZZ _modulo);

bool EC_ZZPoint_Is_Infinity(ZZPoint);
bool EC_Equals_Mod(ZZPoint P, ZZPoint Q, ZZ a, ZZ b, ZZ modulo);
