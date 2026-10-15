# 第二種スターリング数

[説明書の目次](../index.md) · [コード](../../../MathCombinatorics/stirling_second.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/MathCombinatorics/stirling_second.cpp)

第二種スターリング数S(n,k)、すなわちn個の区別される要素をk個の空でない区別されない集合に分割する方法の数を求めます。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=前計算上限。

**共通の前提**

- 0<=n<=max_n。k<0やk>nならgetは0。MODはACL dynamic_modintが扱える正の法。演算に除算はなく、漸化式自体に素数性は不要。

## 型・テンプレート引数

```cpp
struct StirlingSecond;
```

## この実装の注意点

dynamic_modint<-1>の法は同じIDを使う全インスタンスで共有されます。別の法で新しいStirlingSecondを初期化すると、既存インスタンスの値の扱いにも影響します。

## コンストラクタ

```cpp
StirlingSecond();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
StirlingSecond(int n, ll MOD);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N^2)。

## StirlingSecond::init

```cpp
void init(int n, ll MOD);
```

法を設定しS(0,0)=1、S(n,k)=S(n-1,k-1)+k*S(n-1,k)で表を再構築します。

**計算量**

- O(N^2)。

## StirlingSecond::get

```cpp
inline ll get(int n, int k) const;
```

S(n,k) mod MODをlong longで返します。

**計算量**

- O(1)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
StirlingSecond s(5,998244353);
assert(s.get(3,2)==3 && s.get(0,0)==1 && s.get(3,4)==0);
```

## メモリ

O(N^2)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
