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

# Setup

```
> sudo pip install --upgrade .
```

ecpy does not need other module(s) for install & use. but, If installed gmpy, ecpy uses that.
If you want to install Native Module, see [cpp/README.md](cpp/README.md).

# License
this software released under MIT License. Please see /LICENSE.

# Speed
See `bench_pairing.py` .

```
Mon Jul 11 15:28:39 JST 2016 ~/prog/lab/mathematics/ecpy 100%
> python bench_pairing.py 
[+] found gmpy! use gmpy.is_prime
[+] Native Module Enabled.
Point (25 : 5635866983L + 11271733966Lw : 1) on Elliptic Curve y^2 = x^3 + 1 over ExtendedFiniteField(13208557577, "x^2+x+1")
[+] Weil Pairing: 
weil: 57110.20 usec/pass
weil: 56711.49 usec/pass
weil: 56519.84 usec/pass
weil: 56879.90 usec/pass
weil: 56478.80 usec/pass
weil: 56457.00 usec/pass
weil: 56551.49 usec/pass
weil: 56521.75 usec/pass
weil: 56802.75 usec/pass
weil: 56370.65 usec/pass
[+] Tate Pairing: 
tate: 13734.56 usec/pass
tate: 13636.45 usec/pass
tate: 13704.19 usec/pass
tate: 13656.04 usec/pass
tate: 13635.49 usec/pass
tate: 13655.15 usec/pass
tate: 13785.70 usec/pass
tate: 13688.85 usec/pass
tate: 13643.40 usec/pass
tate: 13741.49 usec/pass
================================================================
weil: 56640.39 usec/pass
tate: 13688.13 usec/pass
Mon Jul 11 15:28:54 JST 2016 ~/prog/lab/mathematics/ecpy 100%
> 
```

# Special Thanks
Cybozu Labs Youth (5th, 6th) - http://labs.cybozu.co.jp/youth.html
