# 動的2次元空間バケット

平面を正方形のバケットに区切り、点のIDを保持します。動く点が多く、狭い長方形の中にある点を繰り返し取得するときに使えます。

## API

| API | 返り値・使い方 | 計算量 |
| --- | --- | --- |
| `ahc::SpatialBuckets2D(universe,bucket_size)` | 空の点集合を構築。辺長は正 | O(universe) |
| `points.get(id)` | `optional<Point>`。未登録で `nullopt` | O(1) |
| `points.set(id,x,y)` | 点を登録、または既存の点を移動 | 償却期待O(1) |
| `points.erase(id)` | 点を削除し、登録済みなら `true` | 期待O(1) |
| `points.query_box(x0,y0,x1,y1)` | `[x0,x1) × [y0,y1)` 内のID列 | 期待O(1+B+K) |

`Point` は `pair<int,int>` です。IDは `[0,universe)`、負座標も使えます。異なるIDを同じ座標に登録できます。長方形端点は `long long` なので、座標 `INT_MAX` を含めるには右端を `1LL+INT_MAX` にできます。出力順は任意です。Bは調べるバケット数、Kは範囲をチェックする点数です。箱が広く、交差するバケット数が登録バケット数を超えるときは全登録点を走査します。使用領域はO(universe)です。

## 使用例

```cpp
ahc::SpatialBuckets2D points(10,8);
points.set(4,-2,3);
points.set(7,12,3);
assert(points.query_box(-4,0,0,6) == vector<int>{4});
points.set(4,16,9);
assert(points.query_box(-4,0,0,6).empty());
```
