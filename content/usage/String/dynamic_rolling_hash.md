# 動的Rolling Hash

[説明書の目次](../index.md) · [コード](../../../String/dynamic_rolling_hash.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/String/dynamic_rolling_hash.cpp)

文字列や整数列の双方向・二重Rolling Hashを保持し、部分列比較や回文判定を行います。列の挿入・削除・更新にも対応します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=列長、K=繰り返し数。Hashはf（順方向）・r（逆方向）・pw（基数の累乗）・len（長さ）を保持します。

**共通の前提**

- 区間は0<=l<=r<=N。get/setは0<=i<N。挿入位置は0<=p<=N。異なるオブジェクトの比較は同じクラス間で行います。Hashの一致は確率的判定です。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [DynamicRollingHash::e](#api-2)
- [DynamicRollingHash::single](#api-3)
- [DynamicRollingHash::concat](#api-4)
- [DynamicRollingHash::concat_all](#api-5)
- [DynamicRollingHash::reversed](#api-6)
- [DynamicRollingHash::repeat](#api-7)
- [DynamicRollingHash::same](#api-8)
- [DynamicRollingHash::same_reverse](#api-9)
- [DynamicRollingHash::size](#api-10)
- [DynamicRollingHash::empty](#api-11)
- [DynamicRollingHash::clear](#api-12)
- [DynamicRollingHash::assign](#api-13)
- [DynamicRollingHash::get](#api-14)
- [DynamicRollingHash::set](#api-15)
- [DynamicRollingHash::insert](#api-16)
- [DynamicRollingHash::push_front](#api-17)
- [DynamicRollingHash::push_back](#api-18)
- [DynamicRollingHash::erase](#api-19)
- [DynamicRollingHash::replace](#api-20)
- [DynamicRollingHash::range_assign](#api-21)
- [DynamicRollingHash::reverse](#api-22)
- [DynamicRollingHash::rotate](#api-23)
- [DynamicRollingHash::cut_and_paste](#api-24)
- [DynamicRollingHash::swap_ranges](#api-25)
- [DynamicRollingHash::prod](#api-26)
- [DynamicRollingHash::all_prod](#api-27)
- [DynamicRollingHash::hash](#api-28)
- [DynamicRollingHash::rev_hash](#api-29)
- [DynamicRollingHash::equal](#api-30)
- [DynamicRollingHash::reverse_equal](#api-31)
- [DynamicRollingHash::is_palindrome](#api-32)
- [DynamicRollingHash::equal_with](#api-33)
- [DynamicRollingHash::lcp_with](#api-34)
- [DynamicRollingHash::lcp](#api-35)
- [DynamicRollingHash::lcs_with](#api-36)
- [DynamicRollingHash::lcs](#api-37)
- [DynamicRollingHash::lcp_reverse_with](#api-38)
- [DynamicRollingHash::lcp_reverse](#api-39)
- [DynamicRollingHash::first_diff_with](#api-40)
- [DynamicRollingHash::first_diff](#api-41)
- [DynamicRollingHash::compare_with](#api-42)
- [DynamicRollingHash::compare](#api-43)
- [DynamicRollingHash::less](#api-44)
- [DynamicRollingHash::pal_radius_odd](#api-45)
- [DynamicRollingHash::pal_radius_even](#api-46)
- [DynamicRollingHash::max_palindrome_odd](#api-47)
- [DynamicRollingHash::max_palindrome_even](#api-48)
- [DynamicRollingHash::has_period](#api-49)
- [DynamicRollingHash::is_repeat](#api-50)
- [DynamicRollingHash::after_erase](#api-51)
- [DynamicRollingHash::after_insert](#api-52)
- [DynamicRollingHash::after_replace](#api-53)
- [DynamicRollingHash::after_reverse](#api-54)
- [DynamicRollingHash::to_vector](#api-55)
- [DynamicRollingHash::to_string_chars](#api-56)

</details>

## 型・テンプレート引数

```cpp
struct DynamicRollingHash;
```

## この実装の注意点

法は2^61-1で、基数・saltを実行時にランダム化します。文字列の比較を数学的に完全保証するAPIではありません。pal_radius_odd(c)のdは[c-d,c+d+1)を表し、中心1文字だけならd=0です。pal_radius_even(c)はc-1とcの間を中心に[c-d,c+d)を表します。コピーはできずmove可能です。cut_and_pasteの挿入位置は切り取り後の列の添字です。swap_rangesはr1<=l2の非交差区間を指定します。

<a id="api-1"></a>

## コンストラクタ

```cpp
DynamicRollingHash();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
explicit DynamicRollingHash(const string&s);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

```cpp
explicit DynamicRollingHash(const vector<T>&a);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

<a id="api-2"></a>

## DynamicRollingHash::e

```cpp
static Hash e();
```

空列のHashを返します。

**計算量**

- O(1)。

<a id="api-3"></a>

## DynamicRollingHash::single

```cpp
static Hash single(ll x);
```

1要素[x]のHashを返す

**計算量**

- O(1)。

<a id="api-4"></a>

## DynamicRollingHash::concat

```cpp
static Hash concat(const Hash&a,const Hash&b);
```

A+BのHashを返す

**計算量**

- O(1)。

<a id="api-5"></a>

## DynamicRollingHash::concat_all

```cpp
static Hash concat_all(initializer_list<Hash> v);
static Hash concat_all(const vector<Hash>&v);
```

複数のHashを順番に連結したHashを返します。

**計算量**

- O(Hash数)。

<a id="api-6"></a>

## DynamicRollingHash::reversed

```cpp
static Hash reversed(Hash h);
```

reverse(A)のHashを返す

**計算量**

- O(1)。

<a id="api-7"></a>

## DynamicRollingHash::repeat

```cpp
static Hash repeat(Hash h,long long k);
```

Aをk回連結したHashを返す

**計算量**

- O(logK)。

<a id="api-8"></a>

## DynamicRollingHash::same

```cpp
static bool same(const Hash&a,const Hash&b);
```

2つのHashが一致するか判定

**計算量**

- O(1)。

<a id="api-9"></a>

## DynamicRollingHash::same_reverse

```cpp
static bool same_reverse(const Hash&a,const Hash&b);
```

A==reverse(B)か判定

**計算量**

- O(1)。

<a id="api-10"></a>

## DynamicRollingHash::size

```cpp
int size()const;
```

現在の列長を返す

**計算量**

- O(1)。

<a id="api-11"></a>

## DynamicRollingHash::empty

```cpp
bool empty()const;
```

列長が0かを返します。

**計算量**

- O(1)。

<a id="api-12"></a>

## DynamicRollingHash::clear

```cpp
void clear();
```

列を空にする

**計算量**

- O(N)。

<a id="api-13"></a>

## DynamicRollingHash::assign

```cpp
void assign(const string&s);
void assign(const vector<T>&a);
```

string/vectorで再構築

**計算量**

- O(N)。

<a id="api-14"></a>

## DynamicRollingHash::get

```cpp
ll get(int i);
```

A[i]を返す

**計算量**

- 期待O(logN)。

<a id="api-15"></a>

## DynamicRollingHash::set

```cpp
void set(int i,ll x);
void set(int i,char c);
```

A[i]=xに更新

**計算量**

- 期待O(logN)。

<a id="api-16"></a>

## DynamicRollingHash::insert

```cpp
void insert(int p,ll x);
void insert(int p,char c);
void insert(int p,const string&s);
void insert(int p,const vector<T>&v);
```

iの直前にxを挿入

**計算量**

- 期待O(logN)。

<a id="api-17"></a>

## DynamicRollingHash::push_front

```cpp
void push_front(ll x);
```

先頭にxを挿入

**計算量**

- 期待O(logN)。

<a id="api-18"></a>

## DynamicRollingHash::push_back

```cpp
void push_back(ll x);
```

末尾にxを挿入

**計算量**

- 期待O(logN)。

<a id="api-19"></a>

## DynamicRollingHash::erase

```cpp
void erase(int p);
void erase(int l,int r);
```

A[i]を削除

**計算量**

- 期待O(logN)。

<a id="api-20"></a>

## DynamicRollingHash::replace

```cpp
void replace(int l,int r,const string&s);
void replace(int l,int r,const vector<T>&v);
```

[l,r)をstring/vectorに置換 +|s/v|)

**計算量**

- 期待O(logN+(r-l)。

<a id="api-21"></a>

## DynamicRollingHash::range_assign

```cpp
void range_assign(int l,int r,ll x);
void range_assign(int l,int r,char c);
```

[l,r)をすべてxに変更

**計算量**

- 期待O(logN)。

<a id="api-22"></a>

## DynamicRollingHash::reverse

```cpp
void reverse(int l,int r);
```

[l,r)を反転

**計算量**

- 期待O(logN)。

<a id="api-23"></a>

## DynamicRollingHash::rotate

```cpp
void rotate(int l,int r,long long k);
```

[l,r)を右にk回転、k<0なら左回転

**計算量**

- 期待O(logN)。

<a id="api-24"></a>

## DynamicRollingHash::cut_and_paste

```cpp
void cut_and_paste(int l,int r,int p);
```

[l,r)を切り取り削除後のposへ挿入

**計算量**

- 期待O(logN)。

<a id="api-25"></a>

## DynamicRollingHash::swap_ranges

```cpp
void swap_ranges(int l1,int r1,int l2,int r2);
```

非交差な2区間を交換(r1<=l2)

**計算量**

- 期待O(logN)。

<a id="api-26"></a>

## DynamicRollingHash::prod

```cpp
Hash prod(int l,int r);
```

[l,r)の双方向二重Hashを返す

**計算量**

- 期待O(logN)。

<a id="api-27"></a>

## DynamicRollingHash::all_prod

```cpp
Hash all_prod()const;
```

列全体の双方向二重Hashを返す

**計算量**

- O(1)。

<a id="api-28"></a>

## DynamicRollingHash::hash

```cpp
array<ull,2> hash(int l,int r);
```

[l,r)の順方向二重Hashを返す

**計算量**

- 期待O(logN)。

<a id="api-29"></a>

## DynamicRollingHash::rev_hash

```cpp
array<ull,2> rev_hash(int l,int r);
```

reverse([l,r))の二重Hashを返す

**計算量**

- 期待O(logN)。

<a id="api-30"></a>

## DynamicRollingHash::equal

```cpp
bool equal(int l1,int r1,int l2,int r2);
```

2区間が一致するか判定

**計算量**

- 期待O(logN)。

<a id="api-31"></a>

## DynamicRollingHash::reverse_equal

```cpp
bool reverse_equal(int l1,int r1,int l2,int r2);
```

左区間と右区間の反転が一致するか判定

**計算量**

- 期待O(logN)。

<a id="api-32"></a>

## DynamicRollingHash::is_palindrome

```cpp
bool is_palindrome(int l,int r);
```

[l,r)が回文か判定

**計算量**

- 期待O(logN)。

<a id="api-33"></a>

## DynamicRollingHash::equal_with

```cpp
bool equal_with(DynamicRollingHash&o,int l1,int r1,int l2,int r2);
```

rhとotherの区間一致判定

**計算量**

- 期待O(logN)。

<a id="api-34"></a>

## DynamicRollingHash::lcp_with

```cpp
int lcp_with(DynamicRollingHash&o,int l1,int r1,int l2,int r2);
```

rhとotherのLCP長を返す

**計算量**

- 期待O(log^2N)。

<a id="api-35"></a>

## DynamicRollingHash::lcp

```cpp
int lcp(int l1,int r1,int l2,int r2);
```

2区間のLCP長を返す

**計算量**

- 期待O(log^2N)。

<a id="api-36"></a>

## DynamicRollingHash::lcs_with

```cpp
int lcs_with(DynamicRollingHash&o,int l1,int r1,int l2,int r2);
```

rhとotherのLCS長を返す

**計算量**

- 期待O(log^2N)。

<a id="api-37"></a>

## DynamicRollingHash::lcs

```cpp
int lcs(int l1,int r1,int l2,int r2);
```

2区間のLCS長を返す

**計算量**

- 期待O(log^2N)。

<a id="api-38"></a>

## DynamicRollingHash::lcp_reverse_with

```cpp
int lcp_reverse_with(DynamicRollingHash&o,int l1,int r1,int l2,int r2);
```

rh区間とother区間反転のLCP長を返す

**計算量**

- 期待O(log^2N)。

<a id="api-39"></a>

## DynamicRollingHash::lcp_reverse

```cpp
int lcp_reverse(int l1,int r1,int l2,int r2);
```

左区間と右区間の反転のLCP長を返す

**計算量**

- 期待O(log^2N)。

<a id="api-40"></a>

## DynamicRollingHash::first_diff_with

```cpp
int first_diff_with(DynamicRollingHash&o,int l1,int r1,int l2,int r2);
```

rhとotherの最初の不一致offset、完全一致なら-1

**計算量**

- 期待O(log^2N)。

<a id="api-41"></a>

## DynamicRollingHash::first_diff

```cpp
int first_diff(int l1,int r1,int l2,int r2);
```

最初の不一致offset、完全一致なら-1

**計算量**

- 期待O(log^2N)。

<a id="api-42"></a>

## DynamicRollingHash::compare_with

```cpp
int compare_with(DynamicRollingHash&o,int l1,int r1,int l2,int r2);
```

rhとotherを辞書順比較して-1/0/1を返す

**計算量**

- 期待O(log^2N)。

<a id="api-43"></a>

## DynamicRollingHash::compare

```cpp
int compare(int l1,int r1,int l2,int r2);
```

辞書順比較して-1/0/1を返す

**計算量**

- 期待O(log^2N)。

<a id="api-44"></a>

## DynamicRollingHash::less

```cpp
bool less(int l1,int r1,int l2,int r2);
```

左区間が辞書順で小さいか判定

**計算量**

- 期待O(log^2N)。

<a id="api-45"></a>

## DynamicRollingHash::pal_radius_odd

```cpp
int pal_radius_odd(int c);
```

c中心の最大奇数回文[c-d,c+d+1)のdを返す

**計算量**

- 期待O(log^2N)。

<a id="api-46"></a>

## DynamicRollingHash::pal_radius_even

```cpp
int pal_radius_even(int c);
```

c-1,c間中心の最大偶数回文[c-d,c+d)のdを返す

**計算量**

- 期待O(log^2N)。

<a id="api-47"></a>

## DynamicRollingHash::max_palindrome_odd

```cpp
pair<int,int> max_palindrome_odd(int c);
```

最大奇数回文区間{l,r}を返す

**計算量**

- 期待O(log^2N)。

<a id="api-48"></a>

## DynamicRollingHash::max_palindrome_even

```cpp
pair<int,int> max_palindrome_even(int c);
```

最大偶数回文区間{l,r}を返す

**計算量**

- 期待O(log^2N)。

<a id="api-49"></a>

## DynamicRollingHash::has_period

```cpp
bool has_period(int l,int r,int p);
```

[l,r)が周期pを持つか判定

**計算量**

- 期待O(logN)。

<a id="api-50"></a>

## DynamicRollingHash::is_repeat

```cpp
bool is_repeat(int l,int r,int p);
```

[l,r)が長さpのブロックの完全反復か判定

**計算量**

- 期待O(logN)。

<a id="api-51"></a>

## DynamicRollingHash::after_erase

```cpp
Hash after_erase(int l,int r);
```

[l,r)削除後の仮想Hashを返す

**計算量**

- 期待O(logN)。

<a id="api-52"></a>

## DynamicRollingHash::after_insert

```cpp
Hash after_insert(int p,const Hash&h);
```

posにh挿入後の仮想Hashを返す

**計算量**

- 期待O(logN)。

<a id="api-53"></a>

## DynamicRollingHash::after_replace

```cpp
Hash after_replace(int l,int r,const Hash&h);
```

[l,r)をhに置換後の仮想Hashを返す

**計算量**

- 期待O(logN)。

<a id="api-54"></a>

## DynamicRollingHash::after_reverse

```cpp
Hash after_reverse(int l,int r);
```

[l,r)反転後の仮想Hashを返す

**計算量**

- 期待O(logN)。

<a id="api-55"></a>

## DynamicRollingHash::to_vector

```cpp
vector<ll> to_vector();
```

現在の列をvector<ll>で返す

**計算量**

- O(N)。

<a id="api-56"></a>

## DynamicRollingHash::to_string_chars

```cpp
string to_string_chars();
```

各値をcharとしてstringで返す

**計算量**

- O(N)。

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
DynamicRollingHash h(string("abacaba"));
assert(h.is_palindrome(0,7));
assert(h.equal(0,3,4,7));
assert(h.pal_radius_odd(3)==3);
h.set(0,'z'); assert(!h.is_palindrome(0,7));
h.reverse(0,7); assert(h.get(6)=='z');
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
