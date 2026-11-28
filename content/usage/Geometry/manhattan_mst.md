# マンハッタン最小全域木

平面上の点を頂点とし、2点の距離 $|x_i-x_j|+|y_i-y_j|$ を辺の重みとする完全グラフから、最小全域木を1つ求めます。

## API

```cpp
ManhattanMSTResult manhattan_mst(const std::vector<std::pair<long long,long long>>& points);
```

| メンバ | 内容 |
| --- | --- |
| `weight` | 木全体の重みを `__int128` で返す |
| `edges` | 採用した辺の端点 `pair<int,int>` を列挙する |

点の番号は入力順の 0 始まりです。点が 0 個または 1 個なら重みは 0 で、辺は空です。同じ座標を持つ点も、別々の頂点として扱います。座標は `long long` の全範囲、点数は `INT_MAX` 以下です。`codecf` では `<atcoder/dsu>` を追加します。

座標の変換と掃き出しで 8 方向の最近点から高々 $O(N)$ 本の候補辺を作り、Kruskal 法を実行します。時間は $O(N\log(N+1))$、追加メモリは $O(N)$ です。

## 使用例

```cpp
auto tree=manhattan_mst({{0,0},{2,0},{2,1}});
if(tree.weight!=3 || tree.edges.size()!=2)std::abort();
```

[Library Checker・マンハッタン最小全域木](https://judge.yosupo.jp/problem/manhattanmst)
