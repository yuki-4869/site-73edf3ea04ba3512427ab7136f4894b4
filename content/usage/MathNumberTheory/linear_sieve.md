# 線形篩

[説明書の目次](../index.md) · [コード](../../../MathNumberTheory/linear_sieve.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/88a03de2887e9cafbd2d2fe5a1f3a2129844b351/MathNumberTheory/linear_sieve.cpp)

線形篩で素数・最小素因数・Eulerのphi・Möbiusのmuを前計算します。素因数分解は前計算上限 N の二乗まで完全な分解を保証し、それより大きい入力も残りを1因子として返します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=前計算上限、P=N以下の素数の個数、D=約数列挙の出力個数。

F(x) は素因数分解の計算量で、x<=N なら O(log x)、x>N なら最悪 O(P+log x) とします。

**共通の前提**

- N>=1。`factorize` と `divisors` の入力は `1<=x<=LLONG_MAX`。完全な素因数分解・全約数の列挙を保証する範囲は x<=N^2 です。
- `isPrime` は 0<=x<=N、`spf/phi/mu` の参照は前計算した範囲内で使用します。

## 型・テンプレート引数

```cpp
struct LinearSieve;
```

## コンストラクタ

```cpp
LinearSieve(ll n);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

## LinearSieve::isPrime

```cpp
bool isPrime(ll x);
```

xが素数かを返します。

**計算量**

- O(1)。

## LinearSieve::factorize

```cpp
vector<pair<ll,ll>> factorize(ll x);
```

因子を昇順に `{因子,指数}` で返します。x=1 は空です。返された因子をそれぞれ指数回掛けた積は、入力 x に等しくなります。

- x<=N では、従来どおり最小素因数表 `spf` で分解します。
- x>N では、前計算した素数を昇順に使って試し割りします。同じ素数で割り切れるだけ割り、残りが N 以下になった時点で `spf` に切り替えます。結果に同じ因子の項が重複することはありません。
- 試し割りは `p>残り/p` でも終了します。積 `p*p` を計算しないため、この終了条件でのオーバーフローを避けられます。
- 最後の残り r>1 は `{r,1}` として追加します。x<=N^2 なら r は素数ですが、x>N^2 では合成数の場合もあります。例えば N=10, x=143 では `{{143,1}}` を返します。

Nまでの素数で割った後の残りが合成数なら、その素因数はすべて N より大きく、残りは N^2 より大きくなります。このため、x<=N^2 では完全な素因数分解が保証されます。N^2 までの配列を作る必要はありません。

**計算量**

- F(x)。途中で残りが N 以下になれば、それ以降の試し割りは行いません。

## LinearSieve::divisors

```cpp
vector<ll> divisors(ll x);
```

正の約数を返します。昇順であることは保証しません。x=1は {1}。

内部で `factorize(x)` を使うため、x<=N^2 では全約数を列挙できます。x>N^2 で最後の因子が合成数になった場合は、一部の約数だけを返し、全列挙を保証しません。例えば N=10, x=143 では `{1,143}` を返します。

**計算量**

- F(x)+O(D)。

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `prime` | N以下の素数列 |
| `spf[x]` | 最小素因数 |
| `phi[x]` | Eulerのphi |
| `mu[x]` | Möbiusのmu |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
LinearSieve s(100);
assert(s.isPrime(97) && !s.isPrime(1));
assert(s.phi[12]==4 && s.mu[12]==0);
assert((s.factorize(12)==vector<pair<ll,ll>>{{2,2},{3,1}}));

LinearSieve small(10);
assert((small.factorize(72)==vector<pair<ll,ll>>{{2,3},{3,2}}));
assert((small.factorize(100)==vector<pair<ll,ll>>{{2,2},{5,2}}));
assert((small.factorize(143)==vector<pair<ll,ll>>{{143,1}}));
assert((small.divisors(143)==vector<ll>{1,143}));
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
