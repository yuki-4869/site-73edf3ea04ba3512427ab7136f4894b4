# ビット盤面（横64列まで）

盤面を1行1ワードで保持し、行をまとめてシフトして4近傍を展開します。幅が64以下の盤面で、複数マスの隣接マスや通行可能な到達領域を扱えます。

## API

| API | 返り値・使い方 | 計算量 |
| --- | --- | --- |
| `ahc::BitGrid(H,W)` | ビットが全て0の盤面。 `0 <= W <= 64` | O(H) |
| `grid.height()`, `grid.width()` | 高さ・幅 | O(1) |
| `grid.row(r)`, `grid.set_row(r,bits)` | 行の `uint64_t` を取得・設定 | O(1) |
| `grid.test(r,c)`, `grid.set(r,c,value=true)` | 1マスを取得・設定 | O(1) |
| `grid.count()` | 1のマス数 | O(H) |
| `grid.neighbors4()` | 各1マスから1手で移動できるマスの集合 | O(H) |
| `grid.flood(allowed)` | ビットが1のマスを始点に、 `allowed` の1だけを通って到達できる全マス | O(1+H(D+1)) |

行・列は盤内の0-indexedです。列0が最下位ビットです。 `set_row` は盤外のビットを捨てます。 `flood` の2盤面は同じサイズとし、許可されていない始点は除きます。Dは有効な始点集合からの最大最短距離で、到達点が空なら0です。メモリはO(H)です。距離や経路が必要な場合や縦長の通路では [グリッドBFS](grid_bfs.md) が適しています。

## 使用例

```cpp
ahc::BitGrid start(2,4), allowed(2,4);
start.set(0,0);
allowed.set_row(0,0b1011);
allowed.set_row(1,0b1111);
auto reached = start.flood(allowed);
assert(reached.count() == 7);
```
