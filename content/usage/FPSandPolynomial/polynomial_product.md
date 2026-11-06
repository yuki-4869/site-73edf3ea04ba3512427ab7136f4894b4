# 多項式の総積

[コード](../../../FPSandPolynomial/polynomial_product.cpp)

複数の多項式の積、および根の列からモニック多項式Π(X-a[i])を求めます。

## 入力の選び方

| 手元のデータ | 呼び出し方 | 例 |
| --- | --- | --- |
| 各因子の係数列 | `polynomial_product(factors)` | `{{1,1},{1,2}}` は $(1+X)(1+2X)$ |
| 積の根だけ | `product_linear(roots)` | `{1,2}` は $(X-1)(X-2)$ |

根とは、その値をXに代入すると多項式が0になる数です。**係数列と根の配列は別の入力**です。`product_linear({1,2})` は係数 `{2,-3,1}` を返します。

## 準備・API

[998244353版FPS](fps.md) または [1000000007版FPS](fps_mod1000000007.md) の一方を先に貼ります。
`FPSMint=FPS::value_type`、`FPSVector=vector<FPSMint>` はその法に従います。`codecf` は `#include <atcoder/convolution>` を有効にします。

```cpp
FPS polynomial_product(vector<FPS> factors);
FPS product_linear(const FPSVector& roots);
```

| API | 返り値・処理 | 計算量 |
| --- | --- | --- |
| `polynomial_product(fs)` | 全多項式の積をFPSで返す | O(S log^2 S) |
| `product_linear(a)` | Π(X-a[i])をFPSで返す | O(N log^2 N) |

Sは入力係数数の総和、Nは根の個数です。空の因子も含む入力走査の時間として、総積では別途O(fs.size())を数えます。
作業領域は総積でO(S+fs.size())、一次因子でO(N)です。係数・根の添字は0-indexedです。

## 各引数と返り値

### polynomial_product(factors)

引数は `vector<FPS>` です。外側の添字iが因子の番号、内側の添字jが $X^j$ の係数を表します。`factors[i][j]` がi番目の多項式のj次係数です。因子ごとに項数が違っても構いません。
返り値は $\prod_i f_i(X)$ の係数列 `FPS` です。例えば `{{1,1},{1,2},{1,3}}` なら $(1+X)(1+2X)(1+3X)=1+6X+11X^2+6X^3$ なので `{1,6,11,6}` になります。

### product_linear(roots)

引数 `roots[i]` はi番目の根で、型は `FPSVector` です。返り値は最高次係数が1の $\prod_i(X-\mathrm{roots}_i)$ の係数列 `FPS` です。根の個数Nに対してN+1項を返し、根の並び順は結果に影響しません。
根の重複は重根として扱います。例えば `{2,2}` なら $(X-2)^2$、係数は `{4,-4,1}` です。これは補間に渡す「相異なる点」の条件とは異なります。

$\prod_i(1+a_iX)$ を作りたい場合は、`product_linear(a)` ではなく、因子を `{1,a[i]}` として `polynomial_product` に渡します。$\prod_i(X+a_i)$ なら根に `-a[i]` を渡します。

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
assert((product_linear(FPSVector{2,2})==FPS{4,-4,1}));
assert((product_linear(FPSVector{})==FPS{1}));
assert(polynomial_product(vector<FPS>{{1,1},{0}}).empty());
```
