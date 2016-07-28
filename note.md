ecpy notes
========
# クラス設計
ここでは「構造クラス」「値クラス」の2種類を設計する。

## 構造クラス

* FF
* EF
* EC

→ それぞれ値クラスに対する操作や共通保持すべきデータを保持するクラスで、実際の値については全て値クラスが保持するようにする。

## 値クラス

* FF_elem
* EF_elem
* EC_elem

→ それぞれは値の保持を主目的とするクラスで、各種パラメータや演算処理は構造クラスによる。

## 共通に持つメンバ関数
ここでは構造/値クラス `T` が必ず持つべきメンバ関数を定義する。

```clike=
struct T {
  T *clone(void) const;
  std::string to_string(void) const;
};
```

## 構造クラスの持つメンバ関数
ここでは構造クラス `T` と対応する値クラス `E` が必ず持つべきメンバ関数を定義する。

```clike=
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

```cpp=
struct T {
  T();
  ~T();
  T(const T&);
  T(T&&);
  T& operator=(const T&);
  T& operator=(T&&);
};
```

# Python<=>C++インターフェース
基本的には不透明ポインタをハンドルのように扱うことで実現する。
数値については全て文字列処理で妥協。
e.g.

```python
F = FF_create("7")
x = FF_elem_create(F, "1") # Fは不透明ポインタ FF *
y = FF_elem_create(F, "6") # 同上
print FF_elem_to_string(x) # => 1
print FF_elem_to_string(F) # => F_7

t = FF_elem_create(F)
FF_add(t, x, y) # t = x + y
print FF_elem_to_string(x) # => 1+6 mod 7 = 0
FF_elem_delete(t)
FF_elem_delete(x)
FF_elem_delete(y)
FF_delete(F)
```

実際にはクラスを作って__del__内にdelete系を実装するのでこの扱いは更に簡単には出来る。

# FF/FF_elem
## FF
```clike=
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

## FF_elem
```clike=
struct FF_elem {
  mpz_class v;
  
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