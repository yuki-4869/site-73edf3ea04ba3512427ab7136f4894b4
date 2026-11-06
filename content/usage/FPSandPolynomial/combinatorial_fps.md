# FPSによる組合せ数列

[コード](../../../FPSandPolynomial/combinatorial_fps.cpp)

連続点のLagrange補間、二項変換、Stirling数、Bernoulli数、Bell数、整数分割数をまとめて計算します。

## 準備・型

[998244353版FPS](fps.md) または [1000000007版FPS](fps_mod1000000007.md) の一方を先に貼ります。ほかの拡張ファイルは不要です。
`FPSMint=FPS::value_type`、`FPSVector=vector<FPSMint>` は選んだFPSの法です。`codecf` では `#include <atcoder/convolution>` を有効にしてください。
添字は0-indexedで、返り値の数は法で割った余りです。

## 全API

```cpp
FPSMint lagrange_consecutive(const FPSVector& y,FPSMint x);
FPSVector binomial_transform(const FPSVector& a);
FPSVector inverse_binomial_transform(const FPSVector& a);
FPSVector stirling2_row(ll n);
FPSVector stirling1_signed(ll n);
FPSVector stirling1_unsigned(ll n);
FPSVector bernoulli_numbers(ll n);
FPSVector bell_numbers(ll n);
FPSVector partition_numbers(ll n);
```

| API | 返り値・意味 | 計算量 |
| --- | --- | --- |
| `lagrange_consecutive(y,x)` | 連続点0..N-1の値から求めたf(x) | O(N) |
| `binomial_transform(a)` | b[n]=Σ C(n,k)a[k] | O(M(N)) |
| `inverse_binomial_transform(a)` | b[n]=Σ(-1)^(n-k) C(n,k)a[k] | O(M(N)) |
| `stirling2_row(n)` | S(n,0)..S(n,n) | O(M(n)+n log n) |
| `stirling1_signed(n)` | X(X-1)…(X-n+1)の係数0..n | O(M(n)) |
| `stirling1_unsigned(n)` | 符号を除いた第一種Stirling数0..n | O(M(n)) |
| `bernoulli_numbers(n)` | B_0..B_n | O(M(n)) |
| `bell_numbers(n)` | Bell(0)..Bell(n) | O(M(n)) |
| `partition_numbers(n)` | p(0)..p(n) | O(M(n)+sqrt(n)) |

Mは選んだFPSの乗算計算量です。上のnを使う列は**n+1項**を返し、二項変換は入力と同じ項数です。
作業領域と共有キャッシュはO(N)またはO(n)。法を固定して数え、スカラー逆元の計算がある場合はO(log p)を加えます。

## 定義・制約

- Lagrange補間はy[i]=f(i)、deg f<N、N<=pが前提です。空なら0を返します。xは法上の値として扱い、0<=x.val()<Nなら入力を直接返します。
- 二項変換の配列長はp以下。空なら空です。逆変換と順に適用すると元に戻ります。
- Stirling数・Bell数・分割数は0<=n<p、Bernoulli数は0<=n<p-1です。各中間積のNTT長制限も満たしてください。
- 第一種signedの係数は符号付きです。unsignedはsigned[n,k]に(-1)^(n-k)を掛けたものです。n=0ではどちらも{1}です。
- Bernoulli数は $X/(\exp(X)-1)=\sum B_iX^i/i!$ の定義で、**B_1=-1/2**です。
- Bell(0)=p(0)=1です。整数分割は順序を区別せず、正整数の和としての表し方を数えます。
- 整数逆元・階乗・逆階乗はFPS本体とキャッシュを共有します。最大使用サイズまで合計線形で拡張し、その分のメモリを保持します。

第一種Stirling数は $P_{2m}(X)=P_m(X)P_m(X-m)$ の倍長化とTaylor shiftを使います。
分割数はEulerの五角数定理から疎な分母を作り、FPSの逆数でまとめて求めます。どちらも元の積木・逐次DPから計算量を改善しています。
前提違反はNDEBUG時もメッセージなしで停止します。入力配列は変更しません。

## 使用例

```cpp
FPSVector y={1,4,9,16};
assert(lagrange_consecutive(y,10)==FPSMint(121));
FPSVector a={1,2,3,4};
auto transformed=binomial_transform(a);
assert(inverse_binomial_transform(transformed)==a);
assert((stirling1_signed(3)==FPSVector{0,2,-3,1}));
assert((stirling1_unsigned(3)==FPSVector{0,2,3,1}));
assert((stirling2_row(3)==FPSVector{0,1,3,1}));
assert(bernoulli_numbers(1)[1]==-FPSMint(2).inv());
assert(bell_numbers(3)[3]==FPSMint(5));
assert(partition_numbers(5)[5]==FPSMint(7));
```
