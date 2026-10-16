# Functional Graph

[説明書の目次](../index.md) · [コード](../../../Graph/functional_graph.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Graph/functional_graph.cpp)

各頂点の出次数が1である有向グラフについて、到達するサイクルとそこまでの距離を前計算します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=to.size()。v は頂点番号。

**共通の前提**

- to は vector<ll>。0<=to[v]<N。頂点vは0-indexed。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [FunctionalGraph::build](#api-2)
- [FunctionalGraph::is_cycle](#api-3)
- [FunctionalGraph::get_cycle_id](#api-4)
- [FunctionalGraph::get_dist](#api-5)
- [FunctionalGraph::get_root](#api-6)
- [FunctionalGraph::get_cycle_len](#api-7)
- [FunctionalGraph::get_cycle_pos](#api-8)

</details>

## 型・テンプレート引数

```cpp
struct FunctionalGraph;
```

## この実装の注意点

末尾コメントは実装済みのAPIと一致するように整理済みです。サイクル情報をO(N)で前計算し、k回遷移には別ファイルのBinaryLiftingを使用します。

<a id="api-1"></a>

## コンストラクタ

```cpp
FunctionalGraph(const vl& _to);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

<a id="api-2"></a>

## FunctionalGraph::build

```cpp
void build(const vl& _to);
```

toから全情報を再構築します。

**計算量**

- O(N)。

<a id="api-3"></a>

## FunctionalGraph::is_cycle

```cpp
bool is_cycle(ll v);
```

v自身がサイクル上にあるかを返します。

**計算量**

- O(1)。

<a id="api-4"></a>

## FunctionalGraph::get_cycle_id

```cpp
ll get_cycle_id(ll v);
```

vから進んで最終的に入るサイクルの番号を返します。

**計算量**

- O(1)。

<a id="api-5"></a>

## FunctionalGraph::get_dist

```cpp
ll get_dist(ll v);
```

vから初めてサイクルに入るまでの辺数を返します。

**計算量**

- O(1)。

<a id="api-6"></a>

## FunctionalGraph::get_root

```cpp
ll get_root(ll v);
```

vから初めて到達するサイクル上の頂点を返します。

**計算量**

- O(1)。

<a id="api-7"></a>

## FunctionalGraph::get_cycle_len

```cpp
ll get_cycle_len(ll v);
```

vが到達するサイクルの長さを返します。

**計算量**

- O(1)。

<a id="api-8"></a>

## FunctionalGraph::get_cycle_pos

```cpp
ll get_cycle_pos(ll v);
```

到達先のサイクル入口が、そのサイクル内で何番目かを返します。

**計算量**

- O(1)。

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `to[v]` | 次の頂点 |
| `on_cycle[v]` | サイクル上か |
| `cycle_id[v]` | 到達するサイクルのID |
| `cycle_len[c]` | サイクルcの長さ |
| `dist[v]` | サイクルまでの距離 |
| `root_cycle[v]` | 入口の頂点 |
| `root_pos[v]` | 入口のサイクル内位置 |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
vl to={1,2,3,4,5,2,1};
FunctionalGraph fg(to);
assert(fg.get_dist(0)==2);
assert(fg.get_root(0)==2);
assert(fg.get_cycle_len(0)==4);
assert(fg.is_cycle(2) && !fg.is_cycle(0));
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
