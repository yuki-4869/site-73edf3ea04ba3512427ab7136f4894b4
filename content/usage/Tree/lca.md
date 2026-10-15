# LCA

[説明書の目次](../index.md) · [コード](../../../Tree/lca.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Tree/lca.cpp)

木の最小共通祖先、パスの距離、k番目の頂点をダブリングで求めます。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=頂点数、k=上に進む辺数またはパスを進む辺数。

**共通の前提**

- 木は連結・無向・自己ループなし。頂点0..N-1。辺追加後にbuild(root)。Tは辺重みの加減算を提供し、距離の中間値がTに収まること。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [LCA::add_edge](#api-2)
- [LCA::build](#api-3)
- [LCA::kth_ancestor](#api-4)
- [LCA::lca](#api-5)
- [LCA::dist_edges](#api-6)
- [LCA::dist](#api-7)
- [LCA::jump](#api-8)
- [LCA::is_ancestor](#api-9)

</details>

## 型・テンプレート引数

```cpp
template<class T = long long>
struct LCA;
```

## この実装の注意点

辺の重みを省略すると1です。kは辺数であり重みの和ではありません。未buildのオブジェクトで検索しないでください。

<a id="api-1"></a>

## コンストラクタ

```cpp
LCA();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
explicit LCA(int n);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N log(N+1))。

<a id="api-2"></a>

## LCA::add_edge

```cpp
void add_edge(int a, int b, T w = T{1});
```

無向辺a-bを追加します。LCAで辺重みwを省略した場合は1です。LowLinkでは追加した辺のIDを返します。

**制約**

- 0<=a,b<N

**計算量**

- ならしO(1)。

<a id="api-3"></a>

## LCA::build

```cpp
void build(int root = 0);
```

root を根として前計算

**計算量**

- O(N log N)。

<a id="api-4"></a>

## LCA::kth_ancestor

```cpp
int kth_ancestor(int v, long long k) const;
```

v から k 個上の祖先、存在しなければ -1

**計算量**

- O(log N)。

<a id="api-5"></a>

## LCA::lca

```cpp
int lca(int a, int b) const;
```

N 頂点で初期化 0-index

**計算量**

- O(N log N)。

<a id="api-6"></a>

## LCA::dist_edges

```cpp
int dist_edges(int a, int b) const;
```

a-b 間の辺数

**計算量**

- O(log N)。

<a id="api-7"></a>

## LCA::dist

```cpp
T dist(int a, int b) const;
```

a-b 間の辺重みの総和

**計算量**

- O(log N)。

<a id="api-8"></a>

## LCA::jump

```cpp
int jump(int a, int b, long long k) const;
```

a から b へのパスを k 辺進んだ頂点

**計算量**

- O(log N)。

<a id="api-9"></a>

## LCA::is_ancestor

```cpp
bool is_ancestor(int a, int b) const;
```

a が b の祖先なら true

**計算量**

- O(log N)。

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `depth[v]` | 根からの辺数 |
| `dist_root[v]` | 根からの辺重みの和 |
| `parent[k][v]` | 2^k個上の祖先 |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
LCA<ll> t(4);
t.add_edge(0,1,2); t.add_edge(1,2,3); t.add_edge(1,3,4); t.build(0);
assert(t.lca(2,3)==1 && t.dist(2,3)==7);
assert(t.jump(2,3,1)==1 && t.kth_ancestor(0,1)==-1);
```

## メモリ

O(N log(N+1))。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
