# Sorted Implicit Treap

[説明書の目次](../index.md) · [コード](../../../DataStructureAdvanced/sorted_implicit_treap.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/DataStructureAdvanced/sorted_implicit_treap.cpp)

昇順multisetをTreapで管理し、順位・区間和・値域検索・分割を行います。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=要素数、D=削除数。split/extractで返すオブジェクトはmoveして使用します。

**共通の前提**

- 重複可。添字区間は[l,r)、値域のL,Rは両端を含む[L,R]。k番目の取得は0<=k<N、k個の和は0<=k<=N。和・遅延加算がllに収まること。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [SortedImplicitTreap::size](#api-2)
- [SortedImplicitTreap::empty](#api-3)
- [SortedImplicitTreap::total_sum](#api-4)
- [SortedImplicitTreap::clear](#api-5)
- [SortedImplicitTreap::insert](#api-6)
- [SortedImplicitTreap::erase](#api-7)
- [SortedImplicitTreap::erase_all](#api-8)
- [SortedImplicitTreap::erase_kth](#api-9)
- [SortedImplicitTreap::erase_at](#api-10)
- [SortedImplicitTreap::erase_index_range](#api-11)
- [SortedImplicitTreap::erase_value_range](#api-12)
- [SortedImplicitTreap::erase_less](#api-13)
- [SortedImplicitTreap::erase_leq](#api-14)
- [SortedImplicitTreap::erase_geq](#api-15)
- [SortedImplicitTreap::erase_greater](#api-16)
- [SortedImplicitTreap::get](#api-17)
- [SortedImplicitTreap::kth](#api-18)
- [SortedImplicitTreap::front](#api-19)
- [SortedImplicitTreap::back](#api-20)
- [SortedImplicitTreap::set_val](#api-21)
- [SortedImplicitTreap::lower_bound](#api-22)
- [SortedImplicitTreap::upper_bound](#api-23)
- [SortedImplicitTreap::order_of_key](#api-24)
- [SortedImplicitTreap::count_less](#api-25)
- [SortedImplicitTreap::count_leq](#api-26)
- [SortedImplicitTreap::count_geq](#api-27)
- [SortedImplicitTreap::count_greater](#api-28)
- [SortedImplicitTreap::count](#api-29)
- [SortedImplicitTreap::contains](#api-30)
- [SortedImplicitTreap::first_index](#api-31)
- [SortedImplicitTreap::last_index](#api-32)
- [SortedImplicitTreap::min_value](#api-33)
- [SortedImplicitTreap::max_value](#api-34)
- [SortedImplicitTreap::pop_min](#api-35)
- [SortedImplicitTreap::pop_max](#api-36)
- [SortedImplicitTreap::predecessor](#api-37)
- [SortedImplicitTreap::successor](#api-38)
- [SortedImplicitTreap::floor_value](#api-39)
- [SortedImplicitTreap::ceil_value](#api-40)
- [SortedImplicitTreap::median_lower](#api-41)
- [SortedImplicitTreap::median_upper](#api-42)
- [SortedImplicitTreap::median_pair](#api-43)
- [SortedImplicitTreap::prefix_sum](#api-44)
- [SortedImplicitTreap::range_sum](#api-45)
- [SortedImplicitTreap::sum_smallest](#api-46)
- [SortedImplicitTreap::sum_largest](#api-47)
- [SortedImplicitTreap::sum_less](#api-48)
- [SortedImplicitTreap::sum_leq](#api-49)
- [SortedImplicitTreap::sum_geq](#api-50)
- [SortedImplicitTreap::sum_greater](#api-51)
- [SortedImplicitTreap::sum_value_range](#api-52)
- [SortedImplicitTreap::count_value_range](#api-53)
- [SortedImplicitTreap::add_all](#api-54)
- [SortedImplicitTreap::split_at](#api-55)
- [SortedImplicitTreap::split_less](#api-56)
- [SortedImplicitTreap::split_leq](#api-57)
- [SortedImplicitTreap::extract_range](#api-58)
- [SortedImplicitTreap::extract_value_range](#api-59)
- [SortedImplicitTreap::merge_with](#api-60)
- [SortedImplicitTreap::to_vector](#api-61)
- [SortedImplicitTreap::build](#api-62)
- [SortedImplicitTreap::build_sorted](#api-63)

</details>

## 型・テンプレート引数

```cpp
struct SortedImplicitTreap;
```

## この実装の注意点

コピーは無効化され、moveは可能です。split系は元のtrを空にします。merge_withは左の最大値<=右の最小値が必要で、右側オブジェクトは空になります。min/max/中央値/pop系は非空で使います。

<a id="api-1"></a>

## コンストラクタ

```cpp
SortedImplicitTreap();
```

空の状態で構築します。

**計算量**

- O(1)。

<a id="api-2"></a>

## SortedImplicitTreap::size

```cpp
int size() const;
```

要素数

**計算量**

- O(1)。

<a id="api-3"></a>

## SortedImplicitTreap::empty

```cpp
bool empty() const;
```

空か判定

**計算量**

- O(1)。

<a id="api-4"></a>

## SortedImplicitTreap::total_sum

```cpp
ll total_sum() const;
```

全要素の総和

**計算量**

- O(1)。

<a id="api-5"></a>

## SortedImplicitTreap::clear

```cpp
void clear();
```

全要素削除

**計算量**

- O(N)。

<a id="api-6"></a>

## SortedImplicitTreap::insert

```cpp
void insert(ll x);
```

x を挿入

**計算量**

- 期待 O(log N)。

<a id="api-7"></a>

## SortedImplicitTreap::erase

```cpp
bool erase(ll x);
```

x を1個削除、成功なら true

**計算量**

- 期待 O(log N)。

<a id="api-8"></a>

## SortedImplicitTreap::erase_all

```cpp
int erase_all(ll x);
```

x を全て削除、削除数を返す

**計算量**

- 期待 O(log N + 削除数)。

<a id="api-9"></a>

## SortedImplicitTreap::erase_kth

```cpp
ll erase_kth(int k);
```

0-indexed k 番目を削除して値を返す

**計算量**

- 期待 O(log N)。

<a id="api-10"></a>

## SortedImplicitTreap::erase_at

```cpp
void erase_at(int k);
```

0-indexed k 番目を削除

**計算量**

- 期待 O(log N)。

<a id="api-11"></a>

## SortedImplicitTreap::erase_index_range

```cpp
int erase_index_range(int l, int r);
```

index [l,r) を削除、削除数を返す

**計算量**

- 期待 O(log N + 削除数)。

<a id="api-12"></a>

## SortedImplicitTreap::erase_value_range

```cpp
int erase_value_range(ll L, ll R);
```

値 [L,R] を削除、削除数を返す

**計算量**

- 期待 O(log N + 削除数)。

<a id="api-13"></a>

## SortedImplicitTreap::erase_less

```cpp
int erase_less(ll x);
```

x 未満を全削除、削除数を返す

**計算量**

- 期待 O(log N + 削除数)。

<a id="api-14"></a>

## SortedImplicitTreap::erase_leq

```cpp
int erase_leq(ll x);
```

x 以下を全削除、削除数を返す

**計算量**

- 期待 O(log N + 削除数)。

<a id="api-15"></a>

## SortedImplicitTreap::erase_geq

```cpp
int erase_geq(ll x);
```

x 以上を全削除、削除数を返す

**計算量**

- 期待 O(log N + 削除数)。

<a id="api-16"></a>

## SortedImplicitTreap::erase_greater

```cpp
int erase_greater(ll x);
```

x より大きい値を全削除、削除数を返す

**計算量**

- 期待 O(log N + 削除数)。

<a id="api-17"></a>

## SortedImplicitTreap::get

```cpp
ll get(int k);
```

0-indexed k 番目の値

**計算量**

- 期待 O(log N)。

<a id="api-18"></a>

## SortedImplicitTreap::kth

```cpp
ll kth(int k);
```

get(k) と同じ

**計算量**

- 期待 O(log N)。

<a id="api-19"></a>

## SortedImplicitTreap::front

```cpp
ll front();
```

最小値

**計算量**

- 期待 O(log N)。

<a id="api-20"></a>

## SortedImplicitTreap::back

```cpp
ll back();
```

最大値

**計算量**

- 期待 O(log N)。

<a id="api-21"></a>

## SortedImplicitTreap::set_val

```cpp
void set_val(int k, ll x);
```

k 番目を削除して x を再挿入

**計算量**

- 期待 O(log N)。

<a id="api-22"></a>

## SortedImplicitTreap::lower_bound

```cpp
int lower_bound(ll x);
```

x 以上の最初の index

**計算量**

- 期待 O(log N)。

<a id="api-23"></a>

## SortedImplicitTreap::upper_bound

```cpp
int upper_bound(ll x);
```

x より大きい最初の index

**計算量**

- 期待 O(log N)。

<a id="api-24"></a>

## SortedImplicitTreap::order_of_key

```cpp
int order_of_key(ll x);
```

x 未満の要素数

**計算量**

- 期待 O(log N)。

<a id="api-25"></a>

## SortedImplicitTreap::count_less

```cpp
int count_less(ll x);
```

x 未満の個数

**計算量**

- 期待 O(log N)。

<a id="api-26"></a>

## SortedImplicitTreap::count_leq

```cpp
int count_leq(ll x);
```

x 以下の個数

**計算量**

- 期待 O(log N)。

<a id="api-27"></a>

## SortedImplicitTreap::count_geq

```cpp
int count_geq(ll x);
```

x 以上の個数

**計算量**

- 期待 O(log N)。

<a id="api-28"></a>

## SortedImplicitTreap::count_greater

```cpp
int count_greater(ll x);
```

x より大きい個数

**計算量**

- 期待 O(log N)。

<a id="api-29"></a>

## SortedImplicitTreap::count

```cpp
int count(ll x);
```

x の個数

**計算量**

- 期待 O(log N)。

<a id="api-30"></a>

## SortedImplicitTreap::contains

```cpp
bool contains(ll x);
```

x が存在するか

**計算量**

- 期待 O(log N)。

<a id="api-31"></a>

## SortedImplicitTreap::first_index

```cpp
int first_index(ll x);
```

x の最初の index、なければ -1

**計算量**

- 期待 O(log N)。

<a id="api-32"></a>

## SortedImplicitTreap::last_index

```cpp
int last_index(ll x);
```

x の最後の index、なければ -1

**計算量**

- 期待 O(log N)。

<a id="api-33"></a>

## SortedImplicitTreap::min_value

```cpp
ll min_value();
```

最小値

**計算量**

- 期待 O(log N)。

<a id="api-34"></a>

## SortedImplicitTreap::max_value

```cpp
ll max_value();
```

最大値

**計算量**

- 期待 O(log N)。

<a id="api-35"></a>

## SortedImplicitTreap::pop_min

```cpp
ll pop_min();
```

最小値を削除して返す

**計算量**

- 期待 O(log N)。

<a id="api-36"></a>

## SortedImplicitTreap::pop_max

```cpp
ll pop_max();
```

最大値を削除して返す

**計算量**

- 期待 O(log N)。

<a id="api-37"></a>

## SortedImplicitTreap::predecessor

```cpp
optional<ll> predecessor(ll x);
```

最大の y<x、なければ nullopt

**計算量**

- 期待 O(log N)。

<a id="api-38"></a>

## SortedImplicitTreap::successor

```cpp
optional<ll> successor(ll x);
```

最小の y>x、なければ nullopt

**計算量**

- 期待 O(log N)。

<a id="api-39"></a>

## SortedImplicitTreap::floor_value

```cpp
optional<ll> floor_value(ll x);
```

最大の y<=x、なければ nullopt

**計算量**

- 期待 O(log N)。

<a id="api-40"></a>

## SortedImplicitTreap::ceil_value

```cpp
optional<ll> ceil_value(ll x);
```

最小の y>=x、なければ nullopt

**計算量**

- 期待 O(log N)。

<a id="api-41"></a>

## SortedImplicitTreap::median_lower

```cpp
ll median_lower();
```

小さい側の中央値

**計算量**

- 期待 O(log N)。

<a id="api-42"></a>

## SortedImplicitTreap::median_upper

```cpp
ll median_upper();
```

大きい側の中央値

**計算量**

- 期待 O(log N)。

<a id="api-43"></a>

## SortedImplicitTreap::median_pair

```cpp
pair<ll,ll> median_pair();
```

小さい側・大きい側の中央値のpairを返します。奇数個なら両方同じ値。

**制約**

- N>0

**計算量**

- 期待O(log N)。

<a id="api-44"></a>

## SortedImplicitTreap::prefix_sum

```cpp
ll prefix_sum(int k);
```

小さい方から k 個の総和

**計算量**

- 期待 O(log N)。

<a id="api-45"></a>

## SortedImplicitTreap::range_sum

```cpp
ll range_sum(int l, int r);
```

index [l,r) の総和

**計算量**

- 期待 O(log N)。

<a id="api-46"></a>

## SortedImplicitTreap::sum_smallest

```cpp
ll sum_smallest(int k);
```

小さい方から k 個の総和

**計算量**

- 期待 O(log N)。

<a id="api-47"></a>

## SortedImplicitTreap::sum_largest

```cpp
ll sum_largest(int k);
```

大きい方から k 個の総和

**計算量**

- 期待 O(log N)。

<a id="api-48"></a>

## SortedImplicitTreap::sum_less

```cpp
ll sum_less(ll x);
```

x 未満の値の総和

**計算量**

- 期待 O(log N)。

<a id="api-49"></a>

## SortedImplicitTreap::sum_leq

```cpp
ll sum_leq(ll x);
```

x 以下の値の総和

**計算量**

- 期待 O(log N)。

<a id="api-50"></a>

## SortedImplicitTreap::sum_geq

```cpp
ll sum_geq(ll x);
```

x 以上の値の総和

**計算量**

- 期待 O(log N)。

<a id="api-51"></a>

## SortedImplicitTreap::sum_greater

```cpp
ll sum_greater(ll x);
```

x より大きい値の総和

**計算量**

- 期待 O(log N)。

<a id="api-52"></a>

## SortedImplicitTreap::sum_value_range

```cpp
ll sum_value_range(ll L, ll R);
```

L<=x<=R の総和

**計算量**

- 期待 O(log N)。

<a id="api-53"></a>

## SortedImplicitTreap::count_value_range

```cpp
int count_value_range(ll L, ll R);
```

L<=x<=R の個数

**計算量**

- 期待 O(log N)。

<a id="api-54"></a>

## SortedImplicitTreap::add_all

```cpp
void add_all(ll x);
```

全要素に x を加算

**計算量**

- O(1)。

<a id="api-55"></a>

## SortedImplicitTreap::split_at

```cpp
pair<SortedImplicitTreap,SortedImplicitTreap> split_at(int k);
```

[0,k) と [k,N) に分割、tr は空

**計算量**

- 期待 O(log N)。

<a id="api-56"></a>

## SortedImplicitTreap::split_less

```cpp
pair<SortedImplicitTreap,SortedImplicitTreap> split_less(ll x);
```

値 <x と >=x に分割、tr は空

**計算量**

- 期待 O(log N)。

<a id="api-57"></a>

## SortedImplicitTreap::split_leq

```cpp
pair<SortedImplicitTreap,SortedImplicitTreap> split_leq(ll x);
```

値 <=x と >x に分割、tr は空

**計算量**

- 期待 O(log N)。

<a id="api-58"></a>

## SortedImplicitTreap::extract_range

```cpp
SortedImplicitTreap extract_range(int l, int r);
```

index [l,r) を tr から切り出して返す

**計算量**

- 期待 O(log N)。

<a id="api-59"></a>

## SortedImplicitTreap::extract_value_range

```cpp
SortedImplicitTreap extract_value_range(ll L, ll R);
```

値 [L,R] を tr から切り出して返す

**計算量**

- 期待 O(log N)。

<a id="api-60"></a>

## SortedImplicitTreap::merge_with

```cpp
void merge_with(SortedImplicitTreap&& other);
```

tr の最大値<=other の最小値として結合、other は空

**計算量**

- 期待 O(log N)。

<a id="api-61"></a>

## SortedImplicitTreap::to_vector

```cpp
vector<ll> to_vector();
```

昇順 vector に変換

**計算量**

- O(N)。

<a id="api-62"></a>

## SortedImplicitTreap::build

```cpp
void build(vector<ll> v);
```

v をソートして構築

**計算量**

- 期待 O(N log N)。

<a id="api-63"></a>

## SortedImplicitTreap::build_sorted

```cpp
void build_sorted(const vector<ll>& v);
```

昇順 v から構築

**計算量**

- 期待 O(N log N)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
SortedImplicitTreap t; t.build({4,1,4,2});
assert(t.get(0)==1 && t.count(4)==2);
assert(t.sum_smallest(2)==3);
t.erase(4); assert(t.total_sum()==7);
auto [a,b]=t.split_at(1); assert(t.empty() && a.size()==1 && b.size()==2);
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
