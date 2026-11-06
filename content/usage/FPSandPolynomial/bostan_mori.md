# Bostan–Mori・線形漸化式

[コード](../../../FPSandPolynomial/bostan_mori.cpp)

有理式P(X)/Q(X)の第n係数と、定数係数線形漸化式の第n項を求めます。既知の有限列からの最短漸化式の推定も含みます。

## どの関数を使うか

| 分かっているもの | 求めたいもの | 関数 |
| --- | --- | --- |
| 母関数 $P(X)/Q(X)$ の分子・分母の係数 | 展開した級数の $X^n$ の係数 | `bostan_mori(P,Q,n)` |
| 最初のk項と、直前k項から次項を作る係数 | 数列の第n項 | `linear_recurrence(initial,coefficients,n)` |
| 数列の先頭の値だけ | その既知列に合う最短の漸化式 | `berlekamp_massey(sequence)` |

`n` はどちらも**0-indexedの項番号**です。FPS本体の `inv(n)` などに渡す「出力する項数」とは意味が異なり、返り値は係数型の値1個です。第 $10^{18}$ 項のように全項を列挙できない場合に使えます。

## 準備・型

[998244353版FPS](fps.md) または [1000000007版FPS](fps_mod1000000007.md) の**一方**を先に貼ります。`FPSMint=FPS::value_type`、`FPSVector=vector<FPSMint>` は選んだ法に従います。
`codecf` では `#include <atcoder/convolution>` を有効にしてください。添字は0-indexedで、各中間積はFPSのNTT長制限内である必要があります。

## bostan_mori

```cpp
FPSMint bostan_mori(FPS numerator,FPS denominator,ll n);
```

| 引数 | 意味 | 例 |
| --- | --- | --- |
| `numerator`（P） | 分子多項式の係数。定数項から並べる | `{1}` は $P(X)=1$ |
| `denominator`（Q） | 分母多項式の係数。定数項は非零 | `{1,-1,-1}` は $Q(X)=1-X-X^2$ |
| `n` | 欲しい係数の次数。非負の `ll` | `9` は $X^9$ の係数 |

例えば $1/(1-X-X^2)=1+X+2X^2+3X^3+5X^4+\cdots$ なので、`bostan_mori({1},{1,-1,-1},4)` は5です。点X=nでの値を返す操作ではありません。

$[X^n]P(X)/Q(X)$ を返します。n>=0、Q[0]!=0が必要です。P・Qは値渡しで元の列を変更しません。空のPは0です。
高次の不要な接頭辞外を除き、Pの次数がQ以上なら最初に多項式除算します。n=0・定数の分母は直接計算します。
入力の値渡しにはO(P.size()+Q.size())がかかります。処理中の分母と余りの項数の最大をKとすると、その後はO(M(K)log n)で、最初の除算は別途O(M(P.size()+Q.size()))です。作業領域は入力項数に比例します。

## linear_recurrence

```cpp
FPSMint linear_recurrence(const FPSVector& initial,const FPSVector& coefficients,ll n);
```

| 引数 | 意味 |
| --- | --- |
| `initial`（a） | 最初のk項 $a_0,\ldots,a_{k-1}$ の値。多項式の係数ではない |
| `coefficients`（c） | 直前の項から古い項へ順に並べた漸化式の係数。`c[0]` が $a_{t-1}$ に掛かる |
| `n` | 求めたい項の番号。第0項は `initial[0]` |

a.size()==c.size()==k、n>=0が必要です。aは第0..k-1項、cは次の漸化式の係数です。

$$a_t=c_0a_{t-1}+c_1a_{t-2}+\cdots+c_{k-1}a_{t-k}\quad(t\ge k).$$

第n項を返します。n<kなら初期値を直接返し、k=0なら全0列として0を返します。O(M(k)log n)時間、O(k)作業領域です。

例えば $a_0=1,a_1=4$、$a_t=2a_{t-1}+3a_{t-2}$ なら `initial={1,4}`、`coefficients={2,3}` です。列は1,4,11,34,...なので、`linear_recurrence(initial,coefficients,3)` は34です。係数を `{3,2}` と逆順にしないでください。

内部では $Q(X)=1-c_0X-\cdots-c_{k-1}X^k$、$P(X)=(a_0+a_1X+\cdots+a_{k-1}X^{k-1})Q(X)\bmod X^k$ を作ってBostan–Moriへ渡します。数列と漸化式が分かっている場合、P・Qを手作業で作る必要はありません。

## berlekamp_massey

```cpp
FPSVector berlekamp_massey(const FPSVector& sequence);
```

引数 `sequence`（s）は、連続して分かっている第0項からの**数列の値**です。返り値は初期値や予測値ではなく、`linear_recurrence` の第2引数に渡せる係数列cです。返り値の長さが推定された漸化式の次数kになります。

与えられた有限列sに一致する最短漸化式の係数cを返します。係数の向きはlinear_recurrenceと同じです。
返された次数kに対し、s[t]=Σc[i]s[t-i-1]が既知の範囲t>=kで成立します。未知の続きでの成立は保証しません。
空列・全0列は空を返します。既知列が十分長い場合に真の漸化式を復元できますが、観測した列だけで未知項の正しさは確定しません。
N=s.size()としてO(N^2)時間、O(N)作業領域です。

## 使用例

```cpp
FPSVector initial={0,1},coefficients={1,1};
assert(linear_recurrence(initial,coefficients,10)==FPSMint(55));
FPS P={1},Q={1,-1,-1};
assert(bostan_mori(P,Q,9)==FPSMint(55));
FPSVector sequence={0,1,1,2,3,5,8,13,21,34};
auto recurrence=berlekamp_massey(sequence);
FPSVector first(sequence.begin(),sequence.begin()+recurrence.size());
assert(linear_recurrence(first,recurrence,10)==FPSMint(55));
FPSVector a={1,4},c={2,3};
assert(linear_recurrence(a,c,0)==FPSMint(1));
assert(linear_recurrence(a,c,3)==FPSMint(34));
assert(bostan_mori(FPS{1},FPS{1,-1,-1},4)==FPSMint(5));
```

元の列は変更しません。引数の前提違反はNDEBUG時もメッセージなしで停止します。
