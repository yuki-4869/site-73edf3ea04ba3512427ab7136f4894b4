# Segment Tree Beats の拡張

[説明書の目次](../index.md) · [コード](../../../DataStructureSegment/segment_tree_beats_extra.cpp) · [確認対象の改訂版](https://github.com/yuki-4869/Algorithm/blob/f9177dec185cbc540c81fa7c94c944c1e87348ac/DataStructureSegment/segment_tree_beats_extra.cpp)

非負整数列に対する剰余・floor除算・整数平方根・AND/ORを、それぞれ専用の木で扱います。
2026-10-15改訂。min/maxの名前解決を修正し、境界検査・空の木・コピー/ムーブを整えました。除算・平方根では、一括処理できる節点を増やしています。

**記号**

- N: 配列長。0-indexed、半開区間[l,r)。
- Type: SegMod / SegDiv / SegSqrt / SegBitwiseBeatsのいずれか。
- 各クラスは独立した木です。同じ初期配列から作っても、更新は共有しません。

## 型の選択

| 型 | 専用の更新 | 取得情報 |
| --- | --- | --- |
| SegMod | mod | 和・最小・最大・区間長 |
| SegDiv | floor除算・区間代入 | 和・最小・最大・区間長 |
| SegSqrt | floor平方根 | 和・最小・最大・区間長 |
| SegBitwiseBeats | bit AND / OR | 上記と区間のbit OR / AND |

基本テンプレートの `<bits/stdc++.h>` で標準ヘッダーを読み込みます。型は実装内で明示し、ACLには依存しません。

**制約**

- 0<=i<N、0<=l<=r<=N、0<=N<=INT_MAX/4。
- 値とマスクは0以上のlong long。modの法とdivideの除数は正。
- 各更新後の全体の和と中間演算がlong longに収まること。算術オーバーフローは一律に検出しません。
- とくにpoint set・区間assign・bit ORで値を増やす場合、全体の和の上限も確認してください。

範囲外、負の値・負の長さ、0以下の法/除数、長さの上限超過はメッセージなしで停止します。
NDEBUGでも検査します。空区間への更新でも、法・除数・値の妥当性は検査します。

## コンストラクタ・コピー・ムーブ

```cpp
Type seg;
explicit Type(int n);
Type(const std::vector<long long>& values);
Type(const Type& other);
Type(Type&& other);
Type& operator=(const Type& other);
Type& operator=(Type&& other);
```

既定構築は長さ0、nによる構築は0をn個並べます。コピーは独立し、ムーブ元は長さ0になります。

**計算量**

- 既定構築O(1)、n・配列からの構築O(N)。
- コピー構築O(N_other)、コピー代入O(N+N_other)。
- ムーブ構築O(1)、ムーブ代入O(N+1)。破棄O(N)。

## init / size / empty / clear

```cpp
void init(const std::vector<long long>& values);
int size() const;
bool empty() const;
void clear();
```

initは配列で作り直します。sizeは配列長、emptyは長さ0かを返します。
clearは全ノードを破棄して長さ0にします。vectorの確保容量が残る場合があります。

**計算量**

- init / clearはO(N)、size / emptyはO(1)。

## set / get / prod / sum / min / max / all_sum

```cpp
void set(int i, long long x);
long long get(int i);
Node prod(int l, int r);
long long sum(int l, int r);
long long min(int l, int r);
long long max(int l, int r);
long long all_sum() const;
```

setは点を置換します。getは要素の値、prodは区間の集約情報を返します。
Nodeで利用する共通フィールドはsum・mn・mx・lenです。Bitwise型ではbor・bandも、それぞれ区間のbit OR・ANDとして取得できます。
Nodeのsetv・has・lazy_*は内部管理用で、取得結果の情報としては利用しないでください。

**計算量**

- set / get / prod / sum / min / maxはO(log(N+1))。all_sumはO(1)。

## SegMod::mod

```cpp
void mod(int l, int r, long long x);
```

A[i]%=xを適用します。節点の最大値がx未満なら、その節点は走査しません。

**制約**

- x>0。

**計算量**

- 単発の最悪O(N)。変更される値が縮むことを利用して、操作列全体の走査を減らします。

## SegDiv::divide / assign

```cpp
void divide(int l, int r, long long d);
void assign(int l, int r, long long x);
```

divideはA[i]=floor(A[i]/d)、assignはA[i]=xを適用します。
d=1は走査せず終了します。区間全体の最小値・最大値を除算した結果が等しければ、その節点全体を一括代入します。

**制約**

- d>0、x>=0。

**計算量**

- divideは単発の最悪O(N)、d=1ならO(1)。assignはO(log(N+1))。

## SegSqrt::sqrt

```cpp
void sqrt(int l, int r);
```

A[i]=floor(sqrt(A[i]))を適用します。0/1だけの節点は走査しません。
最小値・最大値が同じ整数平方根に写る節点は、遅延代入で一括処理します。
平方根は浮動小数点の近似後に128bit整数で補正し、long longの最大値付近でも整数の床を求めます。

**計算量**

- 単発の最悪O(N)。値の縮小や一括代入で走査を省きます。

## SegBitwiseBeats::bit_and / bit_or

```cpp
void bit_and(int l, int r, long long mask);
void bit_or(int l, int r, long long mask);
```

それぞれA[i]&=mask、A[i]|=maskを適用します。
区間内で対象ビットが一様な場合は、和・最小・最大の変化をまとめて処理します。
値・マスクは下位63bitの非負long longを使用できます。ALLはLLONG_MAXです。

**計算量**

- 単発の最悪O(N)。ビットが揃う節点を一括処理します。

## 計算量の読み方

上記の非線形更新を、単発O(log N)の操作として扱わないでください。
mod / divide / sqrtでは値の減少を利用できますが、set / assignで値を増やすと、その後に再び大きな値を走査する可能性があります。
AND / ORも交互に適用するとビットが再び変化し得ます。あらゆる混在操作列に一律の対数償却計算量を保証する記述ではありません。

## 空区間・空の木

| 取得情報 | prod(l,l) |
| --- | --- |
| sum / len | 0 |
| mn / mx | LLONG_MAX / -LLONG_MAX |
| Bitwiseのbor / band | 0 / LLONG_MAX |

長さ0やムーブ元にもprod(0,0)、sum(0,0)、all_sumと空区間更新を使用できます。点アクセスは範囲外です。

## 使用例

ソースをグローバルに置き、cassertをincludeしてmain内で実行します。

```cpp
SegMod mod(std::vector<long long>{8,3,10});
mod.mod(0,3,6);
assert(mod.sum(0,3)==9);

SegDiv div(std::vector<long long>{8,9,10,11});
div.divide(0,4,4);                 // 結果が同じになる節点を一括処理
div.set(1,20);
assert(div.all_sum()==26 && div.sum(0,2)==22);

SegSqrt root(std::vector<long long>{9,10,14,15});
root.sqrt(0,4); root.set(2,100);   // 一括更新の後の点更新も可能
assert(root.sum(1,4)==106);
root.sqrt(1,4); assert(root.all_sum()==15);

SegBitwiseBeats bits(std::vector<long long>{(1LL<<62)|3});
bits.bit_and(0,1,1LL<<62); bits.bit_or(0,1,7);
assert(bits.get(0)==((1LL<<62)|7));
assert(bits.prod(0,0).len==0);
```

## 旧版からの移行

| 旧版 | 改訂版 |
| --- | --- |
| 補助min/maxを手動でstd::修飾 | ファイル全体をそのまま使用可能 |
| ll・vector・sqrtlの個人設定に依存 | 型を実装で明示し、標準ヘッダーは基本テンプレートに集約 |
| n / stに直接アクセス | size / prod / get等を使用。内部配列と長さは非公開 |
| assertの妥当性検査 | NDEBUGでもメッセージなしで停止 |
| Bitwiseの0<=値,mask<2^61 | 0以上のlong long（下位63bit）へ拡張。和の上限条件は別途必要 |
| Bitwiseの空区間min/maxが0 | 他の型と同じLLONG_MAX / -LLONG_MAXに統一 |
| INF=2^60 | 空区間用のINFはLLONG_MAXへ変更 |

SegSqrtのNodeには遅延管理用のフィールドを追加しています。集約情報は名前付きのsum・mn・mx・lenで取得してください。

## 確認結果

GNU C++23 / GCC 13.3.0で、標準ヘッダだけを使う専用テストと基本テンプレートを併用した掲載例を確認しました。
空状態・型の端・不正入力・コピー・ムーブ、同一結果の一括更新後の部分取得・点更新を検証しています。
32シード×1024ステップで4種類を通常配列と比較し、和・最小・最大・区間長・bit OR/AND・点取得を確認しました。
遅延セグ木を含む専用テスト全体では163,840グループステップです。NDEBUG、AddressSanitizer、UBSan、デバッグイテレータでも成功しています。実行環境の制限でLeakSanitizerは無効です。

[専用テスト](../../../tests/segment_wrappers_test.cpp) · [C++23の確認結果](../validation.md)

```sh
g++ -std=c++23 -O2 -I/path/to/ac-library tests/segment_wrappers_test.cpp -o /tmp/segment_wrappers_test
/tmp/segment_wrappers_test
```

通常の保持メモリはO(N)。テストはすべての値・更新列・算術オーバーフロー条件の保証ではありません。
