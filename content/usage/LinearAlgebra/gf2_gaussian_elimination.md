# GF(2) のガウス消去法

[説明書の目次](../index.md) · [コード](../../../LinearAlgebra/gf2_gaussian_elimination.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/LinearAlgebra/gf2_gaussian_elimination.cpp)

GF(2)の連立一次方程式をbitsetのXORで解きます。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=方程式数、M=W-1、w=CPUのワードビット数。

**共通の前提**

- W>=1。変数数は常にM=W-1。bitsetの最終bit（添字M）が右辺で、0..M-1が係数。

## 型・テンプレート引数

```cpp
template<size_t W>
struct GF2Ans;
```

## GF2Gaussian

```cpp
GF2Ans<W> GF2Gaussian(vector<bitset<W>> a);
```

GF2Ans<W>{ok,rank,x}を返します。複数解では自由変数0の1解。解なしはxが空です。

**計算量**

- O(NM+N min(N,M) ceil(W/w))。

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `ok` | 解の存在 |
| `rank` | 係数行列のrank |
| `x` | 各変数の0または1 |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
vector<bitset<3>> a={bitset<3>(0b111),bitset<3>(0b010)};
auto r=GF2Gaussian<3>(a);
assert(r.ok && r.x[0]==1 && r.x[1]==0);
```

## メモリ

O(NW/w+M)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
