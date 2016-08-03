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
        base.mul(p, three, a.x); // 3x
        base.mul(p, p, a.x);     // 3x^2
        base.mul(q, c_a, a.z);   // az
        base.mul(q, q, a.z);     // az^2
        base.add(u, p, q);       // 3x^2 + az^2

        base.mul(v, a.y, a.z);   // yz

        base.mul(yv, a.y, v);    // yv

        base.mul(yv4, yv, four); // 4yv

        base.mul(p, u, u);       // u^2
        base.mul(q, two, a.x);   // 2x
        base.mul(q, q, yv4);     // 2x * yv4
        base.sub(w, p, q);       // u^2 - 2x * yv4

        base.mul(p, two, v);     // 2v
        base.mul(Rx, p, w);      // 2vw

        base.mul(p, a.x, yv4);   // x*yv4
        base.sub(p, p, w);       // x*yv4 - w
        base.mul(p, u, p);       // u(x*yv4 - w)
        base.mul(q, eight, yv);  // 8yv
        base.mul(q, q, yv);      // 8yv^2
        base.sub(Ry, p, q);      // u(x*yv4 - w) - 8yv^2

        base.mul(p, eight, v);   // 8v
        base.mul(q, v, v);       // v^2
        base.mul(Rz, p, q);      // 8v^3
      } else {
        E p, q, r, u, v, v2, v3, w;
        base.mul(p, b.y, a.z);   // ByAz
        base.mul(q, a.y, b.z);   // AyBz
        base.sub(u, p, q);       // ByAz - AyBz

        base.mul(p, b.x, a.z);   // BxAz
        base.mul(q, a.x, b.z);   // AxBz
        base.sub(v, p, q);       // BxAz - AxBz

        base.mul(v2, v, v);      // v^2

        base.mul(v3, v2, v);     // v^3

        base.mul(p, u, u);       // u^2
        base.mul(q, a.z, a.z);   // Az^2
        base.mul(p, p, q);       // u^2 Az^2
        base.sub(p, p, v3);      // u^2 Az^2 - v3
        base.mul(q, two, v2);    // 2 v2
        base.mul(r, a.x, b.z);   // AxBz
        base.mul(q, q, r);       // 2 Ax Bz v2
        base.sub(w, p, q);       // u^2 Az^2 - v3 - 2 Ax Bz v2

        base.mul(Rx, v, w);      // vw

        base.mul(p, v2, a.x);    // Ax v2
        base.mul(p, p, b.z);     // Ax Bz v2
        base.sub(p, p, w);       // Ax Bz v2 - w
        base.mul(p, p, u);       // u(Ax Bz v2 - w)
        base.mul(q, v3, a.y);    // Ay v3
        base.mul(q, q, b.z);     // Ay Bz v3
        base.sub(Ry, p, q);      // u(Ax Bz v2 - w) - Ay Bz v3

        base.mul(p, v3, a.z);    // Az v3
        base.mul(Rz, p, b.z);    // Az Bz v3
      }
      ret.x = Rx;
      ret.y = Ry;
      ret.z = Rz;
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
