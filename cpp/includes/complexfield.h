#pragma once
#include "field.h"

class ComplexField;

class ComplexFieldElement {
  private:
    ComplexField *f;
  public:
    mpz_class x, y;
    friend ComplexField;

    ComplexFieldElement() : f(nullptr), x(0), y(0) {}
    ComplexFieldElement(ComplexFieldElement&& rhs) : f(std::move(rhs.f)), x(std::move(rhs.x)) {}
    ComplexFieldElement(const ComplexFieldElement& rhs) : f(rhs.f), x(rhs.x), y(rhs.y) {}
    ~ComplexFieldElement() = default;
    ComplexFieldElement(ComplexField *, mpz_class, mpz_class);

    template <class T>
    ComplexFieldElement operator+(const T& rhs) const;
    template <class T>
    ComplexFieldElement operator-(const T& rhs) const;
    template <class T>
    ComplexFieldElement operator*(const T& rhs) const;
    template <class T>
    ComplexFieldElement operator/(const T& rhs) const;
    template <class T>
    ComplexFieldElement operator^(const T& rhs) const;

    ComplexFieldElement& operator=(const ComplexFieldElement&& rhs);

    ComplexFieldElement operator-() const;

    mpz_class get_mpz_class() const;

    template <class T>
    bool operator==(const T& rhs) const;

    template <class T>
    friend ComplexFieldElement operator+(const T& lhs, const ComplexFieldElement& rhs);
    template <class T>
    friend ComplexFieldElement operator-(const T& lhs, const ComplexFieldElement& rhs);
    template <class T>
    friend ComplexFieldElement operator*(const T& lhs, const ComplexFieldElement& rhs);
    template <class T>
    friend ComplexFieldElement operator/(const T& lhs, const ComplexFieldElement& rhs);

    friend std::ostream& operator<<(std::ostream& os, const ComplexFieldElement& x);
};

class ComplexField : public Field<ComplexFieldElement> {
  public:
    template <class T>
    Element& operator()(const T& x, const T& y) {
      return Element(this, to_mpz_cls(x), to_mpz_cls(y));
    }

    template <class T>
    Element& operator()(const T& x) {
      return Element(this, to_mpz_cls(x), 0);
    }

    friend std::ostream& operator<<(std::ostream& os, const ComplexField& field);
};

extern ComplexField _cf_instance;

#define CC _cf_instance

template <>
inline mpz_class to_mpz_cls<ComplexFieldElement>(const ComplexFieldElement& t) = delete;
