#pragma once

class FiniteField;

class FiniteFieldElement {
  private:
    FiniteField *f;
    mpz_class x;
  public:
    FiniteFieldElement(FiniteField *, mpz_class);
    FiniteFieldElement operator+(const FiniteFieldElement& rhs);
    FiniteFieldElement operator-(const FiniteFieldElement& rhs);
    FiniteFieldElement operator*(const FiniteFieldElement& rhs);
    FiniteFieldElement operator/(const FiniteFieldElement& rhs);
    template <class T>
    friend FiniteFieldElement operator+(const FiniteFieldElement& lhs, T rhs) {
      return FiniteFieldElement(lhs.f, lhs.x + rhs);
    }
    template <class T>
    friend FiniteFieldElement operator+(T lhs, const FiniteFieldElement& rhs) {
      return FiniteFieldElement(rhs.f, rhs.x + lhs);
    }
    template <class T>
    friend FiniteFieldElement operator*(const FiniteFieldElement& lhs, T rhs) {
      return FiniteFieldElement(lhs.f, lhs.x * rhs);
    }
    template <class T>
    friend FiniteFieldElement operator*(T lhs, const FiniteFieldElement& rhs) {
      return FiniteFieldElement(rhs.f, rhs.x * lhs);
    }
    template <class T>
    FiniteFieldElement operator-(T rhs);
    template <class T>
    FiniteFieldElement operator*(T rhs);
    template <class T>
    FiniteFieldElement operator/(T rhs);
    template<class T>
    bool operator==(T rhs) {
      return x == rhs;
    }

    bool operator==(const FiniteFieldElement& rhs);

    friend std::ostream& operator<<(std::ostream& os, const FiniteFieldElement& elem);
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
