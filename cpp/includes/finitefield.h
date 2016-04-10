#pragma once

class FiniteField;

class FiniteFieldElement {
  private:
    mpz_class x;
    FiniteField *f;
  public:
    FiniteFieldElement(FiniteField *, mpz_class);
    FiniteFieldElement operator+(const FiniteFieldElement& rhs);
    FiniteFieldElement operator-(const FiniteFieldElement& rhs);
    FiniteFieldElement operator*(const FiniteFieldElement& rhs);
    FiniteFieldElement operator/(const FiniteFieldElement& rhs);

    friend std::ostream& operator<<(std::ostream& os, const FiniteFieldElement& elem);
};

class FiniteField {
  public:
    mpz_class p;

    FiniteField(mpz_class);

    template <class T>
    FiniteFieldElement operator()(T x) {
      return FiniteFieldElement(this, mpz_class(x));
    }

    friend std::ostream& operator<<(std::ostream& os, const FiniteField& field);
};
