# 静的Rolling Hash

[説明書の目次](../index.md) · [コード](../../../String/static_rolling_hash.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/String/static_rolling_hash.cpp)

文字列や整数列の双方向・二重Rolling Hashを保持し、部分列比較や回文判定を行います。構築後の列は変更しません。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=列長、K=繰り返し数。Hashはf（順方向）・r（逆方向）・pw（基数の累乗）・len（長さ）を保持します。

**共通の前提**

- 区間は0<=l<=r<=N。get/setは0<=i<N。挿入位置は0<=p<=N。異なるオブジェクトの比較は同じクラス間で行います。Hashの一致は確率的判定です。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [StaticRollingHash::size](#api-2)
- [StaticRollingHash::empty](#api-3)
- [StaticRollingHash::get](#api-4)
- [StaticRollingHash::single](#api-5)
- [StaticRollingHash::concat](#api-6)
- [StaticRollingHash::reversed](#api-7)
- [StaticRollingHash::repeat](#api-8)
- [StaticRollingHash::same](#api-9)
- [StaticRollingHash::same_reverse](#api-10)
- [StaticRollingHash::prod](#api-11)
- [StaticRollingHash::all_prod](#api-12)
- [StaticRollingHash::hash](#api-13)
- [StaticRollingHash::rev_hash](#api-14)
- [StaticRollingHash::equal](#api-15)
- [StaticRollingHash::reverse_equal](#api-16)
- [StaticRollingHash::is_palindrome](#api-17)
- [StaticRollingHash::equal_with](#api-18)
- [StaticRollingHash::lcp_with](#api-19)
- [StaticRollingHash::lcp](#api-20)
- [StaticRollingHash::lcs_with](#api-21)
- [StaticRollingHash::lcs](#api-22)
- [StaticRollingHash::first_diff](#api-23)
- [StaticRollingHash::compare](#api-24)
- [StaticRollingHash::less](#api-25)
- [StaticRollingHash::pal_radius_odd](#api-26)
- [StaticRollingHash::pal_radius_even](#api-27)
- [StaticRollingHash::max_palindrome_odd](#api-28)
- [StaticRollingHash::max_palindrome_even](#api-29)
- [StaticRollingHash::has_period](#api-30)
- [StaticRollingHash::is_repeat](#api-31)
- [StaticRollingHash::after_erase](#api-32)
- [StaticRollingHash::after_insert](#api-33)
- [StaticRollingHash::after_replace](#api-34)
- [StaticRollingHash::after_reverse](#api-35)

</details>

## 型・テンプレート引数

```cpp
struct StaticRollingHash;
```

## この実装の注意点

法は2^61-1で、基数・saltを実行時にランダム化します。文字列の比較を数学的に完全保証するAPIではありません。pal_radius_odd(c)のdは[c-d,c+d+1)を表し、中心1文字だけならd=0です。pal_radius_even(c)はc-1とcの間を中心に[c-d,c+d)を表します。仮想変更after_*はHashだけを返し、元の列を変更しません。

<a id="api-1"></a>

## コンストラクタ

```cpp
StaticRollingHash();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
StaticRollingHash(const string& s);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

```cpp
StaticRollingHash(const vector<T>& v);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

<a id="api-2"></a>

## StaticRollingHash::size

```cpp
int size() const;
```

列長を返す

**計算量**

- O(1)。

<a id="api-3"></a>

## StaticRollingHash::empty

```cpp
bool empty() const;
```

列長が0かを返します。

**計算量**

- O(1)。

<a id="api-4"></a>

## StaticRollingHash::get

```cpp
ll get(int i) const;
```

位置iの元の値をllで返します。

**制約**

- 0<=i<N

**計算量**

- O(1)。

<a id="api-5"></a>

## StaticRollingHash::single

```cpp
static Hash single(ll x);
```

1要素[x]のHashを返す

**計算量**

- O(1)。

<a id="api-6"></a>

## StaticRollingHash::concat

```cpp
static Hash concat(const Hash& a,const Hash& b);
```

A+BのHashを返す

**計算量**

- O(1)。

<a id="api-7"></a>

## StaticRollingHash::reversed

```cpp
static Hash reversed(Hash h);
```

reverse(A)のHashを返す

**計算量**

- O(1)。

<a id="api-8"></a>

## StaticRollingHash::repeat

```cpp
static Hash repeat(Hash h,long long k);
```

Aをk回連結したHashを返す

**計算量**

- O(logK)。

<a id="api-9"></a>

## StaticRollingHash::same

```cpp
static bool same(const Hash& a,const Hash& b);
```

2つのHashが一致するか判定

**計算量**

- O(1)。

<a id="api-10"></a>

## StaticRollingHash::same_reverse

```cpp
static bool same_reverse(const Hash& a,const Hash& b);
```

A==reverse(B)か判定

**計算量**

- O(1)。

<a id="api-11"></a>

## StaticRollingHash::prod

```cpp
Hash prod(int l,int r) const;
```

[l,r)の双方向二重Hashを返す

**計算量**

- O(1)。

<a id="api-12"></a>

## StaticRollingHash::all_prod

```cpp
Hash all_prod() const;
```

列全体のHashを返す

**計算量**

- O(1)。

<a id="api-13"></a>

## StaticRollingHash::hash

```cpp
array<ull,2> hash(int l,int r) const;
```

[l,r)の順方向Hashを返す

**計算量**

- O(1)。

<a id="api-14"></a>

## StaticRollingHash::rev_hash

```cpp
array<ull,2> rev_hash(int l,int r) const;
```

reverse([l,r))のHashを返す

**計算量**

- O(1)。

<a id="api-15"></a>

## StaticRollingHash::equal

```cpp
bool equal(int l1,int r1,int l2,int r2) const;
```

2区間が一致するか判定

**計算量**

- O(1)。

<a id="api-16"></a>

## StaticRollingHash::reverse_equal

```cpp
bool reverse_equal(int l1,int r1,int l2,int r2) const;
```

左区間と右区間の反転が一致するか判定

**計算量**

- O(1)。

<a id="api-17"></a>

## StaticRollingHash::is_palindrome

```cpp
bool is_palindrome(int l,int r) const;
```

[l,r)が回文か判定

**計算量**

- O(1)。

<a id="api-18"></a>

## StaticRollingHash::equal_with

```cpp
bool equal_with(const StaticRollingHash& o,int l1,int r1,int l2,int r2) const;
```

rhとotherの区間一致判定

**計算量**

- O(1)。

<a id="api-19"></a>

## StaticRollingHash::lcp_with

```cpp
int lcp_with(const StaticRollingHash& o,int l1,int r1,int l2,int r2) const;
```

rhとotherのLCP長を返す

**計算量**

- O(logN)。

<a id="api-20"></a>

## StaticRollingHash::lcp

```cpp
int lcp(int l1,int r1,int l2,int r2) const;
```

2区間のLCP長を返す

**計算量**

- O(logN)。

<a id="api-21"></a>

## StaticRollingHash::lcs_with

```cpp
int lcs_with(const StaticRollingHash& o,int l1,int r1,int l2,int r2) const;
```

rhとotherのLCS長を返す

**計算量**

- O(logN)。

<a id="api-22"></a>

## StaticRollingHash::lcs

```cpp
int lcs(int l1,int r1,int l2,int r2) const;
```

2区間のLCS長を返す

**計算量**

- O(logN)。

<a id="api-23"></a>

## StaticRollingHash::first_diff

```cpp
int first_diff(int l1,int r1,int l2,int r2) const;
```

最初の不一致offset、完全一致なら-1

**計算量**

- O(logN)。

<a id="api-24"></a>

## StaticRollingHash::compare

```cpp
int compare(int l1,int r1,int l2,int r2) const;
```

辞書順比較して-1/0/1を返す

**計算量**

- O(logN)。

<a id="api-25"></a>

## StaticRollingHash::less

```cpp
bool less(int l1,int r1,int l2,int r2) const;
```

左区間が辞書順で小さいか判定

**計算量**

- O(logN)。

<a id="api-26"></a>

## StaticRollingHash::pal_radius_odd

```cpp
int pal_radius_odd(int c) const;
```

c中心の最大奇数回文半径dを返す

**計算量**

- O(logN)。

<a id="api-27"></a>

## StaticRollingHash::pal_radius_even

```cpp
int pal_radius_even(int c) const;
```

c-1,c間中心の最大偶数回文半径dを返す

**計算量**

- O(logN)。

<a id="api-28"></a>

## StaticRollingHash::max_palindrome_odd

```cpp
pair<int,int> max_palindrome_odd(int c) const;
```

最大奇数回文区間{l,r}を返す

**計算量**

- O(logN)。

<a id="api-29"></a>

## StaticRollingHash::max_palindrome_even

```cpp
pair<int,int> max_palindrome_even(int c) const;
```

最大偶数回文区間{l,r}を返す

**計算量**

- O(logN)。

<a id="api-30"></a>

## StaticRollingHash::has_period

```cpp
bool has_period(int l,int r,int p) const;
```

[l,r)が周期pを持つか判定

**計算量**

- O(1)。

<a id="api-31"></a>

## StaticRollingHash::is_repeat

```cpp
bool is_repeat(int l,int r,int p) const;
```

長さpのブロックの完全な繰り返しか判定

**計算量**

- O(1)。

<a id="api-32"></a>

## StaticRollingHash::after_erase

```cpp
Hash after_erase(int l,int r) const;
```

[l,r)削除後の仮想Hashを返す

**計算量**

- O(1)。

<a id="api-33"></a>

## StaticRollingHash::after_insert

```cpp
Hash after_insert(int p,const Hash& h) const;
```

posにh挿入後の仮想Hashを返す

**計算量**

- O(1)。

<a id="api-34"></a>

## StaticRollingHash::after_replace

```cpp
Hash after_replace(int l,int r,const Hash& h) const;
```

[l,r)をhに置換後の仮想Hashを返す

**計算量**

- O(1)。

<a id="api-35"></a>

## StaticRollingHash::after_reverse

```cpp
Hash after_reverse(int l,int r) const;
```

[l,r)反転後の仮想Hashを返す

**計算量**

- O(1)。

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `Hash::f` | 順方向の2本のハッシュ |
| `Hash::r` | 逆方向の2本のハッシュ |
| `Hash::pw` | 2本の基数の累乗 |
| `Hash::len` | 列長 |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
StaticRollingHash h(string("abacaba"));
assert(h.is_palindrome(0,7));
assert(h.equal(0,3,4,7));
assert(h.pal_radius_odd(3)==3);
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
