# Bostan–Mori・線形漸化式

[コード](../../../FPSandPolynomial/bostan_mori.cpp)

有理式P(X)/Q(X)の第n係数と、定数係数線形漸化式の第n項を求めます。既知の有限列からの最短漸化式の推定も含みます。

## 準備・型

[998244353版FPS](fps.md) または [1000000007版FPS](fps_mod1000000007.md) の**一方**を先に貼ります。`FPSMint=FPS::value_type`、`FPSVector=vector<FPSMint>` は選んだ法に従います。
`codecf` では `#include <atcoder/convolution>` を有効にしてください。添字は0-indexedで、各中間積はFPSのNTT長制限内である必要があります。

## bostan_mori

```cpp
FPSMint bostan_mori(FPS P,FPS Q,ll n);
```

$[X^n]P(X)/Q(X)$ を返します。n>=0、Q[0]!=0が必要です。P・Qは値渡しで元の列を変更しません。空のPは0です。
高次の不要な接頭辞外を除き、Pの次数がQ以上なら最初に多項式除算します。n=0・定数の分母は直接計算します。
入力の値渡しにはO(P.size()+Q.size())がかかります。処理中の分母と余りの項数の最大をKとすると、その後はO(M(K)log n)で、最初の除算は別途O(M(P.size()+Q.size()))です。作業領域は入力項数に比例します。

## linear_recurrence

```cpp
FPSMint linear_recurrence(const FPSVector& a,const FPSVector& c,ll n);
```

a.size()==c.size()==k、n>=0が必要です。aは第0..k-1項、cは次の漸化式の係数です。

$$a_t=c_0a_{t-1}+c_1a_{t-2}+\cdots+c_{k-1}a_{t-k}\quad(t\ge k).$$

第n項を返します。n<kなら初期値を直接返し、k=0なら全0列として0を返します。O(M(k)log n)時間、O(k)作業領域です。

## berlekamp_massey

```cpp
FPSVector berlekamp_massey(const FPSVector& s);
```

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
```

元の列は変更しません。引数の前提違反はNDEBUG時もメッセージなしで停止します。
