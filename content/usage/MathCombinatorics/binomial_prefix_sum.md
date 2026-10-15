# 二項係数の累積和

[説明書の目次](../index.md) · [コード](../../../MathCombinatorics/binomial_prefix_sum.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/MathCombinatorics/binomial_prefix_sum.cpp)

F(n,k)=Σ_{i=0}^k C(n,i)を計算し、nとkを近接状態へ動かす際に更新します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

dn,dkはn,kの変化量。k<0ではF=0、k>=nではF=2^n。

**共通の前提**

- 共通テンプレートのmint・nCr・repを使用。n>=0、到達先もn>=0。階乗法によるnCrが有効な範囲（N+100<mod）とし、必要な最大nまで階乗を先に用意します。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [binom_prefix_sum](#api-2)
- [BinomPrefixState::value](#api-3)
- [BinomPrefixState::down_keep](#api-4)
- [BinomPrefixState::down_decrease](#api-5)
- [BinomPrefixState::up_keep](#api-6)
- [BinomPrefixState::up_increase](#api-7)
- [BinomPrefixState::shift](#api-8)
- [BinomPrefixState::shifted_value](#api-9)

</details>

## 型・テンプレート引数

```cpp
struct BinomPrefixState;
```

## この実装の注意点

nCrの内部で階乗表が不足すると表の再構築が発生するため、上記のO(1)更新は前計算済みの場合です。dn=0ではdk=0しか指定できません。

<a id="api-1"></a>

## コンストラクタ

```cpp
BinomPrefixState(ll n_,ll k_);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(min(n,k)+1)（階乗前計算後）。

<a id="api-2"></a>

## binom_prefix_sum

```cpp
mint binom_prefix_sum(ll n,ll k);
```

F(n,k)をmintで返します。

**計算量**

- O(max(0,min(n,k)+1))。

<a id="api-3"></a>

## BinomPrefixState::value

```cpp
mint value() const;
```

現在のF(n,k)を返します。

**計算量**

- O(1)。

<a id="api-4"></a>

## BinomPrefixState::down_keep

```cpp
void down_keep();
```

(n,k)から(n-1,k)へ更新します。

**制約**

- n>=1

**計算量**

- O(1)。

<a id="api-5"></a>

## BinomPrefixState::down_decrease

```cpp
void down_decrease();
```

(n,k)から(n-1,k-1)へ更新します。

**制約**

- n>=1

**計算量**

- O(1)。

<a id="api-6"></a>

## BinomPrefixState::up_keep

```cpp
void up_keep();
```

(n,k)から(n+1,k)へ更新します。

**計算量**

- O(1)。

<a id="api-7"></a>

## BinomPrefixState::up_increase

```cpp
void up_increase();
```

(n,k)から(n+1,k+1)へ更新します。

**計算量**

- O(1)。

<a id="api-8"></a>

## BinomPrefixState::shift

```cpp
void shift(ll dn,ll dk);
```

(n+dn,k+dk)へ状態を更新します。

**制約**

- dn>=0なら0<=dk<=dn、dn<0ならdn<=dk<=0

**計算量**

- O(|dn|)。

<a id="api-9"></a>

## BinomPrefixState::shifted_value

```cpp
mint shifted_value(ll dn,ll dk) const;
```

状態を変更せず、shift後に得るFを返します。

**制約**

- shiftと同じ制約

**計算量**

- O(|dn|)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
factorial(10);
BinomPrefixState s(4,2); assert(s.value().val()==11);
s.shift(1,1); assert(s.value().val()==26);
s.shift(-1,-1); assert(s.value().val()==11);
```

## メモリ

状態はO(1)、階乗表O(最大n)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
