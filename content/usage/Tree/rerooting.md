# 全方位木DP

[説明書の目次](../index.md) · [コード](../../../Tree/rerooting.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Tree/rerooting.cpp)

各頂点を根とした木DPの答えを、1回の全方位DPで計算します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

DP=方向ごとの情報、E=辺情報、merge(DP,DP)=結合、lift(DP,E,from,to)=辺を渡す変換、add_vertex(DP,v)=頂点自身を加える操作。

**共通の前提**

- N>=1、無向木。mergeは結合則を満たしidentityは単位元。liftとadd_vertexは型DPを返すこと。

## 型・テンプレート引数

```cpp
template<
    class DP,
    class E,
    class Merge,
    class Lift,
    class AddVertex
>
struct Rerooting;
```

## この実装の注意点

ファイル後半のDP/E/merge/identity/lift/add_vertexは穴埋め用の記入例です。identity()にreturn ;が残っているため、ファイル全体はそのままコンパイルできません。Rerootingとmake_rerootingの定義を使用し、問題固有の型・演算を自分で用意してください。例は木上の最遠距離です。

## コンストラクタ

```cpp
Rerooting( int n, DP identity, Merge merge, Lift lift, AddVertex add_vertex );
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

## Rerooting::add_edge

```cpp
int add_edge(int a, int b, const E& data);
```

辺データdata付きの無向辺a-bを追加し、辺IDを返します。

**計算量**

- ならしO(1)。

## Rerooting::build

```cpp
vector<DP> build(int root = 0);
```

全頂点についてDPを求め、answerのコピーを返します。rootは内部の探索根で、最終的な各頂点の答えの意味には影響しません。

**計算量**

- O(N×各コールバックの計算時間)。

## make_rerooting

```cpp
auto make_rerooting( int n, DP identity, Merge merge, Lift lift, AddVertex add_vertex );
```

ラムダ等の型からRerootingオブジェクトを構築して返します。DPとEは明示し、それ以外は推論します。

**計算量**

- O(N)。

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `answer[v]` | vを根とした答え |
| `subdp[v]` | buildの内部rootに対するvの部分木側DP |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
auto rr=make_rerooting<ll,int>(3,0LL,
 [](ll a,ll b){return max(a,b);},
 [](ll x,int w,int from,int to){return x+w;},
 [](ll x,int v){return x;});
rr.add_edge(0,1,1); rr.add_edge(1,2,1);
auto a=rr.build(); assert((a==vl{2,1,2}));
```

## メモリ

O(N+DP情報量)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

検証の際の準備:

- Rerooting/make_rerootingの定義を使用し、後半の未記入DP例を除外

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
