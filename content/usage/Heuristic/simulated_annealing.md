# 焼き鈍し法・山登り法

スコアを最大化します。近傍による差分を渡すと、受理時だけ状態に操作を適用します。非悪化手は常に受理し、悪化手は `exp(delta / temperature)` の確率で受理します。温度0で使うと、同点を許す山登りになります。

## API

| API | 返り値・使い方 | 計算量 |
| --- | --- | --- |
| `ahc::AnnealingSchedule(start,finish)` | 始点と終点の温度を設定 | O(1) |
| `schedule(progress)` | 温度を `double` で返す。両端が正なら幾何補間、片端が0なら線形補間 | O(1) |
| `ahc::AnnealingLossTable(max_loss,temp=0)` | 悪化量1から `max_loss` までを前計算 | O(1+max_loss) |
| `table.set_temperature(temp)` | 同じ表サイズで、確率を再計算 | O(1+max_loss) |
| `table.temperature()`, `table.max_loss()` | 温度と前計算の最大悪化量 | O(1) |
| `table.probability(loss)` | 非負の整数悪化量の受理確率。0なら1 | O(1) |
| `ahc::Annealing<State,Score>(initial,score)` | 現在状態と最良状態を初期化。型は引数から推論可能 | 状態コピー1回 |
| `sa.state()`, `sa.best_state()` | 現在・最良状態の `const State&` | O(1) |
| `sa.score()`, `sa.best_score()` | 現在・最良スコア | O(1) |
| `sa.try_move(delta,temperature,sample,apply)` | 近傍を受理して `apply(state)` を呼んだら `true` | O(1)とコールバック。最良更新時は状態コピー |
| `sa.try_move(delta,table,sample,apply)` | 64 bit以下の符号付き整数スコアで表を再利用 | 通常版と同じ |

`delta` は「変更後 − 変更前」です。最小化では評価値の符号を反転し、差分と総和が収まる符号付き型を使います。 `sample()` は `[0,1)` の一様乱数で、正温度の悪化手の判定時にだけ呼びます。状態は独立にコピーできる必要があります。同じ最良スコアでは先に到達した状態を保持します。

温度は非負有限、浮動小数のスコア・差分・経過率は有限とします。補間の経過率は端で切り詰めます。固定回数で回せば乱数のseedと合わせて再現しやすくなります。

## 使用例

1の個数を最大化するビット列で、1点反転の差分を渡します。 `sample` には [高速乱数](random.md) の `real()` も使えます。

```cpp
mt19937_64 engine(42);
auto sample = [&] { return generate_canonical<double,53>(engine); };
ahc::Annealing search(vector<int>(100,0), 0LL);
ahc::AnnealingSchedule temperature(2.0, 0.01);
for (int iter=0; iter<10000; ++iter) {
    int idx = uniform_int_distribution<int>(0,99)(engine);
    long long delta = search.state()[idx] ? -1 : 1;
    search.try_move(delta, temperature(iter / 9999.0),
        sample,
        [idx](auto& state) { state[idx] ^= 1; });
}
auto answer = search.best_state();
```

## 受理確率の前計算

悪化量の範囲が狭い整数スコアで、同じ温度で近傍をまとめて試すときに使います。温度を更新するごとに表を作り直すため、1試行ごとに温度を変える場合は温度を直接渡します。悪化量が表の範囲外でも、その場で同じ受理確率を計算します。表の使用領域はO(max_loss)です。

```cpp
ahc::AnnealingLossTable loss_table(1);
for (int batch=0; batch<100; ++batch) {
    loss_table.set_temperature(temperature(batch / 99.0));
    for (int step=0; step<100; ++step) {
        int idx = uniform_int_distribution<int>(0,99)(engine);
        long long delta = search.state()[idx] ? -1 : 1;
        search.try_move(delta, loss_table, sample,
            [idx](auto& state) { state[idx] ^= 1; });
    }
}
```
