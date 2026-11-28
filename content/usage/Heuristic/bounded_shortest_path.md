# 0-1 BFS・Dial法

辺重みが小さい非負整数の有向グラフで、多始点最短路を求めます。重み上限をCとして、C+1個の循環バケットを使います。辺重みが0か1のときは頂点数と辺数に線形です。

## API

| API | 返り値・使い方 | 計算量 |
| --- | --- | --- |
| `ahc::dial_shortest_paths(edges,starts,max_edge)` | 距離と親を持つ `BoundedShortestPaths` | O(N+M+S+NC) |
| `ahc::zero_one_bfs(edges,starts)` | 0-1重みの上記操作 | O(N+M+S) |
| `result.distance[v]` | 最短距離の `long long`。未到達なら-1 | O(1) |
| `result.parent[v]` | 最短経路上の直前頂点。始点と未到達は-1 | O(1) |
| `result.path(v)` | 最短経路の `optional<vector<int>>`。未到達は `nullopt` | O(1+経路長) |

頂点は0-indexedで、NはINT_MAX以下です。 `edges[u]` は `vector<pair<int,int>>` で、要素は（行き先, コスト）です。無向辺は両方向に追加します。重みは `[0,max_edge]` で、0-1 BFSでは0か1を渡します。 `starts` は始点集合で、Sは列の長さです。同距離経路の選択順は指定しません。使用領域は入力を除きO(N+M+C)です。大きなコスト上限では基本テンプレートのDijkstraが適しています。

## 使用例

```cpp
vector<vector<pair<int,int>>> edges{{{1,1},{2,0}},{},{{1,0}}};
array<int,1> starts{0};
auto result = ahc::zero_one_bfs(edges,starts);
assert(result.distance[1] == 0);
assert(*result.path(1) == (vector<int>{0,2,1}));
```
