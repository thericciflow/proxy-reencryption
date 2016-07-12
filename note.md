notes
========

優先度順

* Native Moduleのサポート範囲を考える
  - Native Moduleを書く(C++ w/Boost)
  - 一部で良いのか、全体を書き直すか
  - 楕円曲線クラスをまともに使えるかつ速度の出るように書く
* ~~~ 旧式クラス定義を新式へ変更~~~ -> 完了(40d70b28c0486a9739842ef69ab486f98674e0c0)
* リファクタリング
* ドキュメント整備

```
ecpy.util
modinvのpydoc

ecpy.abstract.AdditiveGroup
特になし

ecpy.abstract.Field
AdditiveFieldとかぶっている部分がある→統合?

ecpy.algorithm.pairing
miller関数が射影座標に対応していない
MapToPoint, gen_supersingular_ec辺りは別にしたほうがいい?
symmetric_****_pairing関数は残して良いと思う

ecpy.algorithm.root
/**modular_square_rootのpydocが機能していない(コメントは先頭じゃないとダメっぽい)
tonelli_shanks関数は統合してもよさげ
modular_square_root_extendが書きかけのままだったのでこれを全部書くべき
後151行目のraise NotImplementedError()は絶対に呼ばれないので気づいたら修正しておく

ecpy.algorithm.sssa_attack
特になし

ecpy.structure.ComplexField
Fieldクラスの継承全体に言えるが、未だに旧式クラス定義のままの部分が大半なので書きなおしていかないとダメ

ecpy.structure.EllipticCurve
早めにNative Moduleを導入したい。
GenericEllipticCurve._add関数が射影座標で計算してるのに最後に1/z計算してる
GenericEembedding_degree関数でいいなあ

ecpy.structure.ExtendedFiniteField
```
