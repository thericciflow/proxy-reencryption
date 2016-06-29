# ecpy
[![Code Climate](https://codeclimate.com/github/elliptic-shiho/ecpy/badges/gpa.svg)](https://codeclimate.com/github/elliptic-shiho/ecpy)
[![Build Status](https://travis-ci.org/elliptic-shiho/ecpy.svg?branch=master)](https://travis-ci.org/elliptic-shiho/ecpy)

Elliptic-Curve Cryptography Library (implemented by Python)

# Features
* Elliptic Curve Calculation
  - In Complex Field, Rational Field, FiniteField and Extended Finite Field(irreducible polynomial: `x^2 + 1` or `x^2 + x + 1`)
* Weil/Tate Pairing
  - Distortion Map, symmetric-pairing functions
* SSSA-Attack Implementation
* Example Applications
  - `example/id_based_encryption.py`: Implementation of Boneh-Franklin's Identity-based Encryption
  - `example/boneh_lynn_shacham_short_signature.py`: Implementation of Boneh-Lynn-Shacham's Short Signature

# License
this software released under MIT License. Please see /LICENSE.

# Speed
See `bench_pairing.py` .

```
Sun Jun 19 14:41:46 JST 2016 ~/prog/lab/mathematics/ecpy 100%
> python bench_pairing.py 
[+] found gmpy! use gmpy.is_prime
Point (25 : 7572690594L + 1936823611Lw : 1) on Elliptic Curve y^2 = x^3 + 1 over ExtendedFiniteField(13208557577, "x^2+x+1")
[+] Weil Pairing: 
weil: 72908.25 usec/pass
weil: 72549.59 usec/pass
weil: 73382.09 usec/pass
weil: 72033.70 usec/pass
weil: 72523.50 usec/pass
weil: 73150.35 usec/pass
weil: 71082.10 usec/pass
weil: 71405.10 usec/pass
weil: 71139.05 usec/pass
weil: 71303.40 usec/pass
[+] Tate Pairing: 
tate: 16398.80 usec/pass
tate: 16166.14 usec/pass
tate: 16186.20 usec/pass
tate: 16099.99 usec/pass
tate: 16463.35 usec/pass
tate: 16662.90 usec/pass
tate: 16177.30 usec/pass
tate: 16113.84 usec/pass
tate: 16220.25 usec/pass
tate: 16178.00 usec/pass
================================================================
weil: 72147.71 usec/pass
tate: 16266.68 usec/pass
```

# Special Thanks
Cybozu Labs Youth (5th, 6th) - http://labs.cybozu.co.jp/youth.html
