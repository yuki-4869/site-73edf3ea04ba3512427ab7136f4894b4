# 最小 Steiner 木

## API

`minimum_steiner_tree(n, edges, terminals)` は `optional<SteinerTreeResult>` を返します。`edges` は `vector<tuple<int,int,long long>>` で、順に無向辺の端点と重みです。`terminals` は指定頂点の `vector<int>` です。頂点・辺 ID は 0 始まりです。

| メンバ | 内容 |
| --- | --- |
| `result->weight` | 最小重みの `long long` |
| `result->edge_ids` | 採用した入力辺 ID の `vector<int>` |

辺重みは非負。指定頂点は重複除外後で 20 個以下です。0 個・1 個なら重み 0。全てを連結できない場合や最小重みが `LLONG_MAX` 以上の場合は `nullopt` です。`codecf` では `<atcoder/dsu>` を追加します。

## 計算量

指定頂点の集合ごとに、同じ頂点を根とする部分解をマージし、多始点 Dijkstra で根を移動します。指定頂点数を $K$ とすると時間 $O(3^K N+2^K(N+M)\log(N+M+2))$、領域 $O(2^K N+M)$ です。

## 使用例

```cpp
auto result=minimum_steiner_tree(4,{{0,3,1},{1,3,1},{2,3,1},{0,1,9}}, {0,1,2});
if(!result || result->weight!=3 || result->edge_ids.size()!=3)std::abort();
```

[Library Checker の対応問題](https://judge.yosupo.jp/problem/minimum_steiner_tree)
