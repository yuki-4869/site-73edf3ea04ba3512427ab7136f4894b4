# Virtual Tree

[説明書の目次](../index.md) · [コード](../../../Tree/virtual_tree.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Tree/virtual_tree.cpp)

指定頂点と必要なLCAだけを含む圧縮木を構築します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

K=入力頂点数、C_LCA=LCA1回の時間。

**共通の前提**

- tin,depは同じ根の木に対するDFS順と深さ。lca(a,b)は同じ木のLCAを返す関数。vの各要素は元の木の有効な頂点番号。

## 型・テンプレート引数

```cpp
struct VirtualTreeResult;
```

## この実装の注意点

この例はHLDにも依存します。圧縮木の辺長は元のdepthや距離関数から別途求めます。

## VirtualTree

```cpp
VirtualTreeResult VirtualTree(vector<ll> v, const vector<ll>& tin,const vector<ll>& dep,F lca);
```

VirtualTreeResult{vertex,edge,root}を返します。頂点番号は元の木の番号。edgeは親→子のpairで、1辺は元の木のパスに対応します。重みは含めません。空入力はroot=-1、列は空です。

**計算量**

- O(K log K+K×C_LCA)。

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `vertex` | 重複を除く圧縮木の頂点 |
| `edge` | 元の頂点番号による親子辺 |
| `root` | 圧縮木の根 |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
HLD h(4); h.add_edge(0,1); h.add_edge(1,2); h.add_edge(1,3); h.build();
auto t=VirtualTree(vl{2,3},vl(h.in.begin(),h.in.end()),vl(h.depth.begin(),h.depth.end()),[&](ll a,ll b){return h.lca(a,b);});
assert(t.root==1 && t.vertex.size()==3 && t.edge.size()==2);
```

## メモリ

O(K)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
