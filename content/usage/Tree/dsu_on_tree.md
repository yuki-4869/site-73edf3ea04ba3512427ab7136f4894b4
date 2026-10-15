# DSU on Tree

[説明書の目次](../index.md) · [コード](../../../Tree/dsu_on_tree.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Tree/dsu_on_tree.cpp)

部分木の頂点を追加・削除するコールバックで、全頂点の部分木クエリをまとめて処理します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

add(v)=頂点を追加、del(v)=頂点を削除、answer(v)=部分木全体が入った状態で答えを保存。

**共通の前提**

- N>=1、gは無向木。構築時とrun時に同じrootを指定。add/delは対になる操作で、run前の管理状態は空にします。

## 型・テンプレート引数

```cpp
struct DSUonTree;
```

## この実装の注意点

再帰DFSを使います。長いパス状の木では実行環境のスタック上限に注意してください。

## コンストラクタ

```cpp
DSUonTree(vector<vector<ll>> g,ll root=0);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

## DSUonTree::run

```cpp
void run(Add add,Del del,Ans answer,ll root=0);
```

各頂点vでanswer(v)を呼びます。終了後は根の部分木（木全体）が管理状態に残ります。コールバックは値渡しですが、外部状態は参照キャプチャできます。

**計算量**

- O(N log N)回のadd/del＋answerの総時間。

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `tin[v],tout[v]` | 部分木のEuler半開区間 |
| `euler[i]` | Euler順の頂点 |
| `heavy[v]` | 最も大きい子部分木の根 |
| `sz[v]` | 部分木頂点数 |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
graph g={{1,2},{0},{0}}; DSUonTree d(g);
ll cnt=0; vl ans(3);
d.run([&](ll v){cnt++;},[&](ll v){cnt--;},[&](ll v){ans[v]=cnt;});
assert((ans==vl{3,1,1}));
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
