ecpy notes
========

hackmd: https://hackmd.io/JwZgJsAMDswGYFoCsBDAHAYwQFhfBARtpJAtAGxJwBMVY1AjJEkA#

# C++クラス設計

ここでは「構造クラス」「値クラス」の2種類を設計する。

## 構造クラス

* FF
* EF
* EC

→ それぞれ値クラスに対する操作や共通保持すべきデータを保持するクラスで、実際の値については全て値クラスが保持するようにする。

## 値クラス

* FF\_elem
* EF\_elem
* EC\_elem

→ それぞれは値の保持を主目的とするクラスで、各種パラメータや演算処理は構造クラスによる。

## 共通に持つメンバ関数

ここでは構造/値クラス `T` が必ず持つべきメンバ関数を定義する。

```cpp
struct T {
  T *clone(void) const;
  std::string to_string(void) const;
};
```

## 構造クラスの持つメンバ関数

ここでは構造クラス `T` と対応する値クラス `E` が必ず持つべきメンバ関数を定義する。

```cpp
template <class E>
struct T {
  void add(E& ret, const E& a, const E& b) const;
  void sub(E& ret, const E& a, const E& b) const;
  void mul(E& ret, const E& a, const E& b) const;
  void div(E& ret, const E& a, const E& b) const;
  void pow(E& ret, const E& a, const E& b) const;
};
```

## 特殊メンバ関数について

各クラスの特殊メンバ関数は

* [C\+\+11 時代のクラス設計に関する提案 - 野良C\+\+erの雑記帳](http://d.hatena.ne.jp/gintenlabo/20130604/1370362451)
* [本の虫: C\+\+03とC\+\+11の違い: 特別なメンバー関数編](https://cpplover.blogspot.jp/2013/12/c03c11_13.html)
* [Rule of Five](https://en.wikipedia.org/wiki/Rule_of_three_%28C%2B%2B_programming%29#Rule_of_Five)
* Modern Effective C++
  * 項目14 例外を発生させない関数はnoexceptと宣言する
  * 項目17 自動的に生成される特殊メンバ関数を理解する
  * 項目25 右辺値参照にはstd::moveを、ユニヴァーサル参照にはstd::forwardを用いる

辺りを参考に考える。
これらの関数では例外は一切投げない。

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

Python側でのラッパクラスの設計。

## 構造クラスのラッパクラスの設計

基本となるクラスは以下のようにする。 構造クラスを `T` 、値クラスを `E` とする。

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
    return __to_string(1024)

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
    lib.T_pow(s.ptr, ret.ptr, a.ptr, b.ptr)

  def __del__(s): # GC deleter
    lib.T_delete(s.ptr)
```

## 値クラスのラッパクラスの設計

基本となるクラスは以下のようにする。 構造クラスを `T` 、値クラスを `E` とする。

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
    return __to_string(1024)

  def __del__(s): # GC deleter
    lib.E_delete(s.ptr)

  def to_python(s): # Pythonのオブジェクトに変換する。FFなら数値、EFならタプル等で、これは明確に規程する必要がある。
    return int(str(s))
```

# Python<=>C++インターフェース

基本的には不透明ポインタをハンドルのように扱うことで実現する。

## 多倍長整数について

Python側での多倍長整数(C APIでのPyLong型)はC++では原則mpz\_classへ変換する。やりとりは文字列変換の後行う。

Python\:
```python
# lib.cpp_func(1<<256) # <= PyLong型になるので直接渡せない
lib.cpp_func(str(1<<256)) # <= OK
```

C++\:
```cpp
// __EXPORT__ void cpp_func(const mpz_class& x); <= 直接の変換は不可能
__EXPORT__ void cpp_func(const char *x); // <= OK, 内部でmpz_classへ変換する
```

## 文字列を戻り値とする関数について

文字列を戻り値に持つ関数(`to_string`)についてはPython ctypes APIの`create_string_buffer`メソッドを用いてバッファを先に生成し、コピーしてもらう形を取る。

C++側は `ecpy_native.h` にある次の関数を用いることで、この文書のクラス設計に基づくクラスについては対応できる。

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

Python側は以下のようになる。この時、 `T_to_string` 関数はクラス`T`の `to_string` 関数のラッパ。

```python
  def __str__(s):
    b = create_string_buffer(1024)
    lib.T_to_string(s.ptr, b, 1024)
    return b.value
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
  void T_pow(const T *obj, E *ret, const E *a, const E *b);
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
  FF *clone(void) const;
  std::string to_string(void) const;

  // structure class member functions
  void add(FF_elem& ret, const FF_elem& a, const FF_elem& b) const;
  void sub(FF_elem& ret, const FF_elem& a, const FF_elem& b) const;
  void mul(FF_elem& ret, const FF_elem& a, const FF_elem& b) const;
  void div(FF_elem& ret, const FF_elem& a, const FF_elem& b) const;
  void pow(FF_elem& ret, const FF_elem& a, const FF_elem& b) const;
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
  FF_elem *clone(void) const;
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
  void FF_pow(const FF *obj, FF_elem *ret, const FF_elem *a, const FF_elem *b);
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
  FF base;
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
  void pow(EF_elem& ret, const EF_elem& a, const EF_elem& b) const;

  // common functions
  EF *clone(void) const;
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
  EF_elem *clone(void) const;
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
  void EF_pow(const EF *obj, EF_elem *ret, const EF_elem *a, const EF_elem *b);

  void EF_to_string(const EF *obj, char *ptr, int len);
};

// EF_elem
__EXPORT__ {
  EF_elem *EF_elem_create(const char *u, const char *v);
  void EF_elem_delete(const EF_elem *obj);
  void EF_elem_to_string(const EF_elem *obj, char *ptr, int len);
};
```

