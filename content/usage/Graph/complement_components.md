# 補グラフの連結成分

## API

`complement_components(n, edges)` は元の無向グラフの辺を `vector<pair<int,int>>` で受け取り、補グラフの連結成分を `vector<vector<int>>` で返します。外側の要素が各成分、内側がその頂点です。

頂点は 0 始まり。自己ループは無視し、多重辺にも対応します。成分内・成分間の順序に依存しない使い方をしてください。

## 計算量

未訪問頂点のリストから元のグラフで隣接しない頂点を取り除きながら BFS します。リストで頂点を残す処理は元の辺に対応するため、時間と領域はともに $O(N+M)$ です。

## 使用例

```cpp
auto components=complement_components(3,{{0,1},{0,2}});
if(components!=std::vector<std::vector<int>>{{0},{1,2}})std::abort();
```

[Library Checker の対応問題](https://judge.yosupo.jp/problem/connected_components_of_complement_graph)
