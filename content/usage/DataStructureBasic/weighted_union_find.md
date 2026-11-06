# 重み付きUnion-Find

[説明書の目次](../index.md) · [コード](../../../DataStructureBasic/weighted_union_find.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/DataStructureBasic/weighted_union_find.cpp)

差分制約 potential[b]-potential[a]=w を管理するUnion-Findです。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

a,b=頂点、w=potential[b]-potential[a]。

**共通の前提**

- 頂点は0..N-1。Tは加減算と等値比較ができ、累積差が型の範囲内に収まること。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [WeightedUnionFind::leader](#api-2)
- [WeightedUnionFind::weight](#api-3)
- [WeightedUnionFind::merge](#api-4)
- [WeightedUnionFind::same](#api-5)
- [WeightedUnionFind::size](#api-6)
- [WeightedUnionFind::diff](#api-7)
- [WeightedUnionFind::consistent](#api-8)
- [WeightedUnionFind::groups](#api-9)

</details>

## 型・テンプレート引数

```cpp
template<class T = long long>
struct WeightedUnionFind;
```

<a id="api-1"></a>

## コンストラクタ

```cpp
WeightedUnionFind();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
explicit WeightedUnionFind(int n);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

<a id="api-2"></a>

## WeightedUnionFind::leader

```cpp
int leader(int a);
```

a の代表元を返す )

**計算量**

- 償却 O(alpha(N)。

<a id="api-3"></a>

## WeightedUnionFind::weight

```cpp
T weight(int a);
```

potential[a]-potential[leader(a)]を返します。

**計算量**

- ならしO(alpha(N))。

<a id="api-4"></a>

## WeightedUnionFind::merge

```cpp
int merge(int a, int b, T w);
```

差wの制約として成分を併合し、併合後の代表元を返します。同一成分なら既存の代表元を返し、矛盾を判定しません。

**制約**

- 矛盾検出には先にconsistentを呼ぶ

**計算量**

- ならしO(alpha(N))。

<a id="api-5"></a>

## WeightedUnionFind::same

```cpp
bool same(int a, int b);
```

a,b が同じ連結成分なら true )

**計算量**

- 償却 O(alpha(N)。

<a id="api-6"></a>

## WeightedUnionFind::size

```cpp
int size(int a);
```

a の属する連結成分のサイズ )

**計算量**

- 償却 O(alpha(N)。

<a id="api-7"></a>

## WeightedUnionFind::diff

```cpp
T diff(int a, int b);
```

potential[b]-potential[a]を返します。

**制約**

- same(a,b)==true

**計算量**

- ならしO(alpha(N))。

<a id="api-8"></a>

## WeightedUnionFind::consistent

```cpp
bool consistent(int a, int b, T w);
```

差wの制約が現在の状態と両立するかを返します。未連結ならtrue。状態は併合しません。

**計算量**

- ならしO(alpha(N))。

<a id="api-9"></a>

## WeightedUnionFind::groups

```cpp
vector<vector<int>> groups();
```

全連結成分を列挙)

**計算量**

- O(N alpha(N)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
WeightedUnionFind<ll> uf(3);
uf.merge(0,1,5); uf.merge(1,2,-2);
assert(uf.diff(0,2)==3);
assert(!uf.consistent(0,2,4));
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。

## 練習問題：高さの記録

$N$ 個の地点があり、地点 $i$ の高さを整数 $H_i$ とします。高さは初めは分かっていません。$Q$ 個の操作を順に処理してください。

- `1 a b w`：$H_b-H_a=w$ という記録を追加したい。これまでに採用した記録と両立するなら採用して `Yes`、そうでなければ採用せず `No` を出力する。
- `2 a b`：採用済みの記録だけから $H_b-H_a$ を一意に決められるならその値を、決められないなら `?` を出力する。

### 制約

- $1\le N,Q\le 2\times 10^5$
- $1\le a,b\le N$、$-10^9\le w\le 10^9$
- 入力される値はすべて整数

### 入力

1行目に `N Q`、続く $Q$ 行に操作が1行ずつ与えられます。

### 出力

各操作に対する答えを、操作順に1行ずつ出力してください。

### 入力例

```text
4 7
1 1 2 4
1 2 3 -1
2 1 3
1 1 3 4
2 3 1
2 1 4
1 3 4 2
```

### 出力例

```text
Yes
Yes
3
No
-3
?
Yes
```

### 解法の要点

`consistent(a,b,w)` で採用可能かを調べ、可能なときだけ `merge(a,b,w)` します。`merge` 自体は矛盾を判定しません。質問では `same` を確認してから `diff(a,b)` を呼びます。頂点番号を0-indexedに変換してください。計算量は全体でならし $O((N+Q)\alpha(N))$ です。
