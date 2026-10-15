# SCCと縮約DAG

[説明書の目次](../index.md) · [コード](../../../Graph/scc_condensation.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Graph/scc_condensation.cpp)

有向グラフを強連結成分に分解し、成分を頂点とする縮約DAGも返します。DAG上のDPや閉路判定に使用できます。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=g.size(), M=辺数, K=成分数。u,v は元の頂点番号、c は成分番号。

**共通の前提**

- g は隣接リスト。辺の行き先は 0..N-1。自己ループ・多重辺・非連結グラフに対応。入力gは変更しません。

## 型・テンプレート引数

```cpp
struct SCCResult;
```

## SCCResult::same

```cpp
bool same(ll u, ll v) const;
```

頂点u,vが同じ強連結成分かを返します。

**制約**

- 0<=u,v<N

**計算量**

- O(1)。

## SCCResult::size

```cpp
ll size(ll c) const;
```

成分cの頂点数を返します。

**制約**

- 0<=c<K

**計算量**

- O(1)。

## SCCResult::sources

```cpp
vl sources() const;
```

縮約DAGで入次数0の成分番号を返します。

**計算量**

- O(K)。

## SCCResult::sinks

```cpp
vl sinks() const;
```

縮約DAGで出次数0の成分番号を返します。

**計算量**

- O(K)。

## SCCResult::in_cycle

```cpp
bool in_cycle(ll v) const;
```

頂点vが有向閉路上にあるかを返します。1頂点成分の自己ループも含みます。

**制約**

- 0<=v<N

**計算量**

- O(1)。

## SCCResult::has_cycle

```cpp
bool has_cycle() const;
```

少なくとも1つの有向閉路が存在するかを返します。

**計算量**

- O(K)。

## SCCDecomposition

```cpp
SCCResult SCCDecomposition(const graph& g);
```

gを分解しSCCResultを返します。各頂点はちょうど1成分に属し、異なる成分を結ぶ辺u→vについて cmp[u]<cmp[v] です。成分内の頂点順は保証しません。

**計算量**

- O(N+M)。

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `K` | 成分数 |
| `cmp[v]` | 所属成分番号 |
| `groups[c]` | 成分cの頂点列 |
| `dag[c]` | 縮約DAGの隣接リスト（成分間の多重辺を除去） |
| `rdag[c]` | 逆向きの隣接リスト |
| `indeg[c],outdeg[c]` | 縮約後の入次数・出次数 |
| `cyclic[c]` | 成分が閉路を含むか |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
graph g(4);
g[0]={1}; g[1]={0,2}; g[2]={3};
auto s=SCCDecomposition(g);
assert(s.K==3 && s.same(0,1) && !s.same(1,2));
assert(s.cmp[1]<s.cmp[2] && s.cmp[2]<s.cmp[3]);
assert(s.in_cycle(0) && !s.in_cycle(2));
```

## メモリ

O(N+M)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
