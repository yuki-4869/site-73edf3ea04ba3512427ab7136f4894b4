# 多点評価・多項式補間

[コード](../../../FPSandPolynomial/multipoint.cpp)

一つの多項式を複数の点で評価し、または点と値から次数の小さい多項式を復元します。同じ点集合で繰り返す場合は積木と補間用の逆元を再利用します。

## 準備・型

[998244353版FPS](fps.md) または [1000000007版FPS](fps_mod1000000007.md) を一方だけ先に貼ります。
`FPSMint=FPS::value_type`、`FPSVector=vector<FPSMint>` は選んだFPSの法です。`codecf` は `#include <atcoder/convolution>` を有効にします。

## 構築・全API

```cpp
explicit FPSPointSet(const FPSVector& x);
FPS FPSPointSet::product() const;
FPSVector FPSPointSet::eval(const FPS& f) const;
FPS FPSPointSet::interpolate(const FPSVector& y) const;
FPSVector multipoint_eval(const FPS& f,const FPSVector& x);
FPS interpolate(const FPSVector& x,const FPSVector& y);
```

| API | 返り値・操作 | 計算量 |
| --- | --- | --- |
| `FPSPointSet tree(x)` | 評価点をコピーし、積木を構築 | O(M(N)log(N+1)) |
| `tree.product()` | Π(X-x[i])をコピーして返す | O(N) |
| `tree.eval(f)` | xと同じ順でf(x[i])を返す | O(M(D+N)log(D+N+1)) |
| `tree.interpolate(y)` | 点(x[i],y[i])を通る次数<NのFPS | O(M(N)log(N+1)) |
| `multipoint_eval(f,x)` | 一度だけの多点評価 | 構築+eval |
| `interpolate(x,y)` | 一度だけの補間 | 構築+interpolate |

N=x.size()、D=f.size()、MはFPS乗算の計算量です。積木と評価点の保持にO(Nlog(N+1))領域、評価ではさらにO(D+Nlog(N+1))作業領域を使います。
構築可能な点数は0<=N<=INT_MAX/2に加え、各中間積が選んだFPSのNTT長制限内であることと、メモリを確保できることが必要です。

## 制約・更新

添字は0-indexedです。評価点は構築時に固定し、元のxを変更しても木は変わりません。点を変更する場合は構築し直します。
**評価は重複点にも対応**します。補間ではx.size()==y.size()かつ、法上で全x[i]が相異なる必要があります。
空点集合のproductは{1}、eval・interpolateは空です。補間結果は末尾の0を削除するため、項数はNとは限りません。零多項式は空です。

初回の補間で積多項式の微分値の逆元をまとめて計算し、以後はキャッシュします。逆元の一括計算はO(N+log(p))、微分値の多点評価は別途必要です。
少数の点・低次数の部分問題はHorner法を使います。入力のf・yは変更しません。前提違反はNDEBUG時も無言で停止します。
旧版で公開されていた構築用のn・sz・x・pは内部化しました。通常の構築・product・eval・interpolateを使うコードは同じ形で利用できます。

## 使用例

```cpp
FPSVector x={1,2,3,4};
FPS f={5,3,2};
FPSPointSet tree(x);
auto y=tree.eval(f);
assert((y==FPSVector{10,19,32,49}));
assert(tree.interpolate(y)==f);
assert(multipoint_eval(f,x)==y);
assert(interpolate(x,y)==f);
FPS g={2,1};
auto z=tree.eval(g);
assert(tree.interpolate(z)==g);
```
