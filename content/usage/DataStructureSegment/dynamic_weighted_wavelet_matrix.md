# 動的重み付きWavelet Matrix

[説明書の目次](../index.md) · [コード](../../../DataStructureSegment/dynamic_weighted_wavelet_matrix.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/DataStructureSegment/dynamic_weighted_wavelet_matrix.cpp)

各位置のキーと重みを保持し、キーの点更新と、区間内の順位・キー条件による重み和を求めます。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

sigma=universeと初期keyの異なる値数。N=位置数。位置を挿入・削除する公開APIはありません。

**共通の前提**

- key.size()==val.size()。区間は0<=l<=r<=N、点は0<=p<N。将来使うキーはuniverseに事前登録。初期keyは自動的に候補へ追加。Keyは比較可能、Valは加減算・順序比較が可能な数値型。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [DynamicWeightedWaveletMatrix::build](#api-2)
- [DynamicWeightedWaveletMatrix::size](#api-3)
- [DynamicWeightedWaveletMatrix::get_key](#api-4)
- [DynamicWeightedWaveletMatrix::get_value](#api-5)
- [DynamicWeightedWaveletMatrix::get](#api-6)
- [DynamicWeightedWaveletMatrix::set_key](#api-7)
- [DynamicWeightedWaveletMatrix::set_value](#api-8)
- [DynamicWeightedWaveletMatrix::set](#api-9)
- [DynamicWeightedWaveletMatrix::range_sum](#api-10)
- [DynamicWeightedWaveletMatrix::count](#api-11)
- [DynamicWeightedWaveletMatrix::range_freq](#api-12)
- [DynamicWeightedWaveletMatrix::range_freq_leq](#api-13)
- [DynamicWeightedWaveletMatrix::sum_less](#api-14)
- [DynamicWeightedWaveletMatrix::sum_leq](#api-15)
- [DynamicWeightedWaveletMatrix::sum_range](#api-16)
- [DynamicWeightedWaveletMatrix::sum_equal](#api-17)
- [DynamicWeightedWaveletMatrix::kth_smallest](#api-18)
- [DynamicWeightedWaveletMatrix::kth_largest](#api-19)
- [DynamicWeightedWaveletMatrix::prev_key](#api-20)
- [DynamicWeightedWaveletMatrix::next_key](#api-21)
- [DynamicWeightedWaveletMatrix::min_key](#api-22)
- [DynamicWeightedWaveletMatrix::max_key](#api-23)
- [DynamicWeightedWaveletMatrix::sum_k_smallest](#api-24)
- [DynamicWeightedWaveletMatrix::sum_k_largest](#api-25)
- [DynamicWeightedWaveletMatrix::lower_bound_by_sum](#api-26)

</details>

## 型・テンプレート引数

```cpp
template<class Key = long long, class Val = long long>
class DynamicWeightedWaveletMatrix;
```

## この実装の注意点

内部の動的bit列はTreapであり、検索・更新の対数計算量は優先度の乱数に関する期待値です。sum_k_smallestの同一キーは位置の小さい方を先に取ります。k個の和は0<=k<=r-l、k番目の値は0<=k<r-lです。

<a id="api-1"></a>

## コンストラクタ

```cpp
DynamicWeightedWaveletMatrix();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
DynamicWeightedWaveletMatrix( const vector<Key>& key, const vector<Val>& val, vector<Key> universe );
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O((N+|universe|) log(N+|universe|+1)+N log sigma log(N+1))の上界。

```cpp
DynamicWeightedWaveletMatrix(const vector<Key>& a, vector<Key> universe) requires same_as<Key, Val>;
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O((N+|universe|) log(N+|universe|+1)+N log sigma log(N+1))の上界。

<a id="api-2"></a>

## DynamicWeightedWaveletMatrix::build

```cpp
void build( const vector<Key>& key, const vector<Val>& val, vector<Key> universe );
void build(const vector<Key>& a, vector<Key> universe) requires same_as<Key, Val>;
```

候補キーを圧縮し、keyとvalを再構築します。keyだけの版はkey=valでKeyとValが同型の場合に使用できます。

**計算量**

- 構築と同じ。

<a id="api-3"></a>

## DynamicWeightedWaveletMatrix::size

```cpp
int size() const;
```

位置数Nを返します。

**計算量**

- O(1)。

<a id="api-4"></a>

## DynamicWeightedWaveletMatrix::get_key

```cpp
Key get_key(int p) const;
```

位置pのキーを返します。

**計算量**

- O(1)。

<a id="api-5"></a>

## DynamicWeightedWaveletMatrix::get_value

```cpp
Val get_value(int p) const;
```

位置pの重みを返します。

**計算量**

- O(1)。

<a id="api-6"></a>

## DynamicWeightedWaveletMatrix::get

```cpp
pair<Key, Val> get(int p) const;
```

位置pの {key,val} を返します。

**計算量**

- O(1)。

<a id="api-7"></a>

## DynamicWeightedWaveletMatrix::set_key

```cpp
void set_key(int p, const Key& key);
```

位置pのキーを候補内のkeyへ変更し、重みは保持します。

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-8"></a>

## DynamicWeightedWaveletMatrix::set_value

```cpp
void set_value(int p, Val val);
```

位置pの重みだけをvalへ代入します。

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-9"></a>

## DynamicWeightedWaveletMatrix::set

```cpp
void set(int p, const Key& key, Val val);
```

位置pのキーと重みを同時に代入します。

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-10"></a>

## DynamicWeightedWaveletMatrix::range_sum

```cpp
Val range_sum(int l, int r) const;
```

キー条件なしに、元の位置[l,r)の重み和を返します。

**計算量**

- O(log(N+1))。

<a id="api-11"></a>

## DynamicWeightedWaveletMatrix::count

```cpp
int count(const Key& x, int l, int r) const;
```

key==x の個数

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-12"></a>

## DynamicWeightedWaveletMatrix::range_freq

```cpp
int range_freq(int l, int r, const Key& x) const;
int range_freq(int l, int r, const Key& a, const Key& b) const;
```

key<x の個数

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-13"></a>

## DynamicWeightedWaveletMatrix::range_freq_leq

```cpp
int range_freq_leq(int l, int r, const Key& x) const;
```

key<=x の個数

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-14"></a>

## DynamicWeightedWaveletMatrix::sum_less

```cpp
Val sum_less(int l, int r, const Key& x) const;
```

key<x の val 総和

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-15"></a>

## DynamicWeightedWaveletMatrix::sum_leq

```cpp
Val sum_leq(int l, int r, const Key& x) const;
```

key<=x の val 総和

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-16"></a>

## DynamicWeightedWaveletMatrix::sum_range

```cpp
Val sum_range(int l, int r, const Key& a, const Key& b) const;
```

a<=key<b の val 総和

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-17"></a>

## DynamicWeightedWaveletMatrix::sum_equal

```cpp
Val sum_equal(int l, int r, const Key& x) const;
```

key==x の val 総和

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-18"></a>

## DynamicWeightedWaveletMatrix::kth_smallest

```cpp
Key kth_smallest(int l, int r, int k) const;
```

0-indexed k番目に小さい key

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-19"></a>

## DynamicWeightedWaveletMatrix::kth_largest

```cpp
Key kth_largest(int l, int r, int k) const;
```

0-indexed k番目に大きい key

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-20"></a>

## DynamicWeightedWaveletMatrix::prev_key

```cpp
optional<Key> prev_key(int l, int r, const Key& x) const;
```

x未満の最大 key、なければ nullopt

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-21"></a>

## DynamicWeightedWaveletMatrix::next_key

```cpp
optional<Key> next_key(int l, int r, const Key& x) const;
```

x以上の最小 key、なければ nullopt

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-22"></a>

## DynamicWeightedWaveletMatrix::min_key

```cpp
optional<Key> min_key(int l, int r) const;
```

最小 key、空なら nullopt

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-23"></a>

## DynamicWeightedWaveletMatrix::max_key

```cpp
optional<Key> max_key(int l, int r) const;
```

最大 key、空なら nullopt

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-24"></a>

## DynamicWeightedWaveletMatrix::sum_k_smallest

```cpp
Val sum_k_smallest(int l, int r, int k) const;
```

key が小さい方から k 個の val 総和

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-25"></a>

## DynamicWeightedWaveletMatrix::sum_k_largest

```cpp
Val sum_k_largest(int l, int r, int k) const;
```

key が大きい方から k 個の val 総和

**計算量**

- 期待O(log sigma log(N+1))。

<a id="api-26"></a>

## DynamicWeightedWaveletMatrix::lower_bound_by_sum

```cpp
optional<Key> lower_bound_by_sum(int l, int r, Val k) const;
```

区間の要素をキー昇順に並べた重みの累積和がk以上になる最小キーをoptionalで返します。達しなければnullopt。

**制約**

- 対象区間の全Valが非負

**計算量**

- 期待O(log sigma log(N+1))。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
DynamicWeightedWaveletMatrix<ll,ll> w(vl{3,1,2},vl{30,10,20},vl{1,2,3,4});
assert(w.sum_less(0,3,3)==30 && w.kth_smallest(0,3,0)==1);
w.set(0,4,40); assert(w.range_sum(0,3)==70);
assert(w.sum_k_smallest(0,3,2)==30);
```

## メモリ

O(N log sigma+sigma)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
