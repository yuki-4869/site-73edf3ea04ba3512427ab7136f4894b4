# Miller-Rabin / Pollard Rho

[説明書の目次](../index.md) · [コード](../../../MathNumberTheory/miller_rabin_pollard_rho.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/MathNumberTheory/miller_rabin_pollard_rho.cpp)

64bit非負整数の素数判定と、Pollard Rhoによる素因数分解を行います。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

u64=uint64_t。B=指数や入力のビット長。

**共通の前提**

- factorize/factorize_countはn>=1。mod_mul/mod_powはmod>=1。uint64_tと__uint128_tを使用します。

## 型・テンプレート引数

```cpp
struct PollardRho;
```

## この実装の注意点

因数探索部分は乱数に依存します。rho/factorize_implは内部処理用です。

## PollardRho::mod_mul

```cpp
static u64 mod_mul(u64 a, u64 b, u64 mod);
```

128bit整数を用いa*b mod modを返します。

**計算量**

- O(1)（固定長整数モデル）。

## PollardRho::mod_pow

```cpp
static u64 mod_pow(u64 a, u64 n, u64 mod);
```

a^n mod modを返します。

**計算量**

- O(log(n+1))。

## PollardRho::is_prime

```cpp
static bool is_prime(u64 n);
```

nが素数ならtrue。この実装の固定基底集合はuint64_t全域向けです。

**計算量**

- 固定基底数×O(log n)回のmod乗算。

## PollardRho::factorize

```cpp
static vector<u64> factorize(u64 n);
```

素因数を重複込み・昇順で返します。n=1は空vector。

**計算量**

- 確率的。最悪時間の決定的な上限は保証しません。

## PollardRho::factorize_count

```cpp
static vector<pair<u64, int>> factorize_count(u64 n);
```

昇順に {素因数,指数} で返します。

**計算量**

- factorizeの時間＋結果の集計。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
assert(PollardRho::is_prime(1000000007));
auto f=PollardRho::factorize(60);
assert((f==vector<uint64_t>{2,2,3,5}));
```

## メモリ

素因数列O(log n)と再帰スタック。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
