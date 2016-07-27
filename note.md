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
  void add(const エレメント& z, const エレメント& x, const エレメント& y);
  void sub(const エレメント& z, const エレメント& x, const エレメント& y);
  void mul(const エレメント& z, const エレメント& x, const エレメント& y);
  void div(const エレメント& z, const エレメント& x, const エレメント& y);
  void pow(const エレメント& z, const エレメント& x, const エレメント& y);
  std::string to_string();
  ベース *clone();
};
```

