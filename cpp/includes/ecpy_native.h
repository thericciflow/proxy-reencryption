#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <sstream>

#include <gmp.h>
#include <gmpxx.h>
#include <Python.h>

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using py_object = boost::python::object;

#include "ZZ.h"
#include "ZZPoint.h"
#include "ec.h"

py_object to_py_object(PyObject *);
ZZ modinv(ZZ _a, ZZ _modulo);

bool EC_ZZPoint_Is_Infinity(ZZPoint);
bool EC_Equals_Mod(ZZPoint P, ZZPoint Q, ZZ a, ZZ b, ZZ modulo);
