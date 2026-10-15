# LowLink

[説明書の目次](../index.md) · [コード](../../../Graph/lowlink.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Graph/lowlink.cpp)

無向グラフの橋・関節点と、DFS順およびLowLink値を求めます。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

M=辺数、id=add_edgeの返り値、v=頂点番号。

**共通の前提**

- N>=0。端点は0..N-1。自己ループ・多重辺・非連結グラフを許容。辺を追加した後はbuildを呼び直します。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [LowLink::add_edge](#api-2)
- [LowLink::build](#api-3)
- [LowLink::all_ord](#api-4)
- [LowLink::all_low](#api-5)
- [LowLink::connected_components](#api-6)
- [LowLink::is_articulation](#api-7)
- [LowLink::is_bridge](#api-8)

</details>

## 型・テンプレート引数

```cpp
struct LowLink;
```

<a id="api-1"></a>

## コンストラクタ

```cpp
LowLink();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
explicit LowLink(int n);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

<a id="api-2"></a>

## LowLink::add_edge

```cpp
int add_edge(int a, int b);
```

無向辺a-bを追加します。LCAで辺重みwを省略した場合は1です。LowLinkでは追加した辺のIDを返します。

**制約**

- 0<=a,b<N

**計算量**

- ならしO(1)。

<a id="api-3"></a>

## LowLink::build

```cpp
void build();
```

LowLink を構築

**計算量**

- O(N+M)。

<a id="api-4"></a>

## LowLink::all_ord

```cpp
vl all_ord() const;
```

ord の配列を vl で返す

**計算量**

- O(N)。

<a id="api-5"></a>

## LowLink::all_low

```cpp
vl all_low() const;
```

low の配列を vl で返す

**計算量**

- O(N)。

<a id="api-6"></a>

## LowLink::connected_components

```cpp
int connected_components() const;
```

連結成分数を返す

**計算量**

- O(1)。

<a id="api-7"></a>

## LowLink::is_articulation

```cpp
bool is_articulation(int v) const;
```

v が関節点なら true

**計算量**

- O(1)。

<a id="api-8"></a>

## LowLink::is_bridge

```cpp
bool is_bridge(int id) const;
```

辺 id が橋なら true

**計算量**

- O(1)。

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `ord[v]` | DFSで訪れた順 |
| `low[v]` | DFS部分木から後退辺を高々1本使って到達できる最小ord |
| `articulation_points` | 関節点一覧 |
| `bridge_ids` | 橋の辺ID |
| `bridges` | 橋の両端のpair |
| `dfs_edges` | DFS木の辺 |
| `dfs_edge_ids` | DFS木の辺ID |
| `edges[id]` | 追加した辺の両端 |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
LowLink g(3);
int e=g.add_edge(0,1); g.add_edge(1,2);
g.build();
assert(g.is_bridge(e) && g.is_articulation(1));
assert(g.connected_components()==1);
```

## メモリ

O(N+M)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
