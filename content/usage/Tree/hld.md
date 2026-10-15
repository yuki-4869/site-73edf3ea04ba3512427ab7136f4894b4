# Heavy-Light Decomposition

[説明書の目次](../index.md) · [コード](../../../Tree/hld.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Tree/hld.cpp)

木のパスをO(log N)個の連続区間に分け、セグメント木などと組み合わせます。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

in[v]はEuler順、out[v]は部分木区間の右端。edge=trueは辺を集約するモード。

**共通の前提**

- 連結な無向木。0<=頂点<N。add_edge後にbuild(root)。辺の値は深い側の頂点のin位置に置きます。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [HLD::add_edge](#api-2)
- [HLD::build](#api-3)
- [HLD::lca](#api-4)
- [HLD::dist_edges](#api-5)
- [HLD::kth_ancestor](#api-6)
- [HLD::jump](#api-7)
- [HLD::is_ancestor](#api-8)
- [HLD::subtree](#api-9)
- [HLD::path](#api-10)
- [HLD::path_ordered](#api-11)

</details>

## 型・テンプレート引数

```cpp
struct HLD;
```

## この実装の注意点

非可換演算ではpath_orderedを使用し、反転方向の集約値も用意してください。

<a id="api-1"></a>

## コンストラクタ

```cpp
HLD();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
explicit HLD(int n);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

<a id="api-2"></a>

## HLD::add_edge

```cpp
void add_edge(int a, int b);
```

無向辺a-bを追加します。LCAで辺重みwを省略した場合は1です。LowLinkでは追加した辺のIDを返します。

**制約**

- 0<=a,b<N

**計算量**

- ならしO(1)。

<a id="api-3"></a>

## HLD::build

```cpp
void build(int root = 0);
```

root を根として HL 分解

**計算量**

- O(N)。

<a id="api-4"></a>

## HLD::lca

```cpp
int lca(int a, int b) const;
```

a,b の最小共通祖先

**計算量**

- O(log N)。

<a id="api-5"></a>

## HLD::dist_edges

```cpp
int dist_edges(int a, int b) const;
```

a-b 間の辺数

**計算量**

- O(log N)。

<a id="api-6"></a>

## HLD::kth_ancestor

```cpp
int kth_ancestor(int v, int k) const;
```

v から k 個上の祖先、存在しなければ -1

**計算量**

- O(log N)。

<a id="api-7"></a>

## HLD::jump

```cpp
int jump(int a, int b, int k) const;
```

a→b のパスを k 辺進んだ頂点、存在しなければ -1

**計算量**

- O(log N)。

<a id="api-8"></a>

## HLD::is_ancestor

```cpp
bool is_ancestor(int a, int b) const;
```

aがbの祖先か（自分自身を含む）を返します。

**計算量**

- O(1)。

<a id="api-9"></a>

## HLD::subtree

```cpp
pair<int, int> subtree(int v, bool edge = false) const;
```

vの部分木に対応する[l,r)を返します。edge=trueならv自身の位置を除き、部分木内部の辺だけを表します。

**計算量**

- O(1)。

<a id="api-10"></a>

## HLD::path

```cpp
vector<pair<int, int>> path(int a, int b, bool edge = false) const;
```

a-bパスを区間[l,r)の列で返します。区間の順序や向きは保証せず、和/min/maxなど可換演算向けです。

**計算量**

- O(log N)。

<a id="api-11"></a>

## HLD::path_ordered

```cpp
vector<tuple<int, int, bool>> path_ordered( int a, int b, bool edge = false ) const;
```

aからbの順に {l,r,reversed} を返します。reversed=falseはl→r-1、trueはr-1→lの順です。

**計算量**

- O(log N)。

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `in[v],out[v]` | 部分木の半開区間 |
| `rev[i]` | Euler位置から頂点への逆写像 |
| `parent[v]` | 親（根は-1） |
| `depth[v]` | 根からの辺数 |
| `size[v]` | 部分木頂点数 |
| `head[v]` | heavy pathの先頭 |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
HLD h(4); h.add_edge(0,1); h.add_edge(1,2); h.add_edge(1,3); h.build();
assert(h.lca(2,3)==1 && h.dist_edges(2,3)==2);
int len=0; for(auto [l,r]:h.path(2,3)) len+=r-l;
assert(len==3);
auto [l,r]=h.subtree(1); assert(r-l==3);
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
