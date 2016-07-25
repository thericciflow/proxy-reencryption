# ecpy
[![Code Climate](https://codeclimate.com/github/elliptic-shiho/ecpy/badges/gpa.svg)](https://codeclimate.com/github/elliptic-shiho/ecpy)
[![Build Status](http://travis-ci.org/elliptic-shiho/ecpy.svg?branch=master)](https://travis-ci.org/elliptic-shiho/ecpy)

Elliptic-Curve Cryptography Library (implemented by Python)

# Features
* Calculation between elliptic curve points
  - Elliptic curve\: `y^2 = x^3 + ax + b` over K, `char(K)` is not 2, 3 where K is field.
  - You can choose K from below:
    + Complex Field, Rational Field, FiniteField and Extended Finite Field(Irreducible polynomial: `x^2 + 1` or `x^2 + x + 1`)
* Weil/Tate Pairing
  - Distortion Map, symmetric-pairing functions
* SSSA-Attack Implementation [Sample](https://gist.github.com/elliptic-shiho/40d42dbab87065e06d6c473ef93e244e) (CTF Challenge Writeup)
* Example Applications
  - `example/id_based_encryption.py`: Implementation of Boneh-Franklin's Identity-based Encryption Scheme
  - `example/boneh_lynn_shacham_short_signature.py`: Implementation of Boneh-Lynn-Shacham's Short Signature Scheme

# Setup

```
> git clone https://github.com/elliptic-shiho/ecpy && cd ecpy
> pip install --upgrade .
```

ecpy does not need other module(s) for install & use. but, If installed gmpy, ecpy uses that.

# Speed
See `bench_pairing.py` .

```
> python bench_pairing.py 
[+] found gmpy! use gmpy.is_prime
Point (25 : 5635866983L + 11271733966Lw : 1) on Elliptic Curve y^2 = x^3 + 1 over ExtendedFiniteField(13208557577, "x^2+x+1")
[+] Weil Pairing: 
weil: 103080.75 usec/pass
weil: 103221.25 usec/pass
weil: 109009.15 usec/pass
weil: 102761.40 usec/pass
weil: 103714.86 usec/pass
weil: 105190.36 usec/pass
weil: 108847.40 usec/pass
weil: 105479.80 usec/pass
weil: 103277.70 usec/pass
weil: 103277.05 usec/pass
[+] Tate Pairing: 
tate: 24560.50 usec/pass
tate: 25118.54 usec/pass
tate: 24779.80 usec/pass
tate: 24652.50 usec/pass
tate: 24600.45 usec/pass
tate: 25006.35 usec/pass
tate: 24741.76 usec/pass
tate: 24734.06 usec/pass
tate: 24610.10 usec/pass
tate: 24736.60 usec/pass
================================================================
weil: 104785.97 usec/pass
tate: 24754.07 usec/pass
```

# Special Thanks
Cybozu Labs Youth (5th, 6th) - http://labs.cybozu.co.jp/youth.html

# License
this software released under MIT License. Please see /LICENSE.

