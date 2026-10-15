# Segment Tree Beats

[説明書の目次](../index.md) · [コード](../../../DataStructureSegment/segment_tree_beats.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/DataStructureSegment/segment_tree_beats.cpp)

列の区間chmin/chmax、加算、代入と、和・極値・極値の位置を管理します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=配列長、INF=1LL<<60。第2極値は異なる値の第2位です。

**共通の前提**

- 区間は0<=l<=r<=N、位置は0<=i<N。clampはlo<=hi。値は番兵±INF（1<<60）と区別できる範囲に置き、和や加算をllに収めます。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [SegBeats::init](#api-2)
- [SegBeats::size](#api-3)
- [SegBeats::empty](#api-4)
- [SegBeats::chmin](#api-5)
- [SegBeats::chmax](#api-6)
- [SegBeats::add](#api-7)
- [SegBeats::assign](#api-8)
- [SegBeats::clamp](#api-9)
- [SegBeats::set](#api-10)
- [SegBeats::prod](#api-11)
- [SegBeats::all_prod](#api-12)
- [SegBeats::get](#api-13)
- [SegBeats::sum](#api-14)
- [SegBeats::min](#api-15)
- [SegBeats::max](#api-16)
- [SegBeats::second_min](#api-17)
- [SegBeats::second_max](#api-18)
- [SegBeats::count_min](#api-19)
- [SegBeats::count_max](#api-20)
- [SegBeats::argmin](#api-21)
- [SegBeats::argmax](#api-22)
- [SegBeats::first_ge](#api-23)
- [SegBeats::last_ge](#api-24)
- [SegBeats::first_le](#api-25)
- [SegBeats::last_le](#api-26)
- [SegBeats::all_sum](#api-27)
- [SegBeats::all_min](#api-28)
- [SegBeats::all_max](#api-29)

</details>

## 型・テンプレート引数

```cpp
struct SegBeats;
```

## この実装の注意点

単一のchmin/chmaxが常にO(log N)とは限りません。一般に操作列に対する償却評価です。空区間の極値は番兵になるので、通常は非空区間で極値・位置を取得します。

<a id="api-1"></a>

## コンストラクタ

```cpp
SegBeats();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
SegBeats(int n);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

```cpp
SegBeats(const vector<ll>&a);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

<a id="api-2"></a>

## SegBeats::init

```cpp
void init(const vector<ll>&a);
```

配列aから木を再構築します。

**計算量**

- O(N)。

<a id="api-3"></a>

## SegBeats::size

```cpp
int size()const;
```

管理している要素数を返します。

**計算量**

- O(1)。

<a id="api-4"></a>

## SegBeats::empty

```cpp
bool empty()const;
```

管理要素が0ならtrue。

**計算量**

- O(1)。

<a id="api-5"></a>

## SegBeats::chmin

```cpp
void chmin(int l,int r,ll x);
```

[l,r)をmin(A[i],x)に更新

**計算量**

- 償却O(log^2N)。

<a id="api-6"></a>

## SegBeats::chmax

```cpp
void chmax(int l,int r,ll x);
```

[l,r)をmax(A[i],x)に更新

**計算量**

- 償却O(log^2N)。

<a id="api-7"></a>

## SegBeats::add

```cpp
void add(int l,int r,ll x);
```

[l,r)にx加算

**計算量**

- O(logN)。

<a id="api-8"></a>

## SegBeats::assign

```cpp
void assign(int l,int r,ll x);
```

[l,r)をxに代入

**計算量**

- O(logN)。

<a id="api-9"></a>

## SegBeats::clamp

```cpp
void clamp(int l,int r,ll lo,ll hi);
```

[l,r)を[lo,hi]に丸める

**計算量**

- 償却O(log^2N)。

<a id="api-10"></a>

## SegBeats::set

```cpp
void set(int i,ll x);
```

A[i]=xに変更

**計算量**

- O(logN)。

<a id="api-11"></a>

## SegBeats::prod

```cpp
Node prod(int l,int r);
```

[l,r)の全情報を返す

**計算量**

- O(logN)。

<a id="api-12"></a>

## SegBeats::all_prod

```cpp
Node all_prod()const;
```

全体の全情報を返す

**計算量**

- O(1)。

<a id="api-13"></a>

## SegBeats::get

```cpp
ll get(int i);
```

A[i]を返す

**計算量**

- O(logN)。

<a id="api-14"></a>

## SegBeats::sum

```cpp
ll sum(int l,int r);
```

[l,r)の和を返す

**計算量**

- O(logN)。

<a id="api-15"></a>

## SegBeats::min

```cpp
ll min(int l,int r);
```

[l,r)の最小値を返す

**計算量**

- O(logN)。

<a id="api-16"></a>

## SegBeats::max

```cpp
ll max(int l,int r);
```

[l,r)の最大値を返す

**計算量**

- O(logN)。

<a id="api-17"></a>

## SegBeats::second_min

```cpp
ll second_min(int l,int r);
```

第2最小値、無ければINF

**計算量**

- O(logN)。

<a id="api-18"></a>

## SegBeats::second_max

```cpp
ll second_max(int l,int r);
```

第2最大値、無ければ-INF

**計算量**

- O(logN)。

<a id="api-19"></a>

## SegBeats::count_min

```cpp
int count_min(int l,int r);
```

最小値の個数を返す

**計算量**

- O(logN)。

<a id="api-20"></a>

## SegBeats::count_max

```cpp
int count_max(int l,int r);
```

最大値の個数を返す

**計算量**

- O(logN)。

<a id="api-21"></a>

## SegBeats::argmin

```cpp
pair<ll,int> argmin(int l,int r);
```

区間の {最小値,その最左位置} を返します。

**制約**

- 非空区間

**計算量**

- O(log N)。

<a id="api-22"></a>

## SegBeats::argmax

```cpp
pair<ll,int> argmax(int l,int r);
```

区間の {最大値,その最左位置} を返します。

**制約**

- 非空区間

**計算量**

- O(log N)。

<a id="api-23"></a>

## SegBeats::first_ge

```cpp
int first_ge(int l,int r,ll x);
```

最左のA[i]>=xを返す、無ければ-1

**計算量**

- O(logN)。

<a id="api-24"></a>

## SegBeats::last_ge

```cpp
int last_ge(int l,int r,ll x);
```

最右のA[i]>=xを返す、無ければ-1

**計算量**

- O(logN)。

<a id="api-25"></a>

## SegBeats::first_le

```cpp
int first_le(int l,int r,ll x);
```

最左のA[i]<=xを返す、無ければ-1

**計算量**

- O(logN)。

<a id="api-26"></a>

## SegBeats::last_le

```cpp
int last_le(int l,int r,ll x);
```

最右のA[i]<=xを返す、無ければ-1

**計算量**

- O(logN)。

<a id="api-27"></a>

## SegBeats::all_sum

```cpp
ll all_sum()const;
```

全体の和を返す

**計算量**

- O(1)。

<a id="api-28"></a>

## SegBeats::all_min

```cpp
ll all_min()const;
```

全体の最小値を返す

**計算量**

- O(1)。

<a id="api-29"></a>

## SegBeats::all_max

```cpp
ll all_max()const;
```

全体の最大値を返す

**計算量**

- O(1)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
SegBeats s(vl{1,5,3}); s.chmin(0,3,4); s.add(0,2,2);
assert(s.sum(0,3)==12 && s.max(0,3)==6);
auto [x,i]=s.argmin(0,3); assert(x==3 && i==0);
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
