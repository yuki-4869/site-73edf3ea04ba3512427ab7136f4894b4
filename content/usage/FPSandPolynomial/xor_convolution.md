# XOR畳み込み（FWHT）

[説明書の目次](../index.md) · [コード](../../../FPSandPolynomial/xor_convolution.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/FPSandPolynomial/xor_convolution.cpp)

FWHTでXOR畳み込み c[k]=Σ_{i xor j=k}a[i]b[j] を求めます。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=変換長（畳み込みでは入力長の最大を覆う2冪）。

**共通の前提**

- transformの配列長は正の2冪。convolutionは内部で長さを2冪まで拡張。通常版の逆変換に必要な除算と中間値の演算が型Tで可能であること。mod版はmod>0、逆変換はgcd(N,mod)=1。

## 型・テンプレート引数

```cpp
class FWHT;
```

## この実装の注意点

通常のll版は途中計算がllの範囲を超えないようにしてください。XOR畳み込みなので通常の多項式積とは添字の結合方法が異なります。

## FWHT::transform

```cpp
static void transform(vector<T>& a, bool inverse = false);
```

aをその場でFWHT変換します。inverse=trueなら逆変換します。

**計算量**

- O(N log N)。

## FWHT::convolution

```cpp
static vector<T> convolution(vector<T> a, vector<T> b);
```

入力を値渡しで受け、XOR畳み込みのvector<T>を返します。

**計算量**

- O(N log N)。

## FWHT::transform_mod

```cpp
static void transform_mod(vector<ll>& a, ll mod, bool inverse = false);
```

modを法としてその場でFWHT変換／逆変換します。

**計算量**

- O(N log N)。

## FWHT::convolution_mod

```cpp
static vector<ll> convolution_mod(vector<ll> a, vector<ll> b, ll mod);
```

modによるXOR畳み込みをvector<ll>で返します。

**計算量**

- O(N log N)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
auto c=FWHT::convolution<ll>({1,2},{3,4});
assert((c==vl{11,10}));
auto d=FWHT::convolution_mod({1,2},{3,4},998244353);
assert((d==vl{11,10}));
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
