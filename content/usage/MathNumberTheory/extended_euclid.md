# 拡張ユークリッド互除法

[説明書の目次](../index.md) · [コード](../../../MathNumberTheory/extended_euclid.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/MathNumberTheory/extended_euclid.cpp)

Bézout係数、逆元、一次不定方程式、一次合同式を計算します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

a,b,c=方程式の係数、mod=正の法。

**共通の前提**

- mod>0。solveはa,bの少なくとも一方が非0。計算結果と返却する係数がlong longに収まること。

## ext_euclid::extgcd

```cpp
Result extgcd(ll a,ll b);
```

Result{g,x,y}を返し、a*x+b*y=g>=0を満たします。gはgcd(a,b)。

**計算量**

- O(log(max(|a|,|b|)+1))。

## ext_euclid::bezout

```cpp
pair<ll,ll> bezout(ll a,ll b);
```

extgcdのx,yだけをpairで返します。

**計算量**

- extgcdと同じ。

## ext_euclid::inverse

```cpp
optional<ll> inverse(ll a,ll mod);
```

aのmod逆元を [0,mod) で返し、存在しなければnullopt。

**計算量**

- O(log mod)。

## ext_euclid::solve

```cpp
optional<Diophantine> solve(ll a,ll b,ll c);
```

a*x+b*y=cの解がなければnullopt。存在すればDiophantine{x,y,dx,dy,g}。一般解は(x+dx*t,y+dy*t)。

**計算量**

- extgcdと同じ。

## ext_euclid::congruence

```cpp
optional<Congruence> congruence(ll a,ll b,ll mod);
```

a*x≡b (mod mod)の解がなければnullopt。存在すればCongruence{x,m}を返し、全解はxをmで割った剰余として表します。

**計算量**

- O(log mod)。

## ext_euclid::divide_mod

```cpp
optional<ll> divide_mod(ll a,ll b,ll mod);
```

逆元が存在すればa/b mod modを [0,mod) で返し、存在しなければnullopt。

**計算量**

- O(log mod)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
auto r=ext_euclid::extgcd(30,18);
assert(r.g==6 && 30*r.x+18*r.y==6);
assert(ext_euclid::inverse(3,7).value()==5);
auto s=ext_euclid::solve(6,9,3); assert(s && 6*s->x+9*s->y==3);
```

## メモリ

O(1)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
