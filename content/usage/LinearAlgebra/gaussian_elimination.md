# ガウス消去法

[説明書の目次](../index.md) · [コード](../../../LinearAlgebra/gaussian_elimination.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/LinearAlgebra/gaussian_elimination.cpp)

体上の連立一次方程式A*x=bについて、存在判定・rank・解の1つを返します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=方程式数、M=変数数。

**共通の前提**

- Aは空でないN×M行列。b.size()==N。Tは体の四則演算、0/1の構築、等値比較ができること。浮動小数の誤差対策や部分ピボット選択はありません。

## 型・テンプレート引数

```cpp
template<class T>
struct GaussAns;
```

## GaussianElimination

```cpp
GaussAns<T> GaussianElimination(vector<vector<T>> a,vector<T> b);
```

GaussAns<T>{ok,rank,x}を返します。解なしではok=false,xは空。複数解がある場合は自由変数を0に置いた1解を返します。入力A,bは値渡しで、元の配列を変更しません。

**計算量**

- O(NM min(N,M))。

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `ok` | 解の存在 |
| `rank` | 係数行列のrank |
| `x` | 長さMの1つの解 |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
vector<vector<mint>> a={{1,1},{1,2}}; vector<mint> b={3,5};
auto r=GaussianElimination(a,b);
assert(r.ok && r.rank==2 && r.x[0]==1 && r.x[1]==2);
```

## メモリ

O(NM)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
