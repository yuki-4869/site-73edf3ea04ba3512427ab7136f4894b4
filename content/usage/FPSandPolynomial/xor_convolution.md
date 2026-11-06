# XOR畳み込み（FWHT）

[説明書の目次](../index.md) · [コード](../../../FPSandPolynomial/xor_convolution.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/FPSandPolynomial/xor_convolution.cpp)

FWHTでXOR畳み込み c[k]=Σ_{i xor j=k}a[i]b[j] を求めます。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=変換長（畳み込みでは入力長の最大を覆う2冪）。

**共通の前提**

- transformの配列長は正の2冪。convolutionは内部で長さを2冪まで拡張。通常版の逆変換に必要な除算と中間値の演算が型Tで可能であること。mod版はmod>=2、逆変換はgcd(N,mod)=1。

## 型・テンプレート引数

```cpp
class FWHT;
```

## 各引数の意味

添字iをビット集合と考え、`a[i]` は集合iの重み・個数などを表します。通常の多項式積の添字i+jとは異なり、結果の添字は `i xor j` です。

| 引数 | 意味 | 更新の有無 |
| --- | --- | --- |
| `a`（transform系） | 変換したい非空の2冪長の配列 | その場で書き換える。返り値はvoid |
| `a`,`b`（convolution系） | XOR畳み込みしたい2つの列。長さは異なってもよい | 値渡し。元の列は変更しない |
| `inverse` | `false` は順変換、`true` は逆変換。省略時は `false` | `transform(a,true)` で元へ戻す |
| `mod` | 係数を割る法。配列の長さではない | mod版の返り値・更新後の値は0以上mod未満 |
| テンプレート引数 `T` | 係数の型。`ll`、`double`、modintなど | 加減乗算・逆変換のNによる除算が必要 |

`FWHT::convolution<ll>(a,b)` は整数版、`FWHT::convolution_mod(a,b,998244353)` は `vector<ll>` を使う法付き版です。後者ではTを指定しません。
convolution系の返り値は、両入力長の最大以上の最小の2冪Nの配列です。片方が空なら空です。
例えば `{1,2}` と `{3,4}` では、結果の添字0は $1\cdot3+2\cdot4=11$、添字1は $1\cdot4+2\cdot3=10$ となります。

## この実装の注意点

通常のll版は途中計算がllの範囲を超えないようにしてください。XOR畳み込みなので通常の多項式積とは添字の結合方法が異なります。
通常の整数型で任意の列を逆変換するとNによる除算で端数を捨てます。順変換した列を戻す場合や整数の畳み込みでは、途中計算が範囲内なら割り切れます。
mod版は入力の正規化で `(value % mod + mod)` をllとして計算するため、その加算もllの範囲内であることが必要です。modが2^62以下なら、この正規化の加算は全ll入力に対して範囲内です。

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
vl values={1,2,3,4},original=values;
FWHT::transform(values);       // 順変換は入力を書き換える
FWHT::transform(values,true);  // 同じ列を逆変換する
assert(values==original);
FWHT::transform_mod(values,998244353);
FWHT::transform_mod(values,998244353,true);
assert(values==original);
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
