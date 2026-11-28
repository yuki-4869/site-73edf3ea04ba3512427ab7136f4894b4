# 高速乱数（SplitMix64）

乱数生成器の初期seedを固定すると、同じ呼び出し順で同じ系列を再現できます。近傍の選択、探索順のシャッフル、同一seedでの比較に使えます。基本テンプレートの `Random(l,r)` と併用できる `ahc::Random` です。

## API

| API | 返り値・使い方 | 計算量 |
| --- | --- | --- |
| `ahc::Random(seed=0)` | `uint64_t` のseedで初期化。0も有効 | O(1) |
| `Random::result_type` | `uint64_t` | |
| `Random::min()`, `Random::max()` | 乱数の最小値0、最大値 `UINT64_MAX` | O(1) |
| `rng()` | 64 bitの乱数 | O(1) |
| `rng.below(bound)` | `[0,bound)` の一様整数。正の `bound` を渡す | 期待O(1) |
| `rng.real()` | `[0,1)` の実数。上位53 bitを使用 | O(1) |
| `rng.shuffle(first,last)` | ランダムアクセスの半開区間をその場でシャッフル | 期待O(N) |

`below` は128 bitの積と棄却法で剰余偏りを避けます。GNU C++23の `__uint128_t` を使用します。乱数器はコピー時に内部状態も引き継ぎます。

## 使用例

```cpp
ahc::Random random(42);
vector<int> order{0,1,2,3};
random.shuffle(order.begin(), order.end());
int chosen = order[random.below(order.size())];
double acceptance_sample = random.real();
```
