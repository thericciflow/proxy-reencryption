#pragma once
#include <iostream>

template <class Field>
class EllipticCurvePoint;

template <class Field>
class EllipticCurve {
  private:
    typedef typename Field::Element Element;
  public:
    Field f;
    Element a, b;
    EllipticCurvePoint<Field> *O;

    EllipticCurve(
        Field _f,
        Element _a,
        Element _b) : a(_a), b(_b), f(_f) {
        O = new EllipticCurvePoint<Field>(this, f(0), f(1), f(0));
      }

    template <class T>
    EllipticCurve(
        Field _f,
        T _a,
        T _b) : f(_f), a(f(_a)), b(f(_b)) {
        O = new EllipticCurvePoint<Field>(this, f(0), f(1), f(0));
      }


    constexpr Element determinant() const {
      return -16 * (4 * a * a * a + 27 * b * b);
    }

    constexpr Element j_invariant() const {
      return -1728 * ((4 * a * a * a) / determinant());
    }

    bool is_on_curve(EllipticCurvePoint<Field>& P) {
      auto x = P.x / P.z;
      auto y = P.y / P.z;
      return y*y == x*x*x + a*x + b;
    }

    template <class T>
    EllipticCurvePoint<Field> operator()(T x, T y, T z = 1) {
      return EllipticCurvePoint<Field>(this, f(x), f(y), f(z));
    }

    EllipticCurvePoint<Field> operator()(Element x, Element y, Element z = Element(1)) {
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
    typedef typename Field::Element Element;
  public:
    EllipticCurve<Field> *curve;
    Element x, y, z;

    EllipticCurvePoint(
        EllipticCurve<Field> *_curve,
        Element _x,
        Element _y,
        Element _z = Element(1)) : curve(_curve), x(_x), y(_y), z(_z) {
      if (!curve->is_on_curve(*this) && !is_infinity()) {
        throw "Error: Point is not on Elliptic Curve";
      }
    }

    bool is_infinity() {
      return x == 0 && y == 1 && z == 0;
    }

    bool operator==(EllipticCurvePoint<Field>& rhs) {
      return x * rhs.y == y * rhs.x;
    }

    EllipticCurvePoint<Field> operator+(EllipticCurvePoint<Field>& rhs) {
      auto P = *this;
      auto Q = rhs;
      if (P == Q) {
        auto u = 3 * x * x + curve->a * z * z;
        auto v = y * z;
        auto w = u * u - 8 * x * y * v;
        auto Rx = 2 * v * w;
        auto Ry = u * ( 4 * x * y * v - w) - 8 * y * y * v * v;
        auto Rz = 8 * v * v * v;
        return EllipticCurvePoint<Field>(curve, Rx/Rz, Ry/Rz, curve->f(1));
      } else {
        auto u = Q.y * P.z - P.y * Q.z;
        auto v = Q.x * P.z - P.x * Q.z;
        auto w = u * u * P.z * Q.z - v * v * v - 2 * v * v * P.x * Q.z;
        auto Rx = v * w;
        auto Ry = u * (v * v * P.x * Q.z - w) - v * v * v * P.y * Q.z;
        auto Rz = v * v * v * P.z * Q.z;
        return EllipticCurvePoint<Field>(curve, Rx/Rz, Ry/Rz, curve->f(1));
      }
    }

    friend std::ostream& operator<<(std::ostream& os, const EllipticCurvePoint<Field>& P) {
      os << "Elliptic curve Point : (" << P.x << " : " << P.y << " : " << P.z << ")";
      return os;
    }
};
