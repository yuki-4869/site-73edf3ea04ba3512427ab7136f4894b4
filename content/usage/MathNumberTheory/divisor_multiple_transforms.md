# 約数・倍数ゼータ／メビウス変換

[説明書の目次](../index.md) · [コード](../../../MathNumberTheory/divisor_multiple_transforms.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/MathNumberTheory/divisor_multiple_transforms.cpp)

整数配列の約数・倍数方向の和とその逆変換を求めます。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=変換上限、D=xの約数個数。

**共通の前提**

- 配列は長さN+1で、添字1..Nを使用。0番目は変更しません。Tは+=と-=を提供すること。変換は配列をその場で更新します。

## prime_list

```cpp
inline vector<long long> prime_list(long long n);
```

n以下の素数を昇順で返します。

**計算量**

- O(n log log(n+2))。

## divisor_zeta

```cpp
void divisor_zeta(vector<T>& f);
```

各xについて Σ_{d|x}a[d] を計算します。約数から倍数へ集約。

**計算量**

- O(N log log(N+2))。

## divisor_mobius

```cpp
void divisor_mobius(vector<T>& f);
```

約数ゼータ変換を逆変換し、元の配列を復元します。

**計算量**

- O(N log log(N+2))。

## multiple_zeta

```cpp
void multiple_zeta(vector<T>& f);
```

各xについて Σ_{x|m}a[m] を計算します。倍数から約数へ集約。

**計算量**

- O(N log log(N+2))。

## multiple_mobius

```cpp
void multiple_mobius(vector<T>& f);
```

倍数ゼータ変換を逆変換します。

**計算量**

- O(N log log(N+2))。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
vl a(13,1); a[0]=0;
divisor_zeta(a); assert(a[12]==6);
divisor_mobius(a); assert(a[12]==1);
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
