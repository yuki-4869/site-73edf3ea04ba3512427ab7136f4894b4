# Zobristハッシュ

盤面や列の各「位置・値」に対応する64 bitの値をXORし、状態の比較に使うキーを作ります。1点変更をO(1)で反映できるため、 [ビーム探索の候補選択](beam_selection.md) で有効です。

## API

| API | 返り値・使い方 | 計算量 |
| --- | --- | --- |
| `ahc::ZobristHash(seed=0)` | `uint64_t` のseedを固定 | O(1) |
| `z.key(index,value)` | `uint32_t` の位置・値から特徴キーを作る | O(1) |
| `z.replace(hash,index,before,after)` | 更新前の値を外し、更新後の値を加えたキーを返す | O(1) |
| `z.build(values)` | `span<const uint32_t>` の列をまとめてハッシュ化 | O(N) |

列の位置は0-indexedで、列の長さは最大 $2^{32}$ です。追加メモリはO(1)で、キーの乱数表を事前確保する必要はありません。比較時はseedと状態の符号化を揃えます。残り手数や向きで将来の選択肢が異なる場合は、それらを別の位置として含めます。異なる状態同士の衝突はあり得ます。

## 使用例

```cpp
ahc::ZobristHash z(2026);
vector<uint32_t> board{1,0,2,0};
auto hash = z.build(board);
hash = z.replace(hash, 1, board[1], 3);
board[1] = 3;
assert(hash == z.build(board));
```
