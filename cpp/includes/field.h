#pragma once

template<class FieldElement>
class Field {
  public:
    using Element = FieldElement;
    friend Element;

    template <class T>
    FieldElement operator()(const T& x) {
      return FieldElement(this, x);
    }
};

