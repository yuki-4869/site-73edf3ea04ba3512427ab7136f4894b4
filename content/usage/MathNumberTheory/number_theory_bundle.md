# 整数論のまとめ

[説明書の目次](../index.md) · [コード](../../../MathNumberTheory/number_theory_bundle.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/MathNumberTheory/number_theory_bundle.cpp)

整数配列の約数・倍数方向の和とその逆変換を求めます。線形篩と素因数関連のAPIも同じファイルに収録しています。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=変換上限、D=xの約数個数。

**共通の前提**

- 配列は長さN+1で、添字1..Nを使用。0番目は変更しません。S.n>=N。篩の素因数関連は1<=x<=S.n。変換は値渡しで返します。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [LinearSieve::is_prime](#api-2)
- [LinearSieve::spf](#api-3)
- [LinearSieve::prime_id](#api-4)
- [LinearSieve::prime_count](#api-5)
- [LinearSieve::factor](#api-6)
- [LinearSieve::factor_list](#api-7)
- [LinearSieve::factor_unique](#api-8)
- [LinearSieve::rad](#api-9)
- [LinearSieve::factor_zobrist](#api-10)
- [LinearSieve::divisors](#api-11)
- [LinearSieve::omega](#api-12)
- [LinearSieve::Omega](#api-13)
- [LinearSieve::phi](#api-14)
- [LinearSieve::mobius](#api-15)
- [divisor_zeta](#api-16)
- [divisor_mobius](#api-17)
- [multiple_zeta](#api-18)
- [multiple_mobius](#api-19)

</details>

## 型・テンプレート引数

```cpp
struct LinearSieve;
```

## この実装の注意点

同名のLinearSieve・変換関数を持つ別実装との同時貼り付けに注意してください。

<a id="api-1"></a>

## コンストラクタ

```cpp
LinearSieve(ll n);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

<a id="api-2"></a>

## LinearSieve::is_prime

```cpp
bool is_prime(ll x)const;
```

素数かを返します。

**計算量**

- O(1)。

<a id="api-3"></a>

## LinearSieve::spf

```cpp
ll spf(ll x)const;
```

最小素因数を返します。

**計算量**

- O(1)。

<a id="api-4"></a>

## LinearSieve::prime_id

```cpp
ll prime_id(ll x)const;
```

素数の0-indexed番号（非素数は-1）を返します。

**計算量**

- O(1)。

<a id="api-5"></a>

## LinearSieve::prime_count

```cpp
ll prime_count(ll x)const;
```

x以下の素数個数を返します。

**計算量**

- O(1)。

<a id="api-6"></a>

## LinearSieve::factor

```cpp
vp factor(ll x)const;
```

昇順の{素因数,指数}列を返します。

**計算量**

- O(log x)。

<a id="api-7"></a>

## LinearSieve::factor_list

```cpp
vl factor_list(ll x)const;
```

重複込みの素因数列を返します。

**計算量**

- O(log x)。

<a id="api-8"></a>

## LinearSieve::factor_unique

```cpp
vl factor_unique(ll x)const;
```

重複を除く素因数列を返します。

**計算量**

- O(log x)。

<a id="api-9"></a>

## LinearSieve::rad

```cpp
ll rad(ll x)const;
```

異なる素因数の積を返します。

**計算量**

- O(log x)。

<a id="api-10"></a>

## LinearSieve::factor_zobrist

```cpp
ll factor_zobrist(ll x)const;
```

素因数集合に対するハッシュ（指数は無視）を返します。

**計算量**

- O(log x)。

<a id="api-11"></a>

## LinearSieve::divisors

```cpp
vl divisors(ll x)const;
```

昇順の正の約数列を返します。

**計算量**

- O(log x+D log D)。

<a id="api-12"></a>

## LinearSieve::omega

```cpp
ll omega(ll x)const;
```

異なる素因数の個数を返します。

**計算量**

- O(log x)。

<a id="api-13"></a>

## LinearSieve::Omega

```cpp
ll Omega(ll x)const;
```

重複込みの素因数の個数を返します。

**計算量**

- O(log x)。

<a id="api-14"></a>

## LinearSieve::phi

```cpp
ll phi(ll x)const;
```

Eulerのphiを返します。

**計算量**

- O(log x)。

<a id="api-15"></a>

## LinearSieve::mobius

```cpp
ll mobius(ll x)const;
```

Möbiusのmuを返します。

**計算量**

- O(log x)。

<a id="api-16"></a>

## divisor_zeta

```cpp
vmi divisor_zeta(vmi a,const LinearSieve& S);
```

各xについて Σ_{d|x}a[d] を計算します。約数から倍数へ集約。

**計算量**

- O(N log log(N+2))。

<a id="api-17"></a>

## divisor_mobius

```cpp
vmi divisor_mobius(vmi a,const LinearSieve& S);
```

約数ゼータ変換を逆変換し、元の配列を復元します。

**計算量**

- O(N log log(N+2))。

<a id="api-18"></a>

## multiple_zeta

```cpp
vmi multiple_zeta(vmi a,const LinearSieve& S);
```

各xについて Σ_{x|m}a[m] を計算します。倍数から約数へ集約。

**計算量**

- O(N log log(N+2))。

<a id="api-19"></a>

## multiple_mobius

```cpp
vmi multiple_mobius(vmi a,const LinearSieve& S);
```

倍数ゼータ変換を逆変換します。

**計算量**

- O(N log log(N+2))。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
LinearSieve s(20);
assert(s.factor(12).size()==2 && s.phi(12)==4);
vmi a(21,1); a[0]=0;
auto b=divisor_zeta(a,s);
assert(b[12].val()==6);
assert(divisor_mobius(b,s)==a);
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
