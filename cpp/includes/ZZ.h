#pragma once
#include "ecpy_native.h"

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
    ZZ operator+(const T& rhs) const;
    template <class T>
    ZZ operator-(const T& rhs) const;
    template <class T>
    ZZ operator*(const T& rhs) const;
    template <class T>
    ZZ operator/(const T& rhs) const;
    template <class T>
    ZZ operator%(const T& rhs) const;

    template <class T>
    bool operator==(const T& rhs) const;
    template <class T>
    bool operator!=(const T& rhs) const;
    template <class T>
    bool operator<(const T& rhs) const;
    template <class T>
    bool operator>(const T& rhs) const;

    friend std::ostream& operator<<(std::ostream&, const ZZ&);
    friend ZZ operator+(const mpz_class& lhs, const ZZ& rhs);
    friend ZZ operator-(const mpz_class& lhs, const ZZ& rhs);
    friend ZZ operator*(const mpz_class& lhs, const ZZ& rhs);
    friend ZZ operator/(const mpz_class& lhs, const ZZ& rhs);
    friend ZZ operator%(const mpz_class& lhs, const ZZ& rhs);

    template <class T>
    friend bool operator==(const T& lhs, const ZZ& rhs);
    template <class T>
    friend bool operator!=(const T& lhs, const ZZ& rhs);
    template <class T>
    friend bool operator<(const T& lhs, const ZZ& rhs);
    template <class T>
    friend bool operator>(const T& lhs, const ZZ& rhs);
};

template <class T>
ZZ ZZ::operator+(const T& rhs) const {
  return std::move(ZZ(x + rhs));
}

template <class T>
ZZ ZZ::operator-(const T& rhs) const {
  return std::move(ZZ(x - rhs));
}

template <class T>
ZZ ZZ::operator*(const T& rhs) const {
  return std::move(ZZ(x * rhs));
}

template <class T>
ZZ ZZ::operator/(const T& rhs) const {
  return std::move(ZZ(x / rhs));
}

template <class T>
ZZ ZZ::operator%(const T& rhs) const {
  ZZ ret {x % rhs};
  if (ret.x < 0) {
    ret.x += rhs;
  }
  return std::move(ret);
}

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

template <class T>
bool ZZ::operator<(const T& rhs) const {
  return x < rhs;
}

template <class T>
bool operator<(const T& lhs, const ZZ& rhs) {
  return lhs < rhs.x;
}

template <class T>
bool ZZ::operator>(const T& rhs) const {
  return x > rhs;
}

template <class T>
bool operator>(const T& lhs, const ZZ& rhs) {
  return lhs > rhs.x;
}

void initializeZZConverter();
