# Binary Trie

[説明書の目次](../index.md) · [コード](../../../DataStructureBasic/binary_trie.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/DataStructureBasic/binary_trie.cpp)

重複を許す非負整数集合を二進Trieで管理し、XOR最小／最大や順位取得を行います。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

B=60で、bit60からbit0までの61段を使用。kは0-indexed。

**共通の前提**

- 0<=x<2^61。erase(x)はcount(x)>0のとき。min_xor/max_xorはsize()>0のとき。kthは0<=k<size()。

<details>
<summary>API一覧</summary>

- [BinaryTrie::insert](#api-1)
- [BinaryTrie::erase](#api-2)
- [BinaryTrie::count](#api-3)
- [BinaryTrie::min_xor](#api-4)
- [BinaryTrie::max_xor](#api-5)
- [BinaryTrie::kth](#api-6)
- [BinaryTrie::xor_all](#api-7)
- [BinaryTrie::size](#api-8)
- [BinaryTrie::child](#api-9)
- [BinaryTrie::node_count](#api-10)

</details>

## 型・テンプレート引数

```cpp
struct BinaryTrie;
```

<a id="api-1"></a>

## BinaryTrie::insert

```cpp
void insert(ll x);
```

xを1個追加します。重複可。

**計算量**

- O(61)。

<a id="api-2"></a>

## BinaryTrie::erase

```cpp
void erase(ll x);
```

xを1個削除します。ノード自体は回収しません。

**計算量**

- O(61)。

<a id="api-3"></a>

## BinaryTrie::count

```cpp
ll count(ll x) const;
```

xの個数を返す

**計算量**

- O(logA)。

<a id="api-4"></a>

## BinaryTrie::min_xor

```cpp
ll min_xor(ll x) const;
```

登録値vに対する最小の(v xor x)を返します。v自体ではありません。

**計算量**

- O(61)。

<a id="api-5"></a>

## BinaryTrie::max_xor

```cpp
ll max_xor(ll x) const;
```

登録値vに対する最大の(v xor x)を返します。

**計算量**

- O(61)。

<a id="api-6"></a>

## BinaryTrie::kth

```cpp
ll kth(ll k) const;
```

は0<=k<size()を想定

**計算量**

- O(61)。

<a id="api-7"></a>

## BinaryTrie::xor_all

```cpp
void xor_all(ll x);
```

全登録値をv xor xに変更します。

**計算量**

- O(1)。

<a id="api-8"></a>

## BinaryTrie::size

```cpp
ll size() const;
```

登録されている要素数を返す

**計算量**

- O(1)。

<a id="api-9"></a>

## BinaryTrie::child

```cpp
ll child(ll v, ll bit) const;
```

保存時のbitの子ノードを返し、現存要素がなければ-1。全体XOR後の論理値とは桁の意味が異なります。

**制約**

- bitは0または1

**計算量**

- O(1)。

<a id="api-10"></a>

## BinaryTrie::node_count

```cpp
ll node_count(ll v) const;
```

ノードv以下の要素数。v=-1なら0。

**計算量**

- O(1)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
BinaryTrie bt; bt.insert(5); bt.insert(10);
assert(bt.min_xor(7)==2 && bt.kth(0)==5);
bt.xor_all(3); assert(bt.count(6)==1); bt.erase(6);
assert(bt.size()==1);
```

## メモリ

O(61×作成した経路数)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
