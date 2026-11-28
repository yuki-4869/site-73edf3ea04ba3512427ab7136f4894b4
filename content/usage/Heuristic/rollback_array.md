# 巻き戻せる配列

近傍を試す前に履歴の位置を記録し、評価後に試した点代入だけをまとめて取り消せます。全配列のコピーを避けたい深さ優先探索や、試し適用を伴う局所探索向けです。

## API

| API | 返り値・使い方 | 計算量 |
| --- | --- | --- |
| `ahc::RollbackArray<T>(n,initial=T{})` | 全要素を同じ値で初期化 | O(N) |
| `ahc::RollbackArray(vector<T>)` | 初期列を受け取る。型推論可 | コピーO(N)、move O(1) |
| `a.size()`, `a.get(i)` | 要素数、値のコピー | O(1) |
| `a.snapshot()` | 現在の位置を示す `Snapshot`（`size_t`） | O(1) |
| `a.set(i,value)` | 変更前の値を記録して代入 | 償却O(1) |
| `a.rollback(snapshot)` | 以降の代入を逆順に取り消す | O(取消件数) |
| `a.clear_history()` | 現在値を残し、履歴を破棄 | O(履歴件数) |

添字は `[0,size())` です。同じ値を代入しても履歴を1件記録します。取り消された分岐のスナップショットと、 `clear_history()` より前のスナップショットは再利用しません。値のコピーと代入をO(1)とした計算量です。領域は初期配列に加えて同時に保持した履歴の最大件数に比例します。

## 使用例

```cpp
ahc::RollbackArray board(vector<int>{3,1,4});
const auto before = board.snapshot();
board.set(0,board.get(1));
board.set(2,7);
assert(board.get(2) == 7);
board.rollback(before);
assert(board.get(0) == 3 && board.get(2) == 4);
```
