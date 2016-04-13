#pragma once
#include "field.h"

class FiniteField;

class FiniteFieldElement {
  private:
    FiniteField *f;
  public:
    mpz_class x;
    friend FiniteField;

    FiniteFieldElement() : f(nullptr), x(-1) {}
    FiniteFieldElement(FiniteField *, mpz_class);

    /*
    FiniteFieldElement operator+(const FiniteFieldElement& rhs) const;
    FiniteFieldElement operator-(const FiniteFieldElement& rhs) const;
    FiniteFieldElement operator*(const FiniteFieldElement& rhs) const;
    FiniteFieldElement operator/(const FiniteFieldElement& rhs) const;
    */
    template <class T>
    FiniteFieldElement operator+(const T& rhs) const;
    template <class T>
    FiniteFieldElement operator-(const T& rhs) const;
    template <class T>
    FiniteFieldElement operator*(const T& rhs) const;
    template <class T>
    FiniteFieldElement operator/(const T& rhs) const;
    template <class T>
    FiniteFieldElement operator^(const T& rhs) const;

    template <class T>
    friend FiniteFieldElement operator+(const T& lhs, const FiniteFieldElement& rhs);
    template <class T>
    friend FiniteFieldElement operator-(const T& lhs, const FiniteFieldElement& rhs);
    template <class T>
    friend FiniteFieldElement operator*(const T& lhs, const FiniteFieldElement& rhs);
    template <class T>
    friend FiniteFieldElement operator/(const T& lhs, const FiniteFieldElement& rhs);

    template<class T>
    bool operator==(T rhs) const {
      return x == rhs;
    }

    template<class T>
    bool operator!=(T rhs) const {
      return !((*this) == rhs);
    }

    bool operator==(const FiniteFieldElement& rhs) const;

    friend std::ostream& operator<<(std::ostream& os, const FiniteFieldElement& elem);

    FiniteFieldElement operator-() {
      FiniteFieldElement z(f, -x);
      return z;
    }

    operator mpz_class() const {
      return x;
    }
};

template <class T>
inline mpz_class to_mpz_cls(const T& t) {
  return t;
}

template <>
inline mpz_class to_mpz_cls<FiniteFieldElement>(const FiniteFieldElement& t) {
  return t.x;
}

class FiniteField : public Field<FiniteFieldElement> {
  public:
    mpz_class p;

    FiniteField(mpz_class);

    template <class T>
    Element operator()(T x) {
      return Element(this, mpz_class(x));
    }

    friend std::ostream& operator<<(std::ostream& os, const FiniteField& field);

    template <class T, class U>
    void add(Element& ret, const T& b, const U& c) {
      ret.f = this;
      ret.x = (to_mpz_cls(b) + to_mpz_cls(c)) % p;
    }

    template <class T, class U>
    void sub(Element& ret, const T& b, const U& c) {
      ret.f = this;
      ret.x = (to_mpz_cls(b) + (p - to_mpz_cls(c))) % p;
    }

    template <class T, class U>
    void mul(Element& ret, const T& b, const U& c) {
      ret.f = this;
      ret.x = (to_mpz_cls(b) * to_mpz_cls(c)) % p;
    }

    template <class T, class U>
    void div(Element& ret, const T& b, const U& c) {
      mpz_class t;
      ret.f = this;
      mpz_invert(MPZ_T(t), MPZ_T(to_mpz_cls(c)), MPZ_T(p));
      t *= to_mpz_cls(b);
      ret.x = t % p;
    }
};
template <class T>
FiniteFieldElement FiniteFieldElement::operator+(const T& rhs) const {
  FiniteFieldElement z;
  f->add(z, *this, rhs);
  return z;
}

template <class T>
FiniteFieldElement FiniteFieldElement::operator-(const T& rhs) const {
  FiniteFieldElement z;
  f->sub(z, *this, rhs);
  return z;
}

template <class T>
FiniteFieldElement FiniteFieldElement::operator*(const T& rhs) const {
  FiniteFieldElement z;
  f->mul(z, *this, rhs);
  return z;
}

template <class T>
FiniteFieldElement FiniteFieldElement::operator/(const T& rhs) const {
  FiniteFieldElement z;
  f->div(z, *this, rhs);
  return z;
}

template <class T>
FiniteFieldElement operator+(const T& lhs, const FiniteFieldElement& rhs) {
  FiniteFieldElement z;
  rhs.f->add(z, lhs, rhs);
  return z;
}

template <class T>
FiniteFieldElement operator-(const T& lhs, const FiniteFieldElement& rhs) {
  FiniteFieldElement z;
  rhs.f->sub(z, lhs, rhs);
  return z;
}

template <class T>
FiniteFieldElement operator*(const T& lhs, const FiniteFieldElement& rhs) {
  FiniteFieldElement z;
  rhs.f->mul(z, lhs, rhs);
  return z;
}

template <class T>
FiniteFieldElement operator/(const T& lhs, const FiniteFieldElement& rhs) {
  FiniteFieldElement z;
  rhs.f->div(z, lhs, rhs);
  return z;
}
template <class T>
FiniteFieldElement FiniteFieldElement::operator^(const T& rhs) const {
  FiniteFieldElement res(f, 0);
  mpz_powm(MPZ_T(res.x), MPZ_T(x), MPZ_T(to_mpz_cls(rhs)), MPZ_T(f->p));
  return res;
}
