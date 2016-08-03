#pragma once
#include "ecpy_native.h"

template <class T>
struct EC_elem;

template <class T>
struct EC {
  const T& base;
  mpz_class a, b;

  EC(const T& base, const mpz_class& a, const mpz_class& b) : base(base), a(a), b(b) {}

  EC() = default;
  ~EC() = default;
  EC(const EC<T>& ec) : base(ec.base), a(ec.a), b(ec.b) {};
  EC(EC<T>&& ec) : base(std::move(ec.base)), a(std::move(ec.a)), b(std::move(ec.b)) {};
  EC<T>& operator=(const EC<T>& ec);
  EC<T>& operator=(EC<T>&& ec);

  template <class E>
  void add(EC_elem<E>& ret, const EC_elem<E>& a, const EC_elem<E>& b) const;
  template <class E>
  void sub(EC_elem<E>& ret, const EC_elem<E>& a, const EC_elem<E>& b) const;
  template <class E>
  void mul(EC_elem<E>& ret, const EC_elem<E>& a, const EC_elem<E>& b) const;
  template <class E>
  bool equ(const EC_elem<E>& a, const EC_elem<E>& b) const;

  // ----------------- UNDEFINED(DELETED) -----------------
  template <class E>
  void div(EC_elem<E>& ret, const EC_elem<E>& a, const EC_elem<E>& b) const = delete;
  template <class E>
  void pow(EC_elem<E>& ret, const EC_elem<E>& a, const mpz_class& b) const = delete;
  // ------------------------------------------------------

  template <class E>
  EC_elem<E> to_affine(const EC_elem<E>& elem) const;
  template <class E>
  E line_coeff(const EC_elem<E>& P, const EC_elem<E>& Q) const;
  template <class E>
  bool is_on_curve(const EC_elem<E>& elem) const;
  template <class E>
  bool is_infinity(const EC_elem<E>& P) const;
  EC<T>* clone(void) const;
  std::string to_string(void) const;
};

template <class T>
struct EC_elem {
  T x, y, z;

  EC_elem(const mpz_class& x, const mpz_class& y, const mpz_class& z) : x(x), y(y), z(z) {}
  EC_elem(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}

  EC_elem() = default;
  ~EC_elem() = default;
  EC_elem(const EC_elem<T>& ee) : x(ee.x), y(ee.y), z(ee.z) {};
  EC_elem(EC_elem<T>&& ee) : x(std::move(ee.x)), y(std::move(ee.y)), z(std::move(ee.z)) {};
  EC_elem<T>& operator=(const EC_elem<T>&);
  EC_elem<T>& operator=(EC_elem<T>&&);

  EC_elem<T>* clone(void) const;
  std::string to_string(void) const;
};

template <class T>
template <class E>
void EC<T>::add(EC_elem<E>& ret, const EC_elem<E>& a, const EC_elem<E>& b) const {
  const static E zero {0};
  const static E one  {1};
  const static E two  {2};
  const static E three{3};
  const static E four {4};
  const static E eight{8};
  if (is_infinity(a)) {
    ret = b;
  } else if (is_infinity(b)) {
    ret = a;
  } else {
    E t;
    base.add(t, a.y, b.y);
    E c_a  {this->a};
    if (base.equ(a.x, b.x) && base.equ(t, zero)) {
      ret = EC_elem<E>(zero, one, zero);
    } else {
      E Rx, Ry, Rz;
      if (equ(a, b)) {
        E p, q, u, v, yv, yv4, w;
        base.mul(p, three, a.x);
        base.mul(p, p, a.x);
        base.mul(q, c_a, a.z);
        base.mul(q, q, a.z);
        base.add(u, p, q);

        base.mul(v, a.y, a.z);

        base.mul(yv, a.y, v);

        base.mul(yv4, yv, four);

        base.mul(p, u, u);
        base.mul(q, two, a.x);
        base.mul(q, q, yv4);
        base.sub(w, p, q);

        base.mul(p, two, v);
        base.mul(Rx, p, w);

        base.mul(p, a.x, yv4);
        base.sub(p, p, w);
        base.mul(p, u, p);
        base.mul(q, eight, yv);
        base.mul(q, q, yv);
        base.sub(Ry, p, q);

        base.mul(p, eight, v);
        base.mul(q, v, v);
        base.mul(Rz, p, q);
      } else {
        E p, q, r, u, v, v2, v3, w;
        base.mul(p, b.y, a.z);
        base.mul(q, a.y, b.z);
        base.sub(u, p, q);

        base.mul(p, b.x, a.z);
        base.mul(q, a.x, b.z);
        base.sub(v, p, q);

        base.mul(v2, v, v);

        base.mul(v3, v2, v);

        base.mul(p, u, u);
        base.mul(q, a.z, a.z);
        base.mul(p, p, q);
        base.sub(p, p, v3);
        base.mul(q, two, v2);
        base.mul(r, a.x, b.z);
        base.mul(q, q, r);
        base.sub(w, p, q);

        base.mul(Rx, v, w);

        base.mul(p, v2, a.x);
        base.mul(p, p, b.z);
        base.sub(p, p, w);
        base.mul(p, p, u);
        base.mul(q, v3, a.y);
        base.mul(q, q, b.z);
        base.sub(Ry, p, q);

        base.mul(p, v3, a.z);
        base.mul(Rz, p, b.z);
      }
      ret = {Rx, Ry, Rz};
    }
  }
}

template <class T>
template <class E>
bool EC<T>::equ(const EC_elem<E>& a, const EC_elem<E>& b) const {
  E p, q;
  base.mul(p, a.x, b.y);
  base.mul(q, a.y, b.x);
  return base.equ(p, q);
}

template <class T>
template <class E>
bool EC<T>::is_infinity(const EC_elem<E>& P) const {
  static E zero {0};
  static E one  {1};
  return base.equ(P.x, zero) && base.equ(P.y, one) && base.equ(P.z, zero);
}

template <class T>
EC_elem<T>& EC_elem<T>::operator=(const EC_elem<T>& other) {
  x = other.x;
  y = other.y;
  z = other.z;
  return (*this);
}

template <class T>
EC_elem<T>& EC_elem<T>::operator=(EC_elem<T>&& other) {
  x = std::move(other.x);
  y = std::move(other.y);
  z = std::move(other.z);
  return (*this);
}

template <class T>
EC_elem<T>* EC_elem<T>::clone(void) const {
  return new EC_elem<T>(*this);
}

template <class T>
std::string EC_elem<T>::to_string(void) const {
  std::stringstream ss;
  ss << "("
     << x.to_string()
     << ", "
     << y.to_string()
     << ", "
     << z.to_string()
     << ")";
  return ss.str();
}
