# 時間計測（AHC）

`steady_clock` で実時間を計ります。入力時間も予算に含める場合は、入力前にタイマーを作成します。

## API

| API | 返り値・使い方 |
| --- | --- |
| `ahc::Timer(limit_ms)` | 現時刻を基準に計測開始。予算は有限かつ非負のミリ秒 |
| `timer.reset()` | 同じ制限時間で現在時刻から再計測 |
| `timer.limit_ms()` | 設定した予算 |
| `timer.elapsed_ms()` | 経過時間 |
| `timer.remaining_ms()` | 残り時間。期限以降は0 |
| `timer.expired()` | 期限に達していれば `true` |
| `timer.progress()` | 経過率 `[0,1]`。0 msの予算なら1 |

時間と経過率は `double` です。全操作がO(1)です。時間確認を間引く場合は、1バッチの最悪時間と出力にかかる時間を予算から差し引きます。

## 使用例

1 msだけ回す例です。2秒制限なら、入力・出力と1バッチにかかる時間を見込んで、たとえば1900 msを予算にします。

```cpp
ahc::Timer timer(1);
for (double progress; (progress = timer.progress()) < 1.0; ) {
    // 近傍を数回試す。progressを温度スケジュールに渡してもよい。
}
```
