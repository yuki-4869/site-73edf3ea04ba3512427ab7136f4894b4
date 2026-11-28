# 動的な重み付き抽選

各IDに非負の整数重みを持たせ、重みの総和から抽選します。重みの更新が混ざるケースや、不採用になった候補の重みを0にして抽選対象から外すケースに使えます。

## API

| API | 返り値・使い方 | 計算量 |
| --- | --- | --- |
| `ahc::WeightedSampler(n)` | 全重み0で構築 | O(N) |
| `ahc::WeightedSampler(weights)` | `span<const Weight>` から構築 | O(N) |
| `ws.size()`, `ws.total()`, `ws.weight(i)` | 要素数、重みの総和、1要素の重み | O(1) |
| `ws.set(i,value)` | 指定した重みを置換 | O(log N) |
| `ws.select(rank)` | `rank` が属する要素の添字 | O(log N) |

`Weight` は `uint64_t` です。全重みの和も `uint64_t` に収めます。 `select(rank)` は累積和が初めて `rank` を超える0-indexedの添字を返します。 `rank` の範囲は `[0,ws.total())` で、全重み0のときは抽選を行いません。 `rng.below(ws.total())` を渡すと重みに比例した抽選になります。領域はO(N)です。

## 使用例

```cpp
vector<uint64_t> weights{2,0,3};
ahc::WeightedSampler sampler(weights);
assert(sampler.select(1) == 0 && sampler.select(2) == 2);
sampler.set(0,0);
assert(sampler.total() == 3 && sampler.select(0) == 2);
```
