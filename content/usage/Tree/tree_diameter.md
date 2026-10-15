# 木の直径・最遠距離

[説明書の目次](../index.md) · [コード](../../../Tree/tree_diameter.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Tree/tree_diameter.cpp)

木の直径・中心・各頂点からの最遠距離を前計算します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

TreeDiameterは無重み、WeightedTreeDiameterは重み付き。N=頂点数。

**共通の前提**

- gは無向木または空。重み付きGraphは隣接リストのpairが{行き先,辺重み}。重みは非負。距離・累積和はllに収めます。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [TreeDiameter::build](#api-2)
- [TreeDiameter::get_farthest_dist](#api-3)
- [TreeDiameter::get_farthest_vertex](#api-4)
- [TreeDiameter::on_diameter](#api-5)
- [WeightedTreeDiameter::build](#api-6)
- [WeightedTreeDiameter::get_farthest_dist](#api-7)
- [WeightedTreeDiameter::get_farthest_vertex](#api-8)
- [WeightedTreeDiameter::on_diameter](#api-9)

</details>

## 型・テンプレート引数

```cpp
struct TreeDiameter;
struct WeightedTreeDiameter;
```

<a id="api-1"></a>

## コンストラクタ

```cpp
TreeDiameter();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
TreeDiameter(const graph& g);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

```cpp
WeightedTreeDiameter();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
WeightedTreeDiameter(const Graph& g);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

<a id="api-2"></a>

## TreeDiameter::build

```cpp
void build(const graph& g);
```

指定された木から全情報を再計算します。

**計算量**

- O(N)。

<a id="api-3"></a>

## TreeDiameter::get_farthest_dist

```cpp
ll get_farthest_dist(ll v) const;
```

vから最も遠い頂点までの距離を返します。

**制約**

- 0<=v<N

**計算量**

- O(1)。

<a id="api-4"></a>

## TreeDiameter::get_farthest_vertex

```cpp
ll get_farthest_vertex(ll v) const;
```

vから最も遠い頂点を1つ返します。

**制約**

- 0<=v<N

**計算量**

- O(1)。

<a id="api-5"></a>

## TreeDiameter::on_diameter

```cpp
bool on_diameter(ll v) const;
```

vが今回選ばれた直径のpathに含まれるかを返します。

**制約**

- 0<=v<N

**計算量**

- O(1)。

<a id="api-6"></a>

## WeightedTreeDiameter::build

```cpp
void build(const Graph& g);
```

指定された木から全情報を再計算します。

**計算量**

- O(N)。

<a id="api-7"></a>

## WeightedTreeDiameter::get_farthest_dist

```cpp
ll get_farthest_dist(ll v) const;
```

vから最も遠い頂点までの距離を返します。

**制約**

- 0<=v<N

**計算量**

- O(1)。

<a id="api-8"></a>

## WeightedTreeDiameter::get_farthest_vertex

```cpp
ll get_farthest_vertex(ll v) const;
```

vから最も遠い頂点を1つ返します。

**制約**

- 0<=v<N

**計算量**

- O(1)。

<a id="api-9"></a>

## WeightedTreeDiameter::on_diameter

```cpp
bool on_diameter(ll v) const;
```

vが今回選ばれた直径のpathに含まれるかを返します。

**制約**

- 0<=v<N

**計算量**

- O(1)。

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `diameter` | 直径の長さ |
| `s,t` | 直径の両端 |
| `path` | s→tの頂点列 |
| `dist_s[v],dist_t[v]` | 両端からの距離 |
| `ecc[v]` | vからの最遠距離 |
| `farthest[v]` | 最遠頂点の1つ |
| `radius` | 頂点を中心とする最小の最遠距離 |
| `centers` | その半径を達成する頂点 |
| `diameter_pos[v]` | 今回選んだ直径上の位置、直径外は-1 |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
graph g={{1},{0,2},{1}}; TreeDiameter t(g);
assert(t.diameter==2 && t.get_farthest_dist(1)==1);
assert(t.centers==vl{1});
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
