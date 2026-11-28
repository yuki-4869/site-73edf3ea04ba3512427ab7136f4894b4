# 2次元Fenwick木

グリッドに1点加算しながら、長方形の和を何度も取得します。加減算できる型であれば、整数・modintなどを使えます。

## API

| API | 返り値・使い方 | 計算量 |
| --- | --- | --- |
| `ahc::GridFenwick<T>(H,W)` | すべて0で構築 | O(1+HW) |
| `ahc::GridFenwick(initial)` | 長方形の `vector<vector<T>>` から構築 | O(1+H+HW) |
| `fw.height()`, `fw.width()` | 高さと幅 | O(1) |
| `fw.add(r,c,delta)` | 1点加算 | O(log(H+1)log(W+1)) |
| `fw.prefix_sum(r,c)` | `[0,r) × [0,c)` の和 | O(1+log(H+1)log(W+1)) |
| `fw.sum(top,left,bottom,right)` | `[top,bottom) × [left,right)` の和 | O(1+log(H+1)log(W+1)) |

座標は0-indexedで、和を取る長方形は半開区間です。空の長方形の和は0です。初期配列が空のときは0×0として構築します。型Tには0と加減算が必要で、中間値もTに収めます。領域はO(1+HW)です。

## 使用例

```cpp
ahc::GridFenwick fw(vector<vector<long long>>{{1,2},{3,4}});
fw.add(0,1,10);
assert(fw.sum(0,1,2,2) == 16);
assert(fw.prefix_sum(2,2) == 20);
```
