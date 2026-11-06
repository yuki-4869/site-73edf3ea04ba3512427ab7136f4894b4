# 多項式の総積

[コード](../../../FPSandPolynomial/polynomial_product.cpp)

複数の多項式の積、および根の列からモニック多項式Π(X-a[i])を求めます。

## 準備・API

[998244353版FPS](fps.md) または [1000000007版FPS](fps_mod1000000007.md) の一方を先に貼ります。
`FPSMint=FPS::value_type`、`FPSVector=vector<FPSMint>` はその法に従います。`codecf` は `#include <atcoder/convolution>` を有効にします。

```cpp
FPS polynomial_product(vector<FPS> factors);
FPS product_linear(const FPSVector& roots);
```

| API | 返り値・処理 | 計算量 |
| --- | --- | --- |
| `polynomial_product(fs)` | 全多項式の積をFPSで返す | O(Slog^2(S+1)) |
| `product_linear(a)` | Π(X-a[i])をFPSで返す | O(Nlog^2(N+1)) |

Sは入力係数数の総和、Nは根の個数です。空の因子も含む入力走査の時間として、総積では別途O(fs.size())を数えます。
作業領域は総積でO(S+fs.size())、一次因子でO(N)です。係数・根の添字は0-indexedです。

## 空・零・入力の扱い

空リストの積は{1}、零多項式を一つでも含む積は空です。総積は因子の末尾0を削除します。
根の重複を許し、product_linearはN+1項のモニック多項式を返します。
各中間積が選んだFPSのNTT長制限内である必要があります。

総積は値渡しなので元のfsを変更しません。`polynomial_product(move(fs))` なら入力コピーを省けます。
定数因子を先にまとめ、残りは短い多項式から掛けます。ヒープからの取り出しはmoveを使います。
一次因子の積は少数なら直接、それ以上は平衡分割で計算します。

## 使用例

```cpp
vector<FPS> factors={{1,1},{1,2},{1,3}};
FPS product=polynomial_product(factors);
assert((product==FPS{1,6,11,6}));
FPSVector roots={1,2,3};
FPS monic=product_linear(roots);
assert((monic==FPS{-6,11,-6,1}));
assert((polynomial_product({})==FPS{1}));
```
