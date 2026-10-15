# 線形篩

[説明書の目次](../index.md) · [コード](../../../MathNumberTheory/linear_sieve.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/MathNumberTheory/linear_sieve.cpp)

線形篩で素数・最小素因数・Eulerのphi・Möbiusのmuを前計算します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=前計算上限、D=約数個数。

**共通の前提**

- N>=1。factorize/divisorsは1<=x<=N。isPrimeは0<=x<=Nの範囲で使用します。

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

素因数を昇順に {素因数,指数} で返します。x=1は空。

**計算量**

- O(log x)。

## LinearSieve::divisors

```cpp
vector<ll> divisors(ll x);
```

正の約数を返します。昇順であることは保証しません。x=1は {1}。

**計算量**

- O(log x+D)。

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
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
