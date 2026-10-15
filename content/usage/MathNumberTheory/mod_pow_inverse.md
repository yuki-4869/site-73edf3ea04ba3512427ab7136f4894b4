# mod累乗・逆元

[説明書の目次](../index.md) · [コード](../../../MathNumberTheory/mod_pow_inverse.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/MathNumberTheory/mod_pow_inverse.cpp)

繰り返し二乗法によるmod累乗と、素数modにおける逆元を計算します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

a=基数、n=指数、mod=法。

**共通の前提**

- modpowはn>=0、mod>=1。0<=a<modを推奨。積res*aおよびa*aがlong longに収まること。modinvはmodが素数、mod>=2、a%mod!=0。

## modpow

```cpp
long long modpow(long long a, long long n, long long mod);
```

a^nのmodによる剰余を返します。指数0ではmod=1でも1を返す点に注意してください。

**計算量**

- O(log(n+1))。

## modinv

```cpp
long long modinv(long long a, long long mod);
```

a^(mod-2) mod modを計算して逆元を返します。逆元の存在や法の素数性は確認しません。

**計算量**

- O(log mod)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
assert(modpow(2,10,1000000007)==1024);
assert(modinv(2,1000000007)==500000004);
```

## メモリ

O(1)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
