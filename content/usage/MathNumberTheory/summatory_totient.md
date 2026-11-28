# トーティエント関数の累積和

## API

`summatory_totient<T>(n)` は $\sum_{i=1}^n\varphi(i)$ を型 `T` で返します。$0\le n\le10^{10}$ で、$n=0$ は 0、$\varphi(1)=1$ です。`T` に `modint` または正確な整数計算用の `__int128` を指定できます。

[線形篩](linear_sieve.md) を先に貼ります。三角数の計算では積の前に整数として 2 で割るため、法の上の逆元は不要です。

## 計算量

$N^{2/3}$ 程度まで線形篩で前計算します。残りは $\sum_{d=1}^n\Phi(\lfloor n/d\rfloor)=n(n+1)/2$ から、同じ商を持つ範囲をまとめて評価します。時間と領域はともに $O(N^{2/3})$ です。

## 使用例

```cpp
if(summatory_totient<mint>(10)!=32 || summatory_totient<__int128>(0)!=0)std::abort();
```

[Library Checker の対応問題](https://judge.yosupo.jp/problem/sum_of_totient_function)
