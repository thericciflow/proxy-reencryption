#pragma once
#include "ecpy_native.h"

struct ZZPoint {
  ZZ x, y, z;
  template <class T>
  ZZPoint(T&& _x, T&& _y, T&& _z) :
    x(_x), y(_y), z(_z) {}
  template <class T>
  ZZPoint(T&& _x, T&& _y) :
    x(_x), y(_y), z(1) {}

  std::string to_string() const;
  std::string to_raw_string() const;
};

void initializeZZPointConverter();

