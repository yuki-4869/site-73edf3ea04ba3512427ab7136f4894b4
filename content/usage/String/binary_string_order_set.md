# 二進文字列の順序管理

[説明書の目次](../index.md) · [コード](../../../String/binary_string_order_set.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/String/binary_string_order_set.cpp)

固定長の01文字列を行IDで保持し、全行集合での辞書順順位・個数・ビット更新を扱います。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=行数、K=文字列幅。任意の01文字列へ更新でき、universeの事前登録は不要。

**共通の前提**

- 全行は同じ長さKで文字は0/1。行IDは0..N-1、bit位置は0..K-1。検索キーも長さK。k番目のkは0<=k<N。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [StringWaveletMatrix::build](#api-2)
- [StringWaveletMatrix::size](#api-3)
- [StringWaveletMatrix::rows](#api-4)
- [StringWaveletMatrix::width](#api-5)
- [StringWaveletMatrix::get_key](#api-6)
- [StringWaveletMatrix::get_bit](#api-7)
- [StringWaveletMatrix::ones](#api-8)
- [StringWaveletMatrix::zeros](#api-9)
- [StringWaveletMatrix::set_key](#api-10)
- [StringWaveletMatrix::set_bit](#api-11)
- [StringWaveletMatrix::flip](#api-12)
- [StringWaveletMatrix::count_equal](#api-13)
- [StringWaveletMatrix::count_less](#api-14)
- [StringWaveletMatrix::count_leq](#api-15)
- [StringWaveletMatrix::count_greater](#api-16)
- [StringWaveletMatrix::count_geq](#api-17)
- [StringWaveletMatrix::count_range](#api-18)
- [StringWaveletMatrix::count_prefix](#api-19)
- [StringWaveletMatrix::kth_smallest](#api-20)
- [StringWaveletMatrix::kth_largest](#api-21)
- [StringWaveletMatrix::min_key](#api-22)
- [StringWaveletMatrix::max_key](#api-23)
- [StringWaveletMatrix::lower_bound_key](#api-24)
- [StringWaveletMatrix::upper_bound_key](#api-25)
- [StringWaveletMatrix::prev_key](#api-26)
- [StringWaveletMatrix::next_key](#api-27)
- [StringWaveletMatrix::count_equal_id](#api-28)
- [StringWaveletMatrix::count_less_id](#api-29)
- [StringWaveletMatrix::count_leq_id](#api-30)
- [StringWaveletMatrix::count_greater_id](#api-31)
- [StringWaveletMatrix::count_geq_id](#api-32)
- [StringWaveletMatrix::can_pass](#api-33)

</details>

## 型・テンプレート引数

```cpp
class StringWaveletMatrix;
```

## この実装の注意点

名前はStringWaveletMatrixですが、区間[l,r)の行だけを検索するAPIではありません。検索は全行を対象とします。

<a id="api-1"></a>

## コンストラクタ

```cpp
StringWaveletMatrix();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
explicit StringWaveletMatrix(vector<string> s);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(NK)。

<a id="api-2"></a>

## StringWaveletMatrix::build

```cpp
void build(vector<string> s);
```

同じ幅の文字列列から再構築します。

**計算量**

- O(NK)。

<a id="api-3"></a>

## StringWaveletMatrix::size

```cpp
int size() const;
```

登録している文字列数を返します。

**計算量**

- O(1)。

<a id="api-4"></a>

## StringWaveletMatrix::rows

```cpp
int rows() const;
```

行IDの個数を返します。

**計算量**

- O(1)。

<a id="api-5"></a>

## StringWaveletMatrix::width

```cpp
int width() const;
```

各行の文字列幅Kを返します。

**計算量**

- O(1)。

<a id="api-6"></a>

## StringWaveletMatrix::get_key

```cpp
const string& get_key(int id) const;
```

行idの文字列をconst参照で返します。参照の取得自体はO(1)、コピーする場合はO(K)。

**計算量**

- O(1)。

<a id="api-7"></a>

## StringWaveletMatrix::get_bit

```cpp
int get_bit(int id, int p) const;
```

指定した行・桁のbitを0/1で返します。

**計算量**

- O(1)。

<a id="api-8"></a>

## StringWaveletMatrix::ones

```cpp
int ones(int id) const;
```

指定した行の1の個数を返します。

**計算量**

- O(1)。

<a id="api-9"></a>

## StringWaveletMatrix::zeros

```cpp
int zeros(int id) const;
```

指定した行の0の個数を返します。

**計算量**

- O(1)。

<a id="api-10"></a>

## StringWaveletMatrix::set_key

```cpp
void set_key(int id, string s);
```

行idの文字列をsへ変更します。

**計算量**

- O(K)。

<a id="api-11"></a>

## StringWaveletMatrix::set_bit

```cpp
void set_bit(int id, int p, int b);
```

行idの桁pをbit bへ変更します。

**制約**

- bは0または1

**計算量**

- O(K)。

<a id="api-12"></a>

## StringWaveletMatrix::flip

```cpp
void flip(int id, int p);
```

行idの桁pを反転します。

**計算量**

- O(K)。

<a id="api-13"></a>

## StringWaveletMatrix::count_equal

```cpp
int count_equal(string_view s) const;
```

key==s の個数

**計算量**

- O(K)。

<a id="api-14"></a>

## StringWaveletMatrix::count_less

```cpp
int count_less(string_view s) const;
```

key<s の個数

**計算量**

- O(K)。

<a id="api-15"></a>

## StringWaveletMatrix::count_leq

```cpp
int count_leq(string_view s) const;
```

key<=s の個数

**計算量**

- O(K)。

<a id="api-16"></a>

## StringWaveletMatrix::count_greater

```cpp
int count_greater(string_view s) const;
```

key>s の個数

**計算量**

- O(K)。

<a id="api-17"></a>

## StringWaveletMatrix::count_geq

```cpp
int count_geq(string_view s) const;
```

key>=s の個数

**計算量**

- O(K)。

<a id="api-18"></a>

## StringWaveletMatrix::count_range

```cpp
int count_range( string_view l, string_view r ) const;
```

l<=key<r の個数

**計算量**

- O(K)。

<a id="api-19"></a>

## StringWaveletMatrix::count_prefix

```cpp
int count_prefix(string_view p) const;
```

prefix が p の文字列の個数

**計算量**

- O(|p|)。

<a id="api-20"></a>

## StringWaveletMatrix::kth_smallest

```cpp
string kth_smallest(int k) const;
```

0-indexed k番目に小さい key

**計算量**

- O(K)。

<a id="api-21"></a>

## StringWaveletMatrix::kth_largest

```cpp
string kth_largest(int k) const;
```

0-indexed k番目に大きい key

**計算量**

- O(K)。

<a id="api-22"></a>

## StringWaveletMatrix::min_key

```cpp
optional<string> min_key() const;
```

全体の最小 key、空なら nullopt

**計算量**

- O(K)。

<a id="api-23"></a>

## StringWaveletMatrix::max_key

```cpp
optional<string> max_key() const;
```

全体の最大 key、空なら nullopt

**計算量**

- O(K)。

<a id="api-24"></a>

## StringWaveletMatrix::lower_bound_key

```cpp
optional<string> lower_bound_key(string_view s) const;
```

s以上の最小 key、なければ nullopt

**計算量**

- O(K)。

<a id="api-25"></a>

## StringWaveletMatrix::upper_bound_key

```cpp
optional<string> upper_bound_key(string_view s) const;
```

sより大きい最小 key、なければ nullopt

**計算量**

- O(K)。

<a id="api-26"></a>

## StringWaveletMatrix::prev_key

```cpp
optional<string> prev_key(string_view s) const;
```

s未満の最大 key、なければ nullopt

**計算量**

- O(K)。

<a id="api-27"></a>

## StringWaveletMatrix::next_key

```cpp
optional<string> next_key(string_view s) const;
```

s以上の最小 key、なければ nullopt

**計算量**

- O(K)。

<a id="api-28"></a>

## StringWaveletMatrix::count_equal_id

```cpp
int count_equal_id(int id) const;
```

key==key[i] の個数

**計算量**

- O(K)。

<a id="api-29"></a>

## StringWaveletMatrix::count_less_id

```cpp
int count_less_id(int id) const;
```

key<key[i] の個数

**計算量**

- O(K)。

<a id="api-30"></a>

## StringWaveletMatrix::count_leq_id

```cpp
int count_leq_id(int id) const;
```

key<=key[i] の個数

**計算量**

- O(K)。

<a id="api-31"></a>

## StringWaveletMatrix::count_greater_id

```cpp
int count_greater_id(int id) const;
```

key>key[i] の個数

**計算量**

- O(K)。

<a id="api-32"></a>

## StringWaveletMatrix::count_geq_id

```cpp
int count_geq_id(int id) const;
```

key>=key[i] の個数

**計算量**

- O(K)。

<a id="api-33"></a>

## StringWaveletMatrix::can_pass

```cpp
bool can_pass(int id, int M) const;
```

行idが全0でなく、その値以上の行数がM以下ならtrue。特定の条件判定用の補助APIです。

**計算量**

- O(K)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
StringWaveletMatrix w(vector<string>{"001","101","001"});
assert(w.count_equal("001")==2 && w.kth_smallest(0)=="001");
w.flip(0,0); assert(w.get_key(0)=="101" && w.count_equal("101")==2);
```

## メモリ

O(NK)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
