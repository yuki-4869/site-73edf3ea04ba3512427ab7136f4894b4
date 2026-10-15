# オイラー路・閉路

[説明書の目次](../index.md) · [コード](../../../Graph/euler_trail.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Graph/euler_trail.cpp)

すべての辺をちょうど1回使うオイラー路またはオイラー閉路の頂点列を返します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=n、M=e.size()。入力eは値渡しでコピーされます。

**共通の前提**

- n>=1。eの各端点は0..n-1。多重辺・自己ループを許容。directed=falseは無向、trueは有向。

## EulerTrail

```cpp
vector<ll> EulerTrail(ll n,vector<pair<ll,ll>> e,bool directed=false);
```

存在すれば長さM+1の頂点列を返します。全辺を使う路を構成できない場合は空vector。辺0本でn>=1の場合は {0}。列の順序は入力の辺順に依存します。

**計算量**

- O(N+M)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
vector<pair<ll,ll>> edges={{0,1},{1,2},{2,0}};
auto p=EulerTrail(3,edges);
assert(p.size()==4 && p.front()==p.back());
assert(EulerTrail(4,{{0,1},{2,3}}).empty());
```

## メモリ

O(N+M)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
