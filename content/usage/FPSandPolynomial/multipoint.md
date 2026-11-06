# 多点評価・多項式補間

[コード](../../../FPSandPolynomial/multipoint.cpp)

一つの多項式を複数の点で評価し、または点と値から次数の小さい多項式を復元します。同じ点集合で繰り返す場合は積木と補間用の逆元を再利用します。

## 多点評価と補間で渡すもの

**多点評価**は、係数が分かっている多項式 $f(X)$ に複数の数を代入する操作です。**補間**は、点とその値の組から多項式の係数を求める操作です。

| 変数 | 配列の意味 | 具体例 |
| --- | --- | --- |
| `FPS f` | `f[j]` は $X^j$ の係数。低次から並べる | `{5,3,2}` は $5+3X+2X^2$ |
| `FPSVector x` | `x[i]` は代入する数（評価点） | `{1,2,3,4}` |
| `FPSVector y` | `y[i]` は点 `x[i]` での値 | `{10,19,32,49}` |

この例では、`multipoint_eval(f,x)` が `y` を返し、`interpolate(x,y)` が係数列 `{5,3,2}` を返します。`y` をそのまま `FPS` にしても元の多項式にはなりません。
大文字の $X$ は多項式の変数、小文字の `x` は評価点を並べたC++の配列です。点は昇順に並べる必要がなく、返り値も入力の順番に対応します。

| したいこと | 呼び出し方 | 必要なファイル |
| --- | --- | --- |
| 1点だけで $f(t)$ を求める | `f.eval(t)` | FPS本体 |
| 任意の複数点で評価する | `multipoint_eval(f,x)` | FPS本体 + このファイル |
| 任意の相異なる点と値から係数を復元する | `interpolate(x,y)` | FPS本体 + このファイル |
| 同じ点集合で何度も評価・補間する | `FPSPointSet tree(x)` を一度作る | FPS本体 + このファイル |
| $f(0),\ldots,f(N-1)$ から別の1点の値だけ求める | `lagrange_consecutive(y,t)` | FPS本体 + [組合せ数列](combinatorial_fps.md) |

## 準備・型

[998244353版FPS](fps.md) または [1000000007版FPS](fps_mod1000000007.md) を一方だけ先に貼ります。
`FPSMint=FPS::value_type`、`FPSVector=vector<FPSMint>` は選んだFPSの法です。`codecf` は `#include <atcoder/convolution>` を有効にします。

## 構築・全API

```cpp
explicit FPSPointSet(const FPSVector& points);
FPS FPSPointSet::product() const;
FPSVector FPSPointSet::eval(const FPS& f) const;
FPS FPSPointSet::interpolate(const FPSVector& values) const;
FPSVector multipoint_eval(const FPS& f,const FPSVector& x);
FPS interpolate(const FPSVector& x,const FPSVector& y);
```

| API | 返り値・操作 | 計算量 |
| --- | --- | --- |
| `FPSPointSet tree(x)` | 評価点をコピーし、積木を構築 | O(M(N)log N) |
| `tree.product()` | Π(X-x[i])をコピーして返す | O(N) |
| `tree.eval(f)` | xと同じ順でf(x[i])を返す | O(M(D+N)log(D+N)) |
| `tree.interpolate(y)` | 点(x[i],y[i])を通る次数<NのFPS | O(M(N)log N) |
| `multipoint_eval(f,x)` | 一度だけの多点評価 | 構築+eval |
| `interpolate(x,y)` | 一度だけの補間 | 構築+interpolate |

N=x.size()、D=f.size()、MはFPS乗算の計算量です。積木と評価点の保持にO(N log N)領域、評価ではさらにO(D+N log N)作業領域を使います。
構築可能な点数は0<=N<=INT_MAX/2に加え、各中間積が選んだFPSのNTT長制限内であることと、メモリを確保できることが必要です。

## 引数と返り値

### FPSPointSet tree(x)

引数 `points` に当たる `x` は、**多項式の係数ではなく評価点の配列**です。`tree` は点をコピーし、各点を根に持つ積多項式を木状に保持します。構築だけでは評価や補間は行いません。

### tree.eval(f)・multipoint_eval(f,x)

| 引数 | 意味 |
| --- | --- |
| `f` | 評価する有限多項式の係数列。`f[j]` が $X^j$ の係数 |
| `x` | 一度だけ使う評価点列。`tree.eval(f)` では構築時の点列を使う |

返り値は長さ `x.size()` の `FPSVector` で、必ず `answer[i] == f.eval(x[i])` です。`f` の項数と点数は一致しなくても構いません。空の `f` は零多項式として、各点の値が0になります。評価点の重複も許します。

`multipoint_eval(f,x)` は内部で木を毎回構築します。同じ `x` に異なる多項式を何度も代入するなら、一度作った `tree` に対して `tree.eval(f)` を繰り返します。

### tree.interpolate(y)・interpolate(x,y)

| 引数 | 意味 |
| --- | --- |
| `x` | 既知の点の横座標。法上で全要素が相異なること |
| `y`（宣言の `values`） | 各点での値。`y[i]` が `x[i]` と対になり、長さは点数と同じ |

返り値 `FPS h` は、すべての `i` で $h(x_i)=y_i$ を満たす **次数が点数未満の多項式の係数列**です。点数をNとすると、その条件を満たす多項式は一意です。末尾の0を除去するため、返り値の長さはN以下です。N項の配列が必要なら `h.resize(x.size())` とします。

元の多項式が次数N以上の場合、その値を補間しても元の係数には戻りません。例えば点 `{1,2}` で $f(X)=X^2$ を評価すると値は `{1,4}` ですが、その2点から補間されるのは $3X-2$、すなわち `{-2,3}` です。次数dの多項式を確実に復元するには、相異なる点がd+1個以上必要です。
補間時の点の重複は、値が同じでも受け付けません。整数として異なっても法で同じ値になる点は重複です。

### tree.product()

引数はありません。$\prod_i(X-x_i)$ の係数を、長さN+1の `FPS` で返します。最高次係数は1で、保持する各点を代入すると0です。点 `{1,2,3}` なら $X^3-6X^2+11X-6$ の係数 `{-6,11,-6,1}` になります。
点が1個もない場合は積の単位元 `{1}` です。返り値はコピーなので、編集しても木の内部は変わりません。

## 制約・更新

添字は0-indexedです。評価点は構築時に固定し、元のxを変更しても木は変わりません。点を変更する場合は構築し直します。
**評価は重複点にも対応**します。補間ではx.size()==y.size()かつ、法上で全x[i]が相異なる必要があります。
空点集合のproductは{1}、eval・interpolateは空です。補間結果は末尾の0を削除するため、項数はNとは限りません。零多項式は空です。

初回の補間で積多項式の微分値の逆元をまとめて計算し、以後はキャッシュします。逆元の一括計算はO(N+log p)、微分値の多点評価は別途必要です。
少数の点・低次数の部分問題はHorner法を使います。入力のf・yは変更しません。前提違反はNDEBUG時も無言で停止します。
旧版で公開されていた構築用のn・sz・x・pは内部化しました。通常の構築・product・eval・interpolateを使うコードは同じ形で利用できます。

## 使用例

```cpp
FPSVector x={1,2,3,4};
FPS f={5,3,2};                     // 5+3X+2X^2 の係数
FPSPointSet tree(x);              // 代入する点を登録
auto y=tree.eval(f);              // y[i]=f(x[i])。値の配列
assert((y==FPSVector{10,19,32,49}));
FPS restored=tree.interpolate(y); // 値から係数へ戻す
assert(restored==f);
assert(multipoint_eval(f,x)==y);
assert(interpolate(x,y)==f);
FPS g={2,1};
auto z=tree.eval(g);              // 同じ点集合を再利用
assert((z==FPSVector{3,4,5,6}));
assert(tree.interpolate(z)==g);
FPS roots=tree.product();
assert((roots==FPS{24,-50,35,-10,1}));
assert((multipoint_eval(f,FPSVector{3,1,3})==FPSVector{32,10,32}));
assert((interpolate(FPSVector{1,2},FPSVector{1,4})==FPS{-2,3}));
assert((interpolate(FPSVector{1,2},FPSVector{0,0}).empty()));
```
