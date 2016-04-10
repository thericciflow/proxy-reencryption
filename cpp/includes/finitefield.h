#pragma once

class FiniteField;

class FiniteFieldElement {
  private:
    FiniteField *f;
    mpz_class x;
  public:
    FiniteFieldElement(FiniteField *, mpz_class);
    FiniteFieldElement operator+(const FiniteFieldElement& rhs) const;
    FiniteFieldElement operator-(const FiniteFieldElement& rhs) const;
    FiniteFieldElement operator*(const FiniteFieldElement& rhs) const;
    FiniteFieldElement operator/(const FiniteFieldElement& rhs) const;
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

    template <class T>
    friend FiniteFieldElement operator+(const FiniteFieldElement& lhs, T rhs);
    template <class T>
    friend FiniteFieldElement operator-(const FiniteFieldElement& lhs, T rhs);
    template <class T>
    friend FiniteFieldElement operator*(const FiniteFieldElement& lhs, T rhs);
    template <class T>
    friend FiniteFieldElement operator/(const FiniteFieldElement& lhs, T rhs);

    template <class T>
    friend FiniteFieldElement operator+(T lhs, const FiniteFieldElement& rhs);
    template <class T>
    friend FiniteFieldElement operator-(T lhs, const FiniteFieldElement& rhs);
    template <class T>
    friend FiniteFieldElement operator*(T lhs, const FiniteFieldElement& rhs);
    template <class T>
    friend FiniteFieldElement operator/(T lhs, const FiniteFieldElement& rhs);
};

class FiniteField : public Field<FiniteFieldElement> {
  public:
    mpz_class p;

    FiniteField(mpz_class);

    template <class T>
    Element operator()(T x) {
      return Element(this, mpz_class(x));
    }

    friend std::ostream& operator<<(std::ostream& os, const FiniteField& field);
};
template <class T>
FiniteFieldElement operator+(const FiniteFieldElement& lhs, T rhs) {
  return FiniteFieldElement(lhs.f, lhs.x + rhs);
}

template <class T>
FiniteFieldElement operator+(T lhs, const FiniteFieldElement& rhs) {
  return FiniteFieldElement(rhs.f, rhs.x + lhs);
}

template <class T>
FiniteFieldElement operator-(const FiniteFieldElement& lhs, T rhs) {
  return FiniteFieldElement(lhs.f, lhs.x - rhs);
}

template <class T>
FiniteFieldElement operator-(T lhs, const FiniteFieldElement& rhs) {
  return FiniteFieldElement(rhs.f, lhs - rhs.x);
}

template <class T>
FiniteFieldElement operator*(const FiniteFieldElement& lhs, T rhs) {
  return FiniteFieldElement(lhs.f, lhs.x * rhs);
}

template <class T>
FiniteFieldElement operator*(T lhs, const FiniteFieldElement& rhs) {
  return FiniteFieldElement(rhs.f, rhs.x * lhs);
}

template <class T>
FiniteFieldElement operator/(const FiniteFieldElement& lhs, T rhs) {
  mpz_class t;
  mpz_invert(MPZ_T(t), MPZ_T(rhs), MPZ_T(lhs.f->p));
  return FiniteFieldElement(lhs.f, lhs.x * t);
}

template <class T>
FiniteFieldElement operator/(T lhs, const FiniteFieldElement& rhs) {
  mpz_class t;
  mpz_invert(MPZ_T(t), MPZ_T(rhs.x), MPZ_T(rhs.f->p));
  return FiniteFieldElement(rhs.f, lhs * t);
}
