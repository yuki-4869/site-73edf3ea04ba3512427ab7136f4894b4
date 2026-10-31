# SCCと縮約DAG

[説明書の目次](../index.md) · [コード](../../../Graph/scc_condensation.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/641047791e5091b9805c4f79b3901c30d73b2d2c/Graph/scc_condensation.cpp)

有向グラフを強連結成分に分解し、成分を頂点とする縮約DAGも返します。重み付き版では、各SCCに辺重みの総和が0でない有向閉路が存在するかも O(N+M) で判定します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=g.size(), M=辺数, K=成分数。u,v は元の頂点番号、c は成分番号。

**共通の前提**

- g は隣接リスト。辺の行き先は 0..N-1。自己ループ・多重辺・非連結グラフに対応。入力gは変更しません。
- 無重みは `graph=vector<vector<ll>>`、重み付きは `Graph=vector<vector<pair<ll,ll>>>` です。重み付きの辺は `{行き先,重み}` で、重みは負・0・正を含む `ll` の全範囲に対応します。

## 型・テンプレート引数

```cpp
struct SCCResult;
struct WeightedSCCResult;
```

`WeightedSCCResult` は以下の `SCCResult` のフィールドと取得関数をすべて引き継ぎ、`nonzero_cycle` と `has_nonzero_cycle()` を追加します。

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
WeightedSCCResult SCCDecomposition(const Graph& g);
```

gを分解しSCCResultを返します。各頂点はちょうど1成分に属し、異なる成分を結ぶ辺u→vについて cmp[u]<cmp[v] です。成分内の頂点順は保証しません。

重み付きの `Graph` を渡すと `WeightedSCCResult` を返します。SCC分解は重みを無視して行い、各成分内で非0閉路の有無を追加判定します。`dag` と `rdag` は両方の版で重みを持たない `graph` であり、成分間の多重辺を除去します。

空グラフでは `K=0` で、結果の配列はすべて空です。空グラフを直接指定する場合は `SCCDecomposition(graph{})` または `SCCDecomposition(Graph{})` のように型を明示します。

**計算量**

- どちらの版も O(N+M)。追加判定を含む重み付き版も同じです。

## WeightedSCCResult::has_nonzero_cycle

```cpp
bool has_nonzero_cycle() const;
```

グラフ全体に、辺重みの総和が0でない有向閉路が少なくとも1つ存在するかを返します。総和が正・負のどちらの場合も `true` になり、両者の区別はしません。空グラフでは `false` です。

**計算量**

- O(K)。

## 非0閉路の判定

`nonzero_cycle[c]` は、SCC c の中に辺重みの総和が0でない有向閉路が存在するかを表します。参照は O(1) です。重み0の自己ループだけなら `false`、重みが非0の自己ループがあれば `true` です。

各SCC内で1頂点のポテンシャルを0にし、BFSで未訪問の頂点へ `p[v]=p[u]+w` を割り当てます。訪問済みの頂点に向かう辺でこの等式に矛盾があれば、そのSCCに非0閉路があります。成分をまたぐ辺は判定に使いません。

- すべての内部辺で等式が成立するなら、閉路に沿って差分を足すと重みの和は0になります。
- 矛盾する辺があるなら、SCC内の帰り道を使って始点へ戻ることで、重みの異なる2つの閉じた歩道を作れます。その少なくとも一方は重みの和が非0であり、非0の単純閉路を含みます。

ポテンシャルは `__int128` で計算するため、経路の重みの和が `ll` の範囲を超えても判定できます。

`nonzero_cycle[cmp[v]]` は「頂点vが属するSCCに非0閉路があるか」です。頂点v自身が非0の単純閉路上にあることまでは意味しません。

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
| `nonzero_cycle[c]` | 重み付き版のみ。成分に辺重みの総和が非0の閉路があるか |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
graph g(4);
g[0]={1}; g[1]={0,2}; g[2]={3};
auto s=SCCDecomposition(g);
assert(s.K==3 && s.same(0,1) && !s.same(1,2));
assert(s.cmp[1]<s.cmp[2] && s.cmp[2]<s.cmp[3]);
assert(s.in_cycle(0) && !s.in_cycle(2));

Graph weighted(4);
weighted[0]={{1,2}};
weighted[1]={{0,-2},{2,100}};
weighted[2]={{3,4}};
weighted[3]={{2,-3}};
auto ws=SCCDecomposition(weighted);
assert(ws.same(0,1) && ws.same(2,3));
assert(ws.cyclic[ws.cmp[0]] && !ws.nonzero_cycle[ws.cmp[0]]);
assert(ws.nonzero_cycle[ws.cmp[2]] && ws.has_nonzero_cycle());
```

## メモリ

O(N+M)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
