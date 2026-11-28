# 巡回路の交換・挿入近傍

閉路を順序列で表し、交換または1点移動で変更される辺だけを計算します。移動方向によってコストが異なる場合にも使えます。対称コストの区間反転は [巡回路2-opt](two_opt.md) で扱えます。

## API

| API | 返り値・使い方 | 計算量 |
| --- | --- | --- |
| `ahc::tour_swap_delta(tour,i,j,distance)` | 添字 `i,j` の交換による「変更後 − 変更前」 | O(1) |
| `ahc::tour_relocate_delta(tour,from,after,distance)` | 1点移動による「変更後 − 変更前」 | O(1) |
| `ahc::tour_relocate(tour,from,after)` | `from` の頂点を `after` の頂点の直後へ移す | O(1+差の絶対値) |

添字はすべて変更前の列に対する0-indexedです。閉路の末尾に始点を重複させません。 `after` が `from` または閉路上の直前の位置なら差分は0で、操作は列を変更しません。交換の適用には `std::swap` を使います。費用関数 `distance(u,v)` は定数時間を想定し、費用の中間和と差分が収まる符号付き型を返します。

## 使用例

```cpp
vector<int> tour{0,1,2,3};
auto cost = [](int u,int v) -> long long { return 1LL*u*v*v; };
assert(ahc::tour_relocate_delta(tour,1,2,cost) == -11);
ahc::tour_relocate(tour,1,2);
assert(tour == (vector<int>{0,2,1,3}));
```
