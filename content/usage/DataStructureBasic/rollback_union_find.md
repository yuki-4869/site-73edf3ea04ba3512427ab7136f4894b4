# Rollback Union-Find

[説明書の目次](../index.md) · [コード](../../../DataStructureBasic/rollback_union_find.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/DataStructureBasic/rollback_union_find.cpp)

経路圧縮を行わず、併合を履歴から取り消せるUnion-Findです。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

t=merge呼び出し回数に対応する履歴位置。

**共通の前提**

- N>=0、0<=a,b<N。undoは履歴が存在するとき。保存時刻tは0<=t<=time()。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [RollbackUnionFind::leader](#api-2)
- [RollbackUnionFind::merge](#api-3)
- [RollbackUnionFind::same](#api-4)
- [RollbackUnionFind::size](#api-5)
- [RollbackUnionFind::components](#api-6)
- [RollbackUnionFind::time](#api-7)
- [RollbackUnionFind::undo](#api-8)
- [RollbackUnionFind::rollback](#api-9)
- [RollbackUnionFind::rollback_all](#api-10)
- [RollbackUnionFind::groups](#api-11)

</details>

## 型・テンプレート引数

```cpp
struct RollbackUnionFind;
```

<a id="api-1"></a>

## コンストラクタ

```cpp
RollbackUnionFind();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
explicit RollbackUnionFind(int n);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

<a id="api-2"></a>

## RollbackUnionFind::leader

```cpp
int leader(int a) const;
```

a の代表元を返す

**計算量**

- O(log N)。

<a id="api-3"></a>

## RollbackUnionFind::merge

```cpp
bool merge(int a, int b);
```

併合に成功したらtrue、既に同じ成分ならfalse。どちらの場合も履歴を1件追加します。

**計算量**

- O(log N)。

<a id="api-4"></a>

## RollbackUnionFind::same

```cpp
bool same(int a, int b) const;
```

a,b が同じ連結成分なら true

**計算量**

- O(log N)。

<a id="api-5"></a>

## RollbackUnionFind::size

```cpp
int size(int a) const;
```

a の属する連結成分のサイズ

**計算量**

- O(log N)。

<a id="api-6"></a>

## RollbackUnionFind::components

```cpp
int components() const;
```

現在の連結成分数を返します。

**計算量**

- O(1)。

<a id="api-7"></a>

## RollbackUnionFind::time

```cpp
int time() const;
```

現在の履歴件数を返します。保存してrollbackへ渡せます。

**計算量**

- O(1)。

<a id="api-8"></a>

## RollbackUnionFind::undo

```cpp
void undo();
```

直前のmerge呼び出しを1回取り消します。失敗したmergeも1回として取り消します。

**計算量**

- O(1)。

<a id="api-9"></a>

## RollbackUnionFind::rollback

```cpp
void rollback(int t);
```

time()==tになるまで取り消します。将来の時刻へ進める操作ではありません。

**計算量**

- O(time()-t)。

<a id="api-10"></a>

## RollbackUnionFind::rollback_all

```cpp
void rollback_all();
```

初期状態まで巻き戻します。

**計算量**

- O(time())。

<a id="api-11"></a>

## RollbackUnionFind::groups

```cpp
vector<vector<int>> groups() const;
```

全連結成分を列挙

**計算量**

- O(N log N)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
RollbackUnionFind uf(3);
int t=uf.time(); uf.merge(0,1); uf.merge(0,1);
uf.undo(); assert(uf.same(0,1));
uf.rollback(t); assert(!uf.same(0,1) && uf.components()==3);
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
