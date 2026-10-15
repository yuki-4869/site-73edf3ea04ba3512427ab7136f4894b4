# ACL segtree の各種ラッパー

[説明書の目次](../index.md) · [コード](../../../DataStructureSegment/segtree_wrappers.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/DataStructureSegment/segtree_wrappers.cpp)

ACL segtreeの演算を用途ごとに設定したラッパー集です。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=列長。各クラスのSは下表を参照。非可換な演算も添字の小さい順に集約します。

**共通の前提**

- ACL segtreeが必要。N>=0、0<=l<=r<=N、0<=i<N。演算の結果は各型に収め、極値用番兵を実データとして使いません。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [SegMinMax::cv](#api-2)
- [SegMinMax::set](#api-3)
- [SegArgMax::cv](#api-4)
- [SegArgMax::set](#api-5)
- [SegArgMin::cv](#api-6)
- [SegArgMin::set](#api-7)
- [SegMaxSubarray::cv](#api-8)
- [SegMaxSubarray::set](#api-9)
- [SegBracket::cv](#api-10)
- [SegBracket::set](#api-11)
- [SegBracket::valid](#api-12)
- [用途別の型](#api-13)
- [ACL基底クラスの基本API](#api-14)

</details>

## 型・テンプレート引数

```cpp
template<class S,S(*op)(S,S),S(*e)()>
struct SegBase;
struct SegMinMax;
struct SegArgMax;
struct SegArgMin;
struct SegMaxSubarray;
struct SegAffine;
struct SegBracket;
```

## この実装の注意点

SegMaxSubarrayは空部分列を認めません。SegAffineはf[r-1]∘...∘f[l]の合成を返します。SegArgMin/Maxは同値なら最左位置。int nによる構築では単位元が入るため、0で初期化したい場合はvector<ll>(n)を渡す運用が分かりやすいです。

<a id="api-1"></a>

## コンストラクタ

```cpp
SegMinMax(const vector<ll>&a);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

```cpp
SegArgMax(const vector<ll>&a);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

```cpp
SegArgMin(const vector<ll>&a);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

```cpp
SegMaxSubarray(const vector<ll>&a);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

```cpp
SegBracket(const string&s);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

<a id="api-2"></a>

## SegMinMax::cv

```cpp
static vector<S> cv(const vector<ll>&a);
```

入力列を内部集約型Sの葉の列へ変換します。通常はコンストラクタから呼ばれます。

**計算量**

- O(N)。

<a id="api-3"></a>

## SegMinMax::set

```cpp
void set(int i,ll x);
```

位置iを指定値に上書きします。ラッパーのsetはll/charから内部Sへ変換します。

**制約**

- 0<=i<N

**計算量**

- O(log N)。

<a id="api-4"></a>

## SegArgMax::cv

```cpp
static vector<S> cv(const vector<ll>&a);
```

入力列を内部集約型Sの葉の列へ変換します。通常はコンストラクタから呼ばれます。

**計算量**

- O(N)。

<a id="api-5"></a>

## SegArgMax::set

```cpp
void set(int i,ll x);
```

位置iを指定値に上書きします。ラッパーのsetはll/charから内部Sへ変換します。

**制約**

- 0<=i<N

**計算量**

- O(log N)。

<a id="api-6"></a>

## SegArgMin::cv

```cpp
static vector<S> cv(const vector<ll>&a);
```

入力列を内部集約型Sの葉の列へ変換します。通常はコンストラクタから呼ばれます。

**計算量**

- O(N)。

<a id="api-7"></a>

## SegArgMin::set

```cpp
void set(int i,ll x);
```

位置iを指定値に上書きします。ラッパーのsetはll/charから内部Sへ変換します。

**制約**

- 0<=i<N

**計算量**

- O(log N)。

<a id="api-8"></a>

## SegMaxSubarray::cv

```cpp
static vector<S> cv(const vector<ll>&a);
```

入力列を内部集約型Sの葉の列へ変換します。通常はコンストラクタから呼ばれます。

**計算量**

- O(N)。

<a id="api-9"></a>

## SegMaxSubarray::set

```cpp
void set(int i,ll x);
```

位置iを指定値に上書きします。ラッパーのsetはll/charから内部Sへ変換します。

**制約**

- 0<=i<N

**計算量**

- O(log N)。

<a id="api-10"></a>

## SegBracket::cv

```cpp
static vector<S> cv(const string&s);
```

入力列を内部集約型Sの葉の列へ変換します。通常はコンストラクタから呼ばれます。

**計算量**

- O(N)。

<a id="api-11"></a>

## SegBracket::set

```cpp
void set(int i,char c);
```

位置iを指定値に上書きします。ラッパーのsetはll/charから内部Sへ変換します。

**制約**

- 0<=i<N

**計算量**

- O(log N)。

<a id="api-12"></a>

## SegBracket::valid

```cpp
bool valid(int l,int r);
```

括弧列の[l,r)が正しい括弧列かを返します。

**計算量**

- O(log N)。

<a id="api-13"></a>

## 用途別の型

| クラス | 区間型S | 用途 |
| --- | --- | --- |
| `SegMax` | `ll` | 最大値 |
| `SegMin` | `ll` | 最小値 |
| `SegSum` | `ll` | 和 |
| `SegXOR` | `ll` | XOR |
| `SegOR` | `ll` | OR |
| `SegAND` | `ll` | AND |
| `SegGCD` | `ll` | GCD |
| `SegMinMax` | `S{mx,mn}` | 最大値と最小値 |
| `SegArgMax` | `S{val,id}` | 最大値とその最左位置 |
| `SegArgMin` | `S{val,id}` | 最小値とその最左位置 |
| `SegMaxSubarray` | `S{sum,pref,suff,best}` | 全体和、最大prefix/suffix和、最大部分配列和 |
| `SegAffine` | `S{a,b}` | 一次関数の順序を保つ合成。eval(x)で適用 |
| `SegBracket` | `S{sum,mn}` | 括弧の総和と最小prefix和。valid(l,r)で括弧列判定 |

<a id="api-14"></a>

## ACL基底クラスの基本API

下記はACL基底クラスの宣言の略記です。ラッパーが同名のset/apply等を独自定義している場合、基底のオーバーロードが隠れて直接呼べないことがあります。その場合は、上表の用途別ラッパーと、このページの実際のAPI宣言を使用してください。

```cpp
// Sは上表の区間型。各クラスのメソッドとして使用します。
void set(int p, S x);
S get(int p);
S prod(int l, int r);
S all_prod();
int max_right(int l, auto pred);
int min_left(int r, auto pred);
```

| 操作 | 動作・返り値 | 制約 | 計算量 |
| --- | --- | --- | --- |
| `set(p,x)` | 位置pをxに代入 | `0<=p<N` | `O(log N)` |
| `get(p)` | 位置pのSを取得 | `0<=p<N` | `O(1)` |
| `prod(l,r)` | 左から右の順に区間を集約 | `0<=l<=r<=N` | `O(log N)` |
| `all_prod()` | 全体を集約 | 構築後 | `O(1)` |
| `max_right(l,pred)` | predがtrueとなる最大右端 | `0<=l<=N`、`pred(e())=true`、区間拡張に対して単調 | `O(log N)` |
| `min_left(r,pred)` | predがtrueとなる最小左端 | `0<=r<=N`、`pred(e())=true`、区間拡張に対して単調 | `O(log N)` |

宣言は共通操作を表す略記です。テンプレートによる述語指定など、厳密なACLの宣言は公式資料も参照してください。

[ACL segtreeの公式説明](https://atcoder.github.io/ac-library/production/document_ja/segtree.html)。


## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
SegSum s(vl{1,2,3}); assert(s.prod(0,3)==6);
s.set(1,10); assert(s.prod(0,3)==14);
SegArgMin m(vl{3,1,1}); assert(m.prod(0,3).id==1);
SegBracket b(string("()()")); assert(b.valid(0,4));
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
