#pragma once
#include <iostream>
#include <memory>

template <class Field>
class EllipticCurvePoint;

template <class Field>
class EllipticCurve {
  private:
    using Element = typename Field::Element;
  public:
    Field f;
    const Element a {nullptr};
    const Element b {nullptr};
    EllipticCurvePoint<Field> *O {nullptr};

    EllipticCurve(
        const Field& _f,
        const Element& _a,
        const Element& _b) : a(_a), b(_b), f(_f) {
        O = new EllipticCurvePoint<Field>(this, f(0), f(1), f(0));
      }

    template <class T>
    EllipticCurve(
        const Field& _f,
        const T& _a,
        const T& _b) : f(_f), a(f(_a)), b(f(_b)) {
        O = new EllipticCurvePoint<Field>(this, f(0), f(1), f(0));
    }

    ~EllipticCurve(void) noexcept {
      delete O;
    }

    EllipticCurve<Field>& operator=(EllipticCurve<Field>&& curve) {
      f = std::forward<Field &>(curve.f);
      a = std::forward<const Element&>(curve.a);
      b = std::forward<const Element&>(curve.b);
      return (*this);
    }

    EllipticCurve(EllipticCurve<Field>&& curve)
    : f(std::move<Field &>(curve.f)),
      a(std::move<const Element&>(curve.a)),
      b(std::move<const Element&>(curve.b)),
      O(curve.O)
    {};

    Element determinant() const {
      return -16 * (4 * a * a * a + 27 * b * b);
    }

    Element j_invariant() const {
      return -1728 * ((4 * a * a * a) / determinant());
    }

    bool is_on_curve(const EllipticCurvePoint<Field>& P) const {
      return P.y*P.y*P.z == P.x*P.x*P.x + a*P.x*P.z*P.z + b*P.z*P.z*P.z;
    }

    template <class T>
    EllipticCurvePoint<Field> operator()(const T&& x, const T&& y, const T&& z = 1) {
      return EllipticCurvePoint<Field>(this, f(x), f(y), f(z));
    }

    EllipticCurvePoint<Field> operator()(const Element& x, const Element& y, const Element& z = Element(1)) {
      return EllipticCurvePoint<Field>(this, x, y, z);
    }

    friend std::ostream& operator<<(std::ostream& os, const EllipticCurve<Field>& E) {
      os << "Elliptic Curve : y^2 = x^3";
      if (E.a == 1) {
        os << " + " << "x";
      } else if (E.a != 0) {
        os << " + " << E.a << "x";
      }
      if (E.b != 0) {
        os << " + " << E.b;
      }
      os << " on " << E.f;
      return os;
    }
};

template <class Field>
class EllipticCurvePoint {
  private:
    using Element = typename Field::Element;
  public:
    EllipticCurve<Field> *curve;
    Element x;
    Element y;
    Element z;

    EllipticCurvePoint(
        EllipticCurve<Field> *_curve,
        const Element& _x,
        const Element& _y,
        const Element& _z = Element(1)) : curve(_curve), x(_x), y(_y), z(_z) {
      if (!curve->is_on_curve(*this) && !is_infinity()) {
        throw "Error: Point is not on Elliptic Curve";
      }
    }

    EllipticCurvePoint(const EllipticCurvePoint<Field>&& rhs) : curve(std::move(rhs.curve)), x(std::move(rhs.x)), y(std::move(rhs.y)), z(std::move(rhs.z)) { }
    EllipticCurvePoint(const EllipticCurvePoint<Field>& rhs) : curve(rhs.curve), x(rhs.x), y(rhs.y), z(rhs.z) { }

    EllipticCurvePoint(EllipticCurve<Field> *_curve)
      : curve(_curve), x(curve->f(0)), y(curve->f(1)), z(curve->f(0)) { }

    Element line_coeff(const EllipticCurvePoint<Field>& Q) const {
      if (x * Q.z == Q.x * z) {
        return (3*x*x + curve->a * z * z) / (2 * y * z);
      } else {
        return (Q.y * z - y * Q.z) / (Q.x * z - x * Q.z);
      }
    }

    EllipticCurvePoint<Field> normalize() const {
      if (is_infinity()) {
        return *this;
      }
      auto x = x / z;
      auto y = y / z;
      return EllipticCurvePoint<Field>(curve, std::move(x), std::move(y), curve->f(1));
    }

    bool is_infinity() const {
      return x == 0 && y == 1 && z == 0;
    }

    EllipticCurvePoint<Field>& operator=(const EllipticCurvePoint<Field>&& rhs) {
      curve = std::move(rhs.curve);
      x = std::move(rhs.x);
      y = std::move(rhs.y);
      z = std::move(rhs.z);
      return (*this);
    }

    bool operator==(const EllipticCurvePoint<Field>& rhs) const {
      return x * rhs.y == y * rhs.x;
    }

    bool operator!=(const EllipticCurvePoint<Field>& rhs) const {
      return !(*this == rhs);
    }

    EllipticCurvePoint<Field> operator+(const EllipticCurvePoint<Field>& rhs) const {
      auto P = *this;
      auto Q = rhs;
      if (P.is_infinity()) {
        return Q;
      } else if (Q.is_infinity()) {
        return P;
      } else if (P.x == Q.x && P.y + Q.y == 0) {
        return *(curve->O);
      }
      if (P == Q) {
        auto u = 3 * x * x + curve->a * z * z;
        auto v = y * z;
        auto yv = y * v;
        auto yv4 = 4 * yv;
        auto w = u * u - 2 * x * yv4;
        auto Rx = 2 * v * w;
        auto Ry = u * (x * yv4 - w) - 8 * yv * yv;
        auto Rz = 8 * v * v * v;
        return EllipticCurvePoint<Field>(curve, Rx, Ry, Rz);
      } else {
        auto u = Q.y * z - y * Q.z;
        auto v = Q.x * z - x * Q.z;
        auto v2 = v * v;
        auto v3 = v2 * v;
        auto w = u * u * z * z - v3 - 2 * v2 * x * Q.z;
        auto Rx = v * w;
        auto Ry = u * (v2 * x * Q.z - w) - v3 * y * Q.z;
        auto Rz = v3 * z * Q.z;
        return EllipticCurvePoint<Field>(curve, Rx, Ry, Rz);
      }
    }

    EllipticCurvePoint<Field> operator-() const {
      return EllipticCurvePoint<Field>(curve, x, -y, z);
    }

    EllipticCurvePoint<Field> operator-(EllipticCurvePoint<Field>& rhs) const {
      return -rhs + *this;
    }

    template <class T>
    EllipticCurvePoint<Field> operator*(const T& rhs) const {
      auto P = *this;
      auto m = rhs;
      if (m == 0) {
        return *(curve->O);
      } else if (m == 1) {
        return P;
      } else if (m == 2) {
        return P+P;
      }
      EllipticCurvePoint<Field> Q(curve);
      while (m != 0) {
        if ((m & 1) == 1) {
          Q += P;
        }
        P += P;
        m >>= 1;
      }
      return Q;
    }

    EllipticCurvePoint<Field>& operator+=(const EllipticCurvePoint<Field>& rhs) { 
      *this = *this + rhs;
      return *this;
    }

    template <class T>
    friend inline EllipticCurvePoint<Field> operator*(const T& lhs, EllipticCurvePoint<Field> rhs) {
      return rhs * lhs;
    }

    friend std::ostream& operator<<(std::ostream& os, const EllipticCurvePoint<Field>& P) {
      os << "Elliptic curve Point : (" << P.x << " : " << P.y << " : " << P.z << ")";
      return os;
    }
};
