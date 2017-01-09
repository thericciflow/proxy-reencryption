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
Mon Jan  9 23:45:19 JST 2017 ~/prog/lab/mathematics/ecpy 100%
> time python scripts/bench_pairing.py
[+] found gmpy! use gmpy.is_prime
[+] Found native module!
Point (25 : 7572690594L + 1936823611Lw : 1) on Elliptic Curve y^2 = x^3 + 1 over ExtendedFiniteField(13208557577, "x^2+x+1")
[+] Weil Pairing: 
weil: 15067.84 usec/pass
weil: 15350.75 usec/pass
weil: 14928.81 usec/pass
weil: 16086.85 usec/pass
weil: 15138.26 usec/pass
weil: 15738.64 usec/pass
weil: 15705.55 usec/pass
weil: 15250.10 usec/pass
weil: 14912.95 usec/pass
weil: 14979.51 usec/pass
[+] Tate Pairing: 
tate: 3155.10 usec/pass
tate: 3121.40 usec/pass
tate: 3106.36 usec/pass
tate: 3146.70 usec/pass
tate: 3116.00 usec/pass
tate: 3123.04 usec/pass
tate: 3126.85 usec/pass
tate: 3201.40 usec/pass
tate: 3134.64 usec/pass
tate: 3135.75 usec/pass
================================================================
weil: 15315.92 usec/pass
tate: 3136.72 usec/pass

real    0m4.410s
user    0m4.376s
sys     0m0.008s

```

# Special Thanks
Cybozu Labs Youth (5th, 6th) - http://labs.cybozu.co.jp/youth.html

# License
this software released under MIT License. Please see /LICENSE.

