ecpy notes
========

hackmd: https://hackmd.io/JwZgJsAMDswGYFoCsBDAHAYwQFhfBARtpJAtAGxJwBMVY1AjJEkA#

# C++クラス設計

C++側で書くクラスの設計について

ある対象を構造と値にそれぞれ分離して考える。有限体なら有限体の演算・素数を構造、実際の整数値は値として扱う。

## 構造クラス

* FF
* EF
* EC

→ 値クラスに対する操作(演算)や共通保持すべきデータを保持するクラス。

## 値クラス

* FF\_elem
* EF\_elem
* EC\_elem

→ 値の保持を主目的とするクラス。

## 構造・値クラスが共通に持つメンバ関数

構造/値クラス `T` が必ず持つべきメンバ関数を定義する。

```cpp
struct T {
  T* clone(void) const;
  std::string to_string(void) const;
};
```

## 構造クラスの持つメンバ関数

構造クラス `T` が必ず持つべきメンバ関数を定義する。 `E` は対応する値クラス。

```cpp
template <class E>
struct T {
  void add(E& ret, const E& a, const E& b) const;
  void sub(E& ret, const E& a, const E& b) const;
  void mul(E& ret, const E& a, const E& b) const;
  void div(E& ret, const E& a, const E& b) const;
  void pow(E& ret, const E& a, const mpz_class& b) const;
  bool equals(const E& a, const E& b) const;
};
```

## 特殊メンバ関数について

**これらの関数では例外は一切投げない**。

```cpp
struct T {
  T();
  ~T();
  T(const T&);
  T(T&&);
  T& operator=(const T&);
  T& operator=(T&&);
};
```

# Pythonクラス設計

Python側でのC++ラッパクラスの設計。

以下、基本的な事柄として `lib` はecpy\_nativeライブラリのctypes.CDLLインスタンスで、 `ctypes` は `from ctypes import *` でインポートされているとする。

## 構造クラスのラッパクラスの設計

基本となるクラスは以下のようにする。 構造クラスを `T` 、対応する値クラスを `E` とする。

```python
class T(object):
  def __init__(s, params): # paramsは適宜パラメータを入れる。複数でも構わない。
    s.ptr = lib.T_create(params)

  def __to_string(s, bufsize): # to_stringのラッパ, バッファのサイズは可変
    b = create_string_buffer(bufsize)
    lib.T_to_string(s.ptr, b, bufsize)
    b = b.value
	if len(b) == 0: # not enough buffer size
	  return s.__to_string(2*bufsize)
	return b

  def __str__(s):
    return s.__to_string(1024)

  def add(s, ret, a, b):
    assert isinstance(ret, E) and isinstance(a, E) and isinstance(b, E)
    lib.T_add(s.ptr, ret.ptr, a.ptr, b.ptr)

  def sub(s, ret, a, b):
    assert isinstance(ret, E) and isinstance(a, E) and isinstance(b, E)
    lib.T_sub(s.ptr, ret.ptr, a.ptr, b.ptr)

  def mul(s, ret, a, b):
    assert isinstance(ret, E) and isinstance(a, E) and isinstance(b, E)
    lib.T_mul(s.ptr, ret.ptr, a.ptr, b.ptr)

  def div(s, ret, a, b):
    assert isinstance(ret, E) and isinstance(a, E) and isinstance(b, E)
    lib.T_div(s.ptr, ret.ptr, a.ptr, b.ptr)

  def pow(s, ret, a, b):
    assert isinstance(ret, E) and isinstance(a, E)
    lib.T_pow(s.ptr, ret.ptr, a.ptr, str(b))

  def equ(s, a, b):
    assert isinstance(a, E) and isinstance(b, E)
	return lib.T_equ(s.ptr, a.ptr, b.ptr) != 0

  def __del__(s): # GC deleter
    lib.T_delete(s.ptr)
```

## 値クラスのラッパクラスの設計

構造クラスと大方同じだが、こちらは値の保持が目的なのでそちらを優先する。

```python
class E(object):
  def __init__(s, params): # paramsは適宜パラメータを入れる。複数でも構わない。
    s.ptr = lib.E_create(params)

  def __to_string(s, bufsize): # to_stringのラッパ, バッファのサイズは可変
    b = create_string_buffer(bufsize)
    lib.E_to_string(s.ptr, b, bufsize)
    b = b.value
	if len(b) == 0: # not enough buffer size
	  return s.__to_string(2*bufsize)
	return b

  def __str__(s):
    return s.__to_string(1024)

  def __del__(s): # GC deleter
    lib.E_delete(s.ptr)

  def to_python(s):
    # Pythonのオブジェクトに変換する。FFなら数値、EFならタプル等で、これは明確に規定する必要がある。
```

# Python<=>C++インターフェース

基本的には不透明ポインタをハンドルのように扱うことで実現する。

## 多倍長整数について

Python側での多倍長整数(C APIでのPyLong型)をC\+\+へ渡す際は文字列として渡した後C\+\+側でmpz\_classへ変換する。

Python\:
```python
# lib.cpp_func(1<<256) # <= long型なので直接渡せない
lib.cpp_func(str(1<<256)) # <= OK
```

C++\:
```cpp
// __EXPORT__ void cpp_func(const mpz_class& x); <= 直接の変換は不可能
__EXPORT__ void cpp_func(const char *x); // <= OK, 内部でmpz_classへ変換される
```

## 文字列を戻り値とする関数について

文字列を戻り値に持つ関数(現在の設計では`to_string`)についてはPython ctypes APIの`ctypes.create_string_buffer`を用いて文字列バッファを用意、そこにコピーするようにする

C++側では次の関数 (`ecpy_native.h` に用意されている)を用いてバッファに文字列をコピーする。

```cpp
template <class T>
void write_to_python_string(const T *x, char *ptr, int len) {
  std::stringstream ss;
  ss << x->to_string();
  std::string r = ss.str();
  if (r.size() < len) {
    strcpy(ptr, r.c_str());
  }
}
```

Python側は上のクラス設計の際のものと同じ。

```python
  def __to_string(s, bufsize): # to_stringのラッパ, バッファのサイズは可変
    b = create_string_buffer(bufsize)
    lib.E_to_string(s.ptr, b, bufsize)
    b = b.value
	if len(b) == 0: # not enough buffer size
	  return s.__to_string(2*bufsize)
	return b

  def __str__(s):
    return s.__to_string(1024)
```

## 構造クラスのインターフェース関数

構造クラスを `T` とし、値クラスを `E` とする

```cpp
__EXPORT__ {
  // create T instance
  T *T_create(〜);
  // delete T instance
  void T_delete(const T*);
  // ret = a + b
  void T_add(const T *obj, E *ret, const E *a, const E *b);
  // ret = a - b
  void T_sub(const T *obj, E *ret, const E *a, const E *b);
  // ret = a * b
  void T_mul(const T *obj, E *ret, const E *a, const E *b);
  // ret = a / b
  void T_div(const T *obj, E *ret, const E *a, const E *b);
  // ret = a ^ b
  void T_pow(const T *obj, E *ret, const E *a, const char *b);
  // a == b
  int T_equ(const T *obj, const E *a, const E *b);
  // to python __str__ function
  void T_to_string(const T *obj, char *ptr, int len);
};
```

## 値クラスのインターフェース関数

構造クラスを `T` とし、値クラスを `E` とする

```cpp
__EXPORT__ {
  // create E instance
  E *E_create(〜);
  // delete E instance
  void E_delete(const E*);
  // to python __str__ function
  void E_to_string(const E *obj, char *ptr, int len);
};
```


# FF/FF\_elem
## FF

```cpp
struct FF {
  mpz_class p;

  FF() = default;
  ~FF() = default;
  FF(const mpz_class& p) : p(p) {}
  FF(const FF& t) : p(t.p) {}
  FF(FF&& t) : p(std::move(t.p)) {};

  FF& operator=(const FF&);
  FF& operator=(FF&&);

  // common functions
  FF* clone(void) const;
  std::string to_string(void) const;

  // structure class member functions
  void add(FF_elem& ret, const FF_elem& a, const FF_elem& b) const;
  void sub(FF_elem& ret, const FF_elem& a, const FF_elem& b) const;
  void mul(FF_elem& ret, const FF_elem& a, const FF_elem& b) const;
  void div(FF_elem& ret, const FF_elem& a, const FF_elem& b) const;
  void pow(FF_elem& ret, const FF_elem& a, const mpz_class& b) const;
  bool equ(const FF_elem& a, const FF_elem& b) const;
};
```

## FF\_elem

```cpp
struct FF_elem {
  mpz_class v;

  FF_elem(const mpz_class& v) : v(v) {};

  FF_elem() = default;
  ~FF_elem() = default;
  FF_elem(const FF_elem& t) : v(t.v) {};
  FF_elem(FF_elem&& t) : v(std::move(t.v)) {};

  FF_elem& operator=(const FF_elem&);
  FF_elem& operator=(FF_elem&&);

  // common functions
  FF_elem* clone(void) const;
  std::string to_string(void) const;
};
```

## FF/FF\_elem のPythonインターフェース

```cpp
struct FF;
struct FF_elem;
// FF
__EXPORT__ {
  // create FF instance
  FF *FF_create(const char *p);
  // delete FF instance
  void FF_delete(const FF*);
  // ret = a + b
  void FF_add(const FF *obj, FF_elem *ret, const FF_elem *a, const FF_elem *b);
  // ret = a - b
  void FF_sub(const FF *obj, FF_elem *ret, const FF_elem *a, const FF_elem *b);
  // ret = a * b
  void FF_mul(const FF *obj, FF_elem *ret, const FF_elem *a, const FF_elem *b);
  // ret = a / b
  void FF_div(const FF *obj, FF_elem *ret, const FF_elem *a, const FF_elem *b);
  // ret = a ^ b
  void FF_pow(const FF *obj, FF_elem *ret, const FF_elem *a, const char *b);
  // a == b
  int FF_equ(const FF *obj, const FF_elem *a, const FF_elem *b);
  // to python __str__ function
  void FF_to_string(const FF *obj, char *ptr, int len);
};

// FF_elem
__EXPORT__ {
  // create FF_elem instance
  FF_elem *FF_elem_create(const char *v);
  // delete FF_elem instance
  void FF_elem_delete(const FF_elem*);
  // to python __str__ function
  void FF_elem_to_string(const FF_elem *obj, char *ptr, int len);
};

```

## to\_pythonの返り値: `FF_elem`

`int(str(s))`

つまり保持している数値をそのまま返す。

# EF/EF\_elem

既約多項式が2種類($x^2+1$, $x^2+x+1$)あるので、これはenumにしておく

[enumeration declaration - cppreference.com](http://en.cppreference.com/w/cpp/language/enum)
> the keywords `class` and `struct` are exactly equivalent

```cpp
enum class IrreduciblePolynomialType : int {
  X2_1, // x^2+1
  X2_X_1, // x^2+x+1
};
```
## EF

```cpp
struct EF {
  const FF& base;
  IrreduciblePolynomialType poly;

  EF(const FF& ff, IrreduciblePolynomialType pol) : base(ff), poly(pol) {}

  EF(const mpz_class& p, IrreduciblePolynomialType pol) : base(p), poly(pol) {}


  EF() = default;
  ~EF() = default;
  EF(const EF& ef) : base(ef.base), poly(ef.poly) {}
  EF(EF&& ef) : base(std::move(ef.base)), poly(ef.poly) {}

  EF& operator=(const EF& ef);
  EF& operator=(EF&& ef);

  // structure class member functions
  void add(EF_elem& ret, const EF_elem& a, const EF_elem& b) const;
  void sub(EF_elem& ret, const EF_elem& a, const EF_elem& b) const;
  void mul(EF_elem& ret, const EF_elem& a, const EF_elem& b) const;
  void div(EF_elem& ret, const EF_elem& a, const EF_elem& b) const;
  void pow(EF_elem& ret, const EF_elem& a, const mpz_class& b) const;
  bool equ(const EF_elem& a, const EF_elem& b) const;

  // common functions
  EF* clone(void) const;
  std::string to_string(void) const;
};
```

## EF\_elem

```cpp
struct EF_elem {
  FF_elem u, v;

  EF_elem(const FF_elem& u, const FF_elem& v) : u(u), v(v) {}

  EF_elem(const mpz_class& u, const mpz_class& v) : u(u), v(v) {}


  EF_elem() = default;
  ~EF_elem() = default;
  EF_elem(const EF_elem& ee) : u(ee.u), v(ee.v) {};
  EF_elem(EF_elem&& ee) : u(std::move(ee.u)), v(std::move(ee.v)) {};

  EF_elem& operator=(const EF_elem& ee);
  EF_elem& operator=(EF_elem&& ee);

  // common functions
  EF_elem* clone(void) const;
  std::string to_string(void) const;
};
```

## EF/EF\_elemのPythonインターフェース

```cpp
struct EF;
struct EF_elem;

// EF
__EXPORT__ {
  // create EF instance
  // polynomial is string of irreducible polynomial. 
  // e.g. x^2+x+1, x^2+1, X^2+1, x^2+ x +1 (ignore spaces and case insensitive)
  EF *EF_create(const char *p, const char *polynomial);
  // delete EF instance
  void EF_delete(const EF *ef);

  // r = a + b
  void EF_add(const EF *obj, EF_elem *ret, const EF_elem *a, const EF_elem *b);
  // r = a - b
  void EF_sub(const EF *obj, EF_elem *ret, const EF_elem *a, const EF_elem *b);
  // r = a * b
  void EF_mul(const EF *obj, EF_elem *ret, const EF_elem *a, const EF_elem *b);
  // r = a / b
  void EF_div(const EF *obj, EF_elem *ret, const EF_elem *a, const EF_elem *b);
  // r = a ^ b
  void EF_pow(const EF *obj, EF_elem *ret, const EF_elem *a, const char *b);
  // a == b
  int EF_equ(const EF *obj, const EF_elem *a, const EF_elem *b);
  void EF_to_string(const EF *obj, char *ptr, int len);
};

// EF_elem
__EXPORT__ {
  EF_elem *EF_elem_create(const char *u, const char *v);
  void EF_elem_delete(const EF_elem *obj);
  void EF_elem_to_string(const EF_elem *obj, char *ptr, int len);
};
```

## to\_pythonの返り値: `EF_elem`

`ast.literal_eval(str(s).lstrip("EF_elem")))`

二次拡大体なので2つの要素が返らなければならない。そのため返り値はタプルで内容は要素を `a+b*v` (vは基底) とした時 `(a, b)`。

# EC/EC\_elem

楕円曲線クラス

べき乗・除算は数式上ありえないので除外する。除外するにはdelete代入を利用する。

テンプレート型引数の `T` は `FF/EF` 等構造クラスを表し、 `E` は対応する値クラスを表す。

## EC

```cpp
template <class T>
struct EC {
  const T& base;
  mpz_class a, b;

  EC(const T& base, const mpz_class& a, const mpz_class& b) : base(base), a(a), b(b) {}

  EC() = default;
  ~EC() = default;
  EC(const EC<T>& ec) : base(ec.base), a(ec.a), b(ec.b) {};
  EC(EC<T>&& ec) : base(std::move(ec.base)), a(std::move(ec.a)), b(std::move(ec.b)) {};
  EC<T>& operator=(const EC<T>& ec);
  EC<T>& operator=(EC<T>&& ec);

  template <class E>
  void add(EC_elem<E>& ret, const EC_elem<E>& a, const EC_elem<E>& b) const;
  template <class E>
  void sub(EC_elem<E>& ret, const EC_elem<E>& a, const EC_elem<E>& b) const;
  template <class E>
  void mul(EC_elem<E>& ret, const EC_elem<E>& a, const EC_elem<E>& b) const;
  template <class E>
  bool equ(const EC_elem& a, const EC_elem<E>& b) const;

  // ----------------- UNDEFINED(DELETED) -----------------
  template <class E>
  void div(EC_elem<E>& ret, const EC_elem<E>& a, const EC_elem<E>& b) const = delete;
  template <class E>
  void pow(EC_elem<E>& ret, const EC_elem<E>& a, const mpz_class& b) const = delete;
  // ------------------------------------------------------

  template <class E>
  EC_elem<E> to_affine(const EC_elem<E>& elem) const;
  template <class E>
  E line_coeff(const EC_elem<E>& P, const EC_elem<E>& Q) const;
  template <class E>
  bool is_on_curve(const EC_elem<E>& elem) const;
  template <class E>
  bool is_equal(const EC_elem<E>& rhs, const EC_elem<E>& lhs) const;
  template <class E>
  bool is_infinity(const EC_elem<E>& P) const;

  EC<T>* clone(void) const;
  std::string to_string(void) const;
};
```

追加した関数については以下の通り:

### to\_affine
あるEC\_elemについて通常の射影座標からアフィン座標(xy座標)に変換して返す(z座標を1にして実質アフィン座標になるようにする)

### line\_coeff
楕円曲線の点P、点Qを通る直線 $ax + b$ の係数 $a$ を求める。もし $P=Q$ だったならば接線の係数を返す。

### is\_on\_curve
ある点Pが楕円曲線上の点かどうかを判定する。

### is\_equal
点Pと点Qが射影座標を考慮して同一であるかを判定する。

### is\_infinity
ある点Pが無限遠点かどうかを判定する。

## EC\_elem

```cpp
template <class T>
struct EC_elem {
  T x, y, z;

  EC_elem(const mpz_class& x, const mpz_class& y, const mpz_class& z) : x(x), y(y), z(z) {}

  EC_elem() = default;
  ~EC_elem() = default;
  EC_elem(const EC_elem<T>& ee) : x(ee.x), y(ee.y), z(ee.z) {};
  EC_elem(EC_elem<T>&& ee) : x(std::move(ee.x)), y(std::move(ee.y)), z(std::move(ee.z)) {};
  EC_elem<T>& operator=(const EC_elem<T>&);
  EC_elem<T>& operator=(EC_elem<T>&&);

  EC_elem<T>* clone(void) const;
  std::string to_string(void) const;
};
```

# References
* [C\+\+11 時代のクラス設計に関する提案 - 野良C\+\+erの雑記帳](http://d.hatena.ne.jp/gintenlabo/20130604/1370362451)
* [本の虫: C\+\+03とC\+\+11の違い: 特別なメンバー関数編](https://cpplover.blogspot.jp/2013/12/c03c11_13.html)
* [Rule of Three (C++ programming)#Rule of Five](https://en.wikipedia.org/wiki/Rule_of_three_%28C%2B%2B_programming%29#Rule_of_Five)
* Modern Effective C++
  * 項目11 privateな未定義関数よりもdeleteを優先する
  * 項目14 例外を発生させない関数はnoexceptと宣言する
  * 項目17 自動的に生成される特殊メンバ関数を理解する
  * 項目25 右辺値参照にはstd::moveを、ユニヴァーサル参照にはstd::forwardを用いる











