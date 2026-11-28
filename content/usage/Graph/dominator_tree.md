# 支配木（Dominator Tree）

指定した始点から頂点 $v$ に至るすべての経路に頂点 $u$ が含まれるとき、$u$ は $v$ を支配します。各頂点の、自身以外で最も近い支配頂点を求めます。

## API

```cpp
std::vector<int> dominator_tree(const std::vector<std::vector<int>>& graph, int root);
```

有向グラフの 0 始まりの隣接リストを渡します。返り値の `result[v]` が支配木上の親です。根の値は根自身、到達できない頂点は `-1` です。自己ループや多重辺も扱えます。空でないグラフを使用し、頂点数は `INT_MAX` 未満です。

Lengauer–Tarjan 法の単純版を使います。DFSと経路圧縮はともに反復処理です。頂点数 $N$、辺数 $M$ に対して、時間 $O((N+M)\log(N+1))$、追加メモリ $O(N+M)$ です。

## 使用例

```cpp
std::vector<std::vector<int>> g={{1,2},{3},{3},{},{}};
auto parent=dominator_tree(g,0);
if(parent!=std::vector<int>{0,0,0,0,-1})std::abort();
```

頂点 3 には 1 を通る経路と 2 を通る経路があるため、直近支配頂点は 0 です。

[Library Checker・支配木（Dominator Tree）](https://judge.yosupo.jp/problem/dominatortree)
