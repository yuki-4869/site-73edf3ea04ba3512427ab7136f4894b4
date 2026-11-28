# 最近傍法・巡回路2-opt

最近傍法で初期巡回路を作り、区間反転で改善します。対称コストなら区間内の辺は反転前後で費用が同じなので、区間の両端につながる2辺だけで反転の差分を計算できます。

## API

| API | 返り値・使い方 | 計算量 |
| --- | --- | --- |
| `ahc::nearest_neighbor_tour(n,start,distance)` | `0..n-1` を巡回する `vector<int>`。同距離では小さい頂点を選ぶ | O(N²) |
| `ahc::cycle_cost(tour,distance)` | 最後から先頭に戻る辺も含めた費用。0～1頂点は0 | O(N) |
| `ahc::two_opt_delta(tour,l,r,distance)` | `[l,r)` 反転の「変更後 − 変更前」を返す | O(1) |
| `ahc::two_opt_first_improvement(tour,distance,epsilon=0)` | 先頭を固定し、費用が `epsilon` より大きく減る反転を最初に見つけて適用。改善したら `true` | 最悪O(N²) |

巡回路の末尾に始点を重複させません。反転操作は対称コスト専用で、 `0 <= l <= r <= N` の半開区間です。 `distance(u,v)` はここではO(1)とし、中間和と差分に足りる符号付き型で返します。浮動小数の値は有限とし、 `epsilon` は非負です。N=0の最近傍法では `start=0` を渡します。反転を自分で適用する場合はO(r-l)です。

## 使用例

```cpp
vector<pair<int,int>> points{{0,0},{4,0},{0,4},{4,4}};
auto distance = [&](int u,int v) -> long long {
    return llabs(1LL*points[u].first-points[v].first)
         + llabs(1LL*points[u].second-points[v].second);
};
auto tour = ahc::nearest_neighbor_tour(points.size(), 0, distance);
while (ahc::two_opt_first_improvement(tour, distance)) {}
assert(ahc::cycle_cost(tour, distance) == 16);
```
