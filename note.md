notes
========
FF, EF, ECを作る

## 基礎
基本は次の通り

```cpp
struct エレメント {
  基礎型 v;
  エレメント(const 基礎型&);
  エレメント(const エレメント&);
  エレメント *clone();
  std::string to_string();
};

struct ベース {
  パラメータ群;
  ベース(パラメータ) : パラメータ(パラメータ) {}
  ベース(ベース) : パラメータ(ベース.パラメータ) {}
  エレメント *add(const エレメント *x, const エレメント *y);
  エレメント *sub(const エレメント *x, const エレメント *y);
  エレメント *mul(const エレメント *x, const エレメント *y);
  エレメント *div(const エレメント *x, const エレメント *y);
  エレメント *pow(const エレメント *x, const エレメント *y);
  std::string to_string();
  ベース *clone();
};
```

## `FF/FF_elem` (ほぼおわり, `cpp/src/FF.cpp`)
* ベース = `FF`
* エレメント = `FF_elem`
* パラメータ = `mpz_class p`
* 基礎型 = `mpz_class`

## `EF/EF_elem`
対応する既約多項式\: `x^2+1`, `x^2+x+1`
それぞれEnumで以下のようにする

```cpp
enum struct IrreduciblePolynomial : int {
  X2_1,
  X2_X_1
};
```

`template <class T, class E>` でいくらか別に対応できるように

* ベース = `EF<T, E>`
* エレメント = `EF_elem<E>`
* パラメータ = `IrreduciblePolynomial poly, T *base`
* 基礎型 = `E`

```cpp
template <class T>
struct EF_elem {
  T *u, *v;
  EF_elem(const T&, const T&);
  EF_elem(const EF_elem&);
  EF_elem *clone();
  std::string to_string();
};

template <class T, class E>
struct EF {
  T *base;
  IrreduciblePolynomial poly;
  EF(const T *base, IrreduciblePolynomial poly) : base(base), poly(poly) {}
  EF(const EF& ef) : base(ef.base), poly(ef.poly) {}
  EF_elem<E> *add(const EF_elem<E> *x, const FF_elem<E> *y);
  EF_elem<E> *sub(const EF_elem<E> *x, const FF_elem<E> *y);
  EF_elem<E> *mul(const EF_elem<E> *x, const FF_elem<E> *y);
  EF_elem<E> *div(const EF_elem<E> *x, const FF_elem<E> *y);
  EF_elem<E> *pow(const EF_elem<E> *x, const FF_elem<E> *y);
  std::string to_string();
  EF *clone();
};
```

## `EC/EC_elem`

`EC_elem` はいわゆる点

* ベース = `EC<T, E>`
* エレメント = `EC_elem<E>`
* パラメータ = `T *base`
* 基礎型 = `E`

```cpp
struct EC_elem {
  T *x, *y, *z;
  EC_elem(const T*, const T*, const T*);
  EC_elem(const EC_elem&);
  EC_elem *clone();
  std::string to_string();
};

template <class T, class E>
struct EC {
  mpz_class a, b;
  T *base;
  EC(const mpz_class& a, const mpz_class& b, const T *base) : a(a), b(b), base(base) {}
  EC(const EC& ec) : a(ec.a), b(ec.b), base(ec.base) {}
  EC_elem<E> *add(const EC_elem<E> *x, const EC_elem<E> *y);
  EC_elem<E> *sub(const EC_elem<E> *x, const EC_elem<E> *y);
  EC_elem<E> *mul(const EC_elem<E> *x, const EC_elem<E> *y);
  EC_elem<E> *div(const EC_elem<E> *x, const EC_elem<E> *y);
  EC_elem<E> *pow(const EC_elem<E> *x, const EC_elem<E> *y);
  std::string to_string();
  EC *clone();
};
```













