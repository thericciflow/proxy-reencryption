#pragma once

template<class FieldElement>
class Field {
  public:
    typedef FieldElement Element;
    friend Element;

    template <class T>
    FieldElement operator()(T x) {
      return FieldElement(this, x);
    }
};

