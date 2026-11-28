# 平面上の最遠点対

## API

`furthest_pair(points)` に点列 `vector<pll>` を渡すと、`FurthestPairResult` が返ります。

| メンバ | 内容 |
| --- | --- |
| `first`, `second` | 最遠の 2 点の元の入力順の添字 `int` |
| `distance_squared` | 最大ユークリッド距離の二乗 `__int128` |

各座標成分は絶対値が $10^{18}$ 以下の整数。重複座標を含んでも使えます。点が 2 個未満なら添字は両方 `-1`、距離二乗は 0 です。先に [凸包](convex_hull.md) を貼ります。

## 計算量

凸包を構築し、回転キャリパーで対蹠点を調べます。時間 $O(N\log(N+2))$、領域 $O(N)$ です。

## 使用例

```cpp
auto result=furthest_pair({{0,0},{1,0},{4,3},{0,0}});
if(result.distance_squared!=25 || result.first==result.second)std::abort();
```

[Library Checker の対応問題](https://judge.yosupo.jp/problem/furthest_pair)
