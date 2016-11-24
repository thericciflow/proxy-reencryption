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
Thu Nov 24 14:33:55 JST 2016 ~/prog/lab/mathematics/ecpy 100%
> time python scripts/bench_pairing.py
[+] found gmpy! use gmpy.is_prime
[+] Found native module!
Point (25 : 7572690594L + 1936823611Lw : 1) on Elliptic Curve y^2 = x^3 + 1 over ExtendedFiniteField(13208557577, "x^2+x+1")
[+] Weil Pairing: 
weil: 16189.55 usec/pass
weil: 15987.54 usec/pass
weil: 16135.55 usec/pass
weil: 15801.89 usec/pass
weil: 15559.90 usec/pass
weil: 16082.70 usec/pass
weil: 15545.90 usec/pass
weil: 16006.70 usec/pass
weil: 16379.70 usec/pass
weil: 15683.25 usec/pass
[+] Tate Pairing: 
tate: 3366.21 usec/pass
tate: 3534.65 usec/pass
tate: 3481.95 usec/pass
tate: 3605.70 usec/pass
tate: 4865.16 usec/pass
tate: 3525.25 usec/pass
tate: 3454.05 usec/pass
tate: 3374.05 usec/pass
tate: 3407.90 usec/pass
tate: 3514.30 usec/pass
================================================================
weil: 15937.27 usec/pass
tate: 3612.92 usec/pass

real    0m4.708s
user    0m4.608s
sys     0m0.024s
```

# Special Thanks
Cybozu Labs Youth (5th, 6th) - http://labs.cybozu.co.jp/youth.html

# License
this software released under MIT License. Please see /LICENSE.

