# グリッド多始点BFS

4近傍のグリッドで、始点集合から各マスへの最短距離を求め、経路を `URDL` の操作列として復元します。壁つき文字盤面のほか、向きに依存する通行判定も使えます。

## API

| API | 返り値・使い方 | 計算量 |
| --- | --- | --- |
| `ahc::GridBfs(H,W)` | 盤面サイズを設定。 `Cell` は `pair<int,int>` | O(HW) |
| `bfs.inside(cell)` | 盤内なら `true` | O(1) |
| `bfs.run(starts,can_step)` | `span<const Cell>` と `bool can_step(Cell from,Cell to)` で探索 | O(HW+S) |
| `bfs.run_board(board,starts,wall='#')` | `vector<string>` 上で壁を避けて探索 | O(H+HW+S) |
| `bfs.prepare_board(board,wall='#')` | 固定盤面の通行方向を前計算 | O(H+HW) |
| `bfs.run_prepared(starts)` | 前計算した盤面で探索 | O(HW+S) |
| `bfs.dist(cell)` | 距離。未探索・到達不能なら `-1` | O(1) |
| `bfs.path(cell)` | 最短経路の `optional<string>`。到達不能なら `nullopt`、始点なら空文字列 | O(1+経路長) |

座標は0-indexedの（行,列）です。縦横ともに非負で、 `H*W <= INT_MAX` とします。Sは始点数、 `can_step` はO(1)、探索領域はO(HW)です。文字盤面は長方形で、始点は壁以外に置きます。辺のコストはすべて1です。同距離の場合は始点の登録順とURDLの探索順で経路が決まります。同じ盤面サイズなら再実行できます。

## 使用例

```cpp
vector<string> board{"...",".#.","..."};
ahc::GridBfs bfs(3,3);
array<ahc::GridBfs::Cell,1> starts{{{0,0}}};
bfs.prepare_board(board);
bfs.run_prepared(starts);
assert(bfs.dist({2,2}) == 4);
assert(bfs.path({2,2}) == "RRDD");
```

固定盤面を何度も探索するときは、最初に `prepare_board` を1回だけ呼び、始点を変えて `run_prepared` を呼びます。1マスにつき1バイトの通行方向マスクを使い、探索中の盤内判定と文字参照を省きます。盤面の壁が変わったら前計算をやり直します。前計算だけでは、前回の距離や経路は更新されません。通行方向をコールバックで指定する場合は `run(starts,can_step)` を使います。
