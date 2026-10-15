# 木の重心

[説明書の目次](../index.md) · [コード](../../../Tree/tree_centroid.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Tree/tree_centroid.cpp)

頂点を1つ削除した後の最大連結成分のサイズ（または重み）を最小にする重心を求めます。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=g.size()。辺重みは重心判定に影響しません。頂点重みを指定した場合は頂点数の代わりに重み和を使います。

**共通の前提**

- gは無向木または空。重み付き版はw.size()==g.size()、w[v]>=0。隣接リスト要素は頂点番号または{行き先,辺重み}。

## 型・テンプレート引数

```cpp
struct CentroidInfo;
```

## この実装の注意点

重みなしの非空の木では重心は通常1つまたは2つです。頂点重み0を許す版では3つ以上になる場合もあります。

## CentroidInfo::one

```cpp
ll one() const;
```

保存済みの重心を1つ返し、一覧が空なら-1。

**計算量**

- O(1)。

## CentroidInfo::is_centroid

```cpp
bool is_centroid(ll v) const;
```

頂点vが保存済みの重心かを返します。範囲外はfalse。

**計算量**

- O(1)。

## centroid_info

```cpp
CentroidInfo centroid_info(const G& g);
CentroidInfo centroid_info(const G& g,const vl& w);
```

CentroidInfoに重心一覧、根0の親と部分木集計、各頂点を削除したときの最大成分値を返します。

**計算量**

- O(N)。

## centroid

```cpp
vl centroid(const G& g);
vl centroid(const G& g,const vl& w);
```

重心の頂点番号をすべて返します。

**計算量**

- O(N)。

## centroid_one

```cpp
ll centroid_one(const G& g);
ll centroid_one(const G& g,const vl& w);
```

重心を1つ返し、空の木なら-1。

**計算量**

- O(N)。

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `centroids` | 全重心 |
| `parent[v]` | 根0での親 |
| `subtree[v]` | 部分木のサイズ／重み |
| `max_component[v]` | v削除後の最大成分値 |
| `balance` | max_componentの最小値 |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
graph g={{1},{0,2},{1}};
auto c=centroid_info(g);
assert(c.one()==1 && c.is_centroid(1) && c.balance==1);
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
