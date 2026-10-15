# 約数列挙

[説明書の目次](../index.md) · [コード](../../../MathNumberTheory/divisors.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/MathNumberTheory/divisors.cpp)

正の整数nの正の約数を昇順で返します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

D=約数個数。

**共通の前提**

- 推奨する利用範囲はn>=1。i*iがlong longを超えない範囲で使用。n=0はこの実装では空vector。

## この実装の注意点

負数の処理も含まれますが、負の完全平方数では重複します。例えば-4は {-4,-2,-2,-1,1,2,2,4}。LLONG_MINのabsと、大きなnでのi*iのオーバーフローにも対応していません。

## get_divisors

```cpp
vector<ll> get_divisors(ll n);
```

nの約数の昇順vectorを返します。正数は各約数が1回ずつ現れます。

**計算量**

- O(sqrt(n)+D log D)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
auto d=get_divisors(12);
assert((d==vl{1,2,3,4,6,12}));
```

## メモリ

O(D)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
