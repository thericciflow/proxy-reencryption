#pragma once

template<class FieldElement>
class Field {
  public:
    typedef FieldElement Element;
    friend Element;

    template <class T>
    FieldElement operator()(const T& x) {
      return FieldElement(this, x);
    }
};

