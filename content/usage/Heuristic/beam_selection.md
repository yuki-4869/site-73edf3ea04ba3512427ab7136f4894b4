# ビーム探索の候補選択

各層で候補を集め、評価の高い順にビーム幅まで返します。候補に保存するのは親の位置・操作・評価値・キーです。採用候補だけに操作を適用し、次の状態を生成できます。

## API

| API | 返り値・使い方 | 計算量 |
| --- | --- | --- |
| `ahc::BeamSelection<Move,Score>(width,deduplicate=false)` | 空の層を作成。 `Score` の既定値は `long long` | O(1) |
| `beam.reserve(candidate_count)` | 候補領域を予約 | 期待O(U) |
| `beam.push(parent,move,score,key=0)` | 0-indexedの親番号と操作を登録。幅0なら追加しない | 償却期待O(1) |
| `beam.take()` | `vector<Candidate>` をスコア降順で返す。同時に層を空にする | 期待O(U + K log K) |
| `beam.clear()` | 現在の層を空にする | O(U) |

`Candidate` の公開メンバーは `parent`、`move`、`score`、`key` です。Uは保持中の候補数、Kは `min(width,U)` です。計算量では操作型とスコア型の操作をO(1)とします。浮動小数のスコアにNaNを渡さないでください。

重複排除を有効にした場合、同じ64 bitキーのうち最高スコアの候補だけを保持します。同キー・同点では先着、異なるキーで同点なら各キーの初回登録順です。キーを省略すると0になるため、重複排除を使う層では [Zobristハッシュ](zobrist_hash.md) または状態を一意に表すキーを渡してください。ハッシュを使う場合は衝突の可能性があります。

## 使用例

```cpp
ahc::BeamSelection<char> beam(2,true);
beam.reserve(3);
beam.push(0,'L',10,101);
beam.push(0,'R',20,102);
beam.push(1,'U',21,102);
auto next = beam.take();
assert(next.size() == 2 && next[0].parent == 1 && next[0].move == 'U');
```
