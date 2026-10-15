# Binary Lifting

[説明書の目次](../index.md) · [コード](../../../Graph/binary_lifting.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Graph/binary_lifting.cpp)

関数fによる繰り返し遷移と、各遷移で得る値の集約をダブリングで求めます。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=頂点数。k は unsigned long long の遷移回数（0..2^64-1）。a[v] はvから1回遷移する際の値。

**共通の前提**

- f.size()==a.size()。0<=f[v]<N。opは結合則を満たしeは両側単位元。opは遷移順に適用され、可換である必要はありません。

## 型・テンプレート引数

```cpp
template<class T>
struct BinaryLifting;
```

## コンストラクタ

```cpp
BinaryLifting(vector<ll> f,vector<T> a,T e,function<T(T,T)> op);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(64N)。

## BinaryLifting::query

```cpp
pair<ll,T> query(ll v,unsigned long long k);
```

vからk回進んだ頂点と、そのk回の値を順番にopで集約した結果のpairを返します。k=0なら {v,e}。

**制約**

- 0<=v<N

**計算量**

- O(log(k+1))、最大64段。

## BinaryLifting::jump

```cpp
ll jump(ll v,unsigned long long k);
```

query(v,k).first、すなわちk回後の頂点を返します。

**計算量**

- O(log(k+1))。

## BinaryLifting::prod

```cpp
T prod(ll v,unsigned long long k);
```

query(v,k).second、すなわちk回分の集約値を返します。

**計算量**

- O(log(k+1))。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
vl to={1,2,0}, cost={2,3,5};
BinaryLifting<ll> db(to,cost,0LL,[](ll a,ll b){return a+b;});
auto [v,sum]=db.query(0,4);
assert(v==1 && sum==12);
assert(db.jump(0,0)==0 && db.prod(0,0)==0);
```

## メモリ

O(64N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
