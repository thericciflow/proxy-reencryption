#pragma once
#include <array>
#include <string>
#include <random>
#include <vector>
#include <sstream>
#include <numeric>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include <gmp.h>
#include <gmpxx.h>

#define __EXPORT__ extern "C"

#include "FF.h"

#include "EF.h"

template <class T>
void write_to_python_string(const T *x, char *ptr, int len) {
  std::stringstream ss;
  ss << x->to_string();
  std::string r = ss.str();
  if (r.size() < len) {
    strcpy(ptr, r.c_str());
  }
}
