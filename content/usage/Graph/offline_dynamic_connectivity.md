# 動的連結性（オフライン）

[説明書の目次](../index.md) · [コード](../../../Graph/offline_dynamic_connectivity.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Graph/offline_dynamic_connectivity.cpp)

時刻ごとに追加・削除される無向辺について、連結判定をオフラインでまとめて処理します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

T=時刻数、I=辺が有効な区間数、C=連結判定クエリ数。追加時刻を含み、削除時刻を含まない [add,erase) に辺が存在します。

**共通の前提**

- N>=0、T>=1。0<=t<T、0<=u,v<N。同じ無向辺は存在中に再追加しないでください。eraseには存在する辺を指定します。solveは登録後に1回呼びます。

## 型・テンプレート引数

```cpp
struct OfflineDynamicConnectivity;
```

## この実装の注意点

元コメントのaddのO(1)表記とは異なり、辺の有効時刻はstd::mapで管理されています。時刻順にadd/eraseを登録してください。 基本テンプレートのp(s)マクロは内部のvector pの初期化p(n)と衝突します。このコードを貼り付ける前に #undef p を記述するか、出力マクロの名前を変更してください。

## コンストラクタ

```cpp
OfflineDynamicConnectivity(ll n,ll T);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N+T)。

## OfflineDynamicConnectivity::add

```cpp
void add(ll t,ll u,ll v);
```

時刻tから無向辺u-vを有効にします。返り値なし。

**計算量**

- O(log E)（mapへの登録）。

## OfflineDynamicConnectivity::erase

```cpp
void erase(ll t,ll u,ll v);
```

時刻tから辺u-vを無効にし、その有効区間を登録します。

**計算量**

- O(log E+log T)。

## OfflineDynamicConnectivity::query

```cpp
ll query(ll t,ll u,ll v);
```

時刻tの連結判定を登録し、答え配列内のIDを返します。

**計算量**

- ならしO(1)。

## OfflineDynamicConnectivity::solve

```cpp
vector<ll> solve();
```

全クエリの答えを登録ID順のvector<ll>で返します。連結なら1、非連結なら0です。

**計算量**

- O(N+T+I log T log N+C log N)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
OfflineDynamicConnectivity dc(3,3);
dc.add(0,0,1); dc.add(1,1,2);
auto a=dc.query(1,0,2);
dc.erase(2,1,2); auto b=dc.query(2,0,2);
auto ans=dc.solve(); assert(ans[a]==1 && ans[b]==0);
```

## メモリ

O(N+T+I log T+C)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

検証の際の準備:

- 出力マクロpとの衝突を避けるため#undef pを追加

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
