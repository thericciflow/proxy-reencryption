#pragma once
#include <array>
#include <string>
#include <random>
#include <vector>
#include <sstream>
#include <iostream>

#include <gmp.h>
#include <gmpxx.h>
#include <Python.h>

#include <boost/python.hpp>
#include <boost/foreach.hpp>
#include <boost/python/tuple.hpp>
#include <boost/range/value_type.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

using py_object = boost::python::object;

#include "ZZ.h"
#include "ZZPoint.h"
#include "ec.h"
#include "vector_converter.h"

py_object to_py_object(PyObject *);
ZZ modinv(ZZ _a, ZZ _modulo);
ZZ powermod(ZZ base, ZZ exp, ZZ mod);
ZZ power(ZZ base, ZZ _exp);

std::vector<ZZ> tonelli_shanks(ZZ n, ZZ p);
std::vector<ZZ> modular_square_root(ZZ a, ZZ m);

bool EC_ZZPoint_Is_Infinity(ZZPoint);
bool EC_Equals_Mod(ZZPoint P, ZZPoint Q, ZZ a, ZZ b, ZZ modulo);

#define MPZ_T(t) t.get_mpz_t()
