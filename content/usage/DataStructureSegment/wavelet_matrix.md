# Wavelet Matrix

[説明書の目次](../index.md) · [コード](../../../DataStructureSegment/wavelet_matrix.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/DataStructureSegment/wavelet_matrix.cpp)

静的配列の区間内で、値の頻度・k番目・前後の値を取得します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=配列長、sigma=異なる値数、B=max(1,ceil(log2 sigma))。

**共通の前提**

- Tは比較可能。区間は0<=l<=r<=N。k番目は0<=k<r-l。内部で座標圧縮するので負数も利用可。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [WaveletMatrix::build](#api-2)
- [WaveletMatrix::ones](#api-3)
- [WaveletMatrix::zeros](#api-4)
- [WaveletMatrix::access](#api-5)
- [WaveletMatrix::count](#api-6)
- [WaveletMatrix::range_freq_code](#api-7)
- [WaveletMatrix::range_freq](#api-8)
- [WaveletMatrix::kth_smallest](#api-9)
- [WaveletMatrix::kth_largest](#api-10)
- [WaveletMatrix::prev_value](#api-11)
- [WaveletMatrix::next_value](#api-12)
- [WaveletMatrix::size](#api-13)

</details>

## 型・テンプレート引数

```cpp
template<class T = long long>
struct WaveletMatrix;
```

## この実装の注意点

構築後の点更新はありません。値の上限は排他的です。

<a id="api-1"></a>

## コンストラクタ

```cpp
WaveletMatrix();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
explicit WaveletMatrix(const vector<T>& a);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N log(N+1)+NB)。

<a id="api-2"></a>

## WaveletMatrix::build

```cpp
void build(const vector<T>& a);
```

配列aから座標圧縮とbitごとの前計算を再構築します。

**計算量**

- O(N log(N+1)+NB)。

<a id="api-3"></a>

## WaveletMatrix::ones

```cpp
int ones(int level, int l, int r) const;
```

内部levelのbit列で[l,r)の1の個数。元の配列の区間とは異なる内部座標です。

**制約**

- 通常は直接使わない

**計算量**

- O(1)。

<a id="api-4"></a>

## WaveletMatrix::zeros

```cpp
int zeros(int level, int l, int r) const;
```

内部levelのbit列で[l,r)の0の個数。

**制約**

- 通常は直接使わない

**計算量**

- O(1)。

<a id="api-5"></a>

## WaveletMatrix::access

```cpp
T access(int k) const;
```

元の位置kの値を返します。順位ではありません。

**制約**

- 0<=k<N

**計算量**

- O(B)。

<a id="api-6"></a>

## WaveletMatrix::count

```cpp
int count(const T& x, int l, int r) const;
```

A[l,r) に値 x が何個あるか

**計算量**

- O(log N)。

<a id="api-7"></a>

## WaveletMatrix::range_freq_code

```cpp
int range_freq_code( int l, int r, int upper ) const;
```

内部圧縮番号がupper未満の個数。通常はrange_freqを使用します。

**計算量**

- O(B)。

<a id="api-8"></a>

## WaveletMatrix::range_freq

```cpp
int range_freq( int l, int r, const T& upper ) const;
int range_freq( int l, int r, const T& lower, const T& upper ) const;
```

A[l,r) で x < upper の要素数

**計算量**

- O(log N)。

<a id="api-9"></a>

## WaveletMatrix::kth_smallest

```cpp
T kth_smallest( int l, int r, int k ) const;
```

A[l,r) の 0-indexed k 番目に小さい値

**計算量**

- O(log N)。

<a id="api-10"></a>

## WaveletMatrix::kth_largest

```cpp
T kth_largest( int l, int r, int k ) const;
```

A[l,r) の 0-indexed k 番目に大きい値

**計算量**

- O(log N)。

<a id="api-11"></a>

## WaveletMatrix::prev_value

```cpp
optional<T> prev_value( int l, int r, const T& upper ) const;
```

A[l,r) で upper 未満の最大値 存在しなければ nullopt

**計算量**

- O(log N)。

<a id="api-12"></a>

## WaveletMatrix::next_value

```cpp
optional<T> next_value( int l, int r, const T& lower ) const;
```

A[l,r) で lower 以上の最小値 存在しなければ nullopt

**計算量**

- O(log N)。

<a id="api-13"></a>

## WaveletMatrix::size

```cpp
int size() const;
```

配列長

**計算量**

- O(1)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
WaveletMatrix<ll> w(vl{3,1,4,1,5});
assert(w.kth_smallest(0,5,2)==3 && w.count(1,0,5)==2);
assert(w.range_freq(0,5,2,5)==2);
assert(w.prev_value(0,5,3).value()==1);
```

## メモリ

O(NB+sigma)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
