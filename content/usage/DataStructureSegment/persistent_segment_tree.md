# 永続セグメント木

[説明書の目次](../index.md) · [コード](../../../DataStructureSegment/persistent_segment_tree.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/DataStructureSegment/persistent_segment_tree.cpp)

点更新ごとに新しい根を返し、過去の版を保持できるセグメント木です。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=1版の配列長、NODES=ノードプール容量。更新位置kは0..N-1、取得区間[a,b)は0<=a<=b<=N。

**共通の前提**

- N>=1、buildのvector長はN。T,Fはモノイドを構成しIDは単位元。すべての根は同じpstから得たもの。更新後に戻る根を保存します。

## 型・テンプレート引数

```cpp
template<typename T, typename F, int NODES = 10000000>
struct PersistentSegmentTree;
```

## この実装の注意点

update_applyの返り値を捨てると新しい版を参照できません。元の使用例のpst.update_apply(...);だけでは根が更新されません。デフォルトNODES=10,000,000は大きいため必要な容量を見積もって指定してください。容量超過を検知するassertはなく、超過すると未定義動作です。pstのコピーもプールの二重解放を招くため行わないでください。

## コンストラクタ

```cpp
PersistentSegmentTree(const ll &N_, const F &f_, const T &ID_);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(NODES)の配列確保（Tの構築コストにも依存）。

## PersistentSegmentTree::new_tree

```cpp
Node *new_tree();
```

全要素がIDの空の根を返します。新しい配列全体を確保する操作ではありません。

**計算量**

- O(1)。

## PersistentSegmentTree::build

```cpp
Node *build(const vector<T> &v);
```

vの内容で根を構築して返します。約2N個のプールノードを消費します。

**計算量**

- O(N)。

## PersistentSegmentTree::update

```cpp
Node *update(Node *n, ll k, const T &x);
```

根nの位置kをxにした新しい根を返します。古い根は変更しません。

**計算量**

- O(log N)時間・新規ノード数。

## PersistentSegmentTree::update_apply

```cpp
Node *update_apply(Node *n, ll k, const G &g);
```

根nの位置kにgを作用させた新しい根を返します。

**計算量**

- O(log N)＋gの時間。

## PersistentSegmentTree::query

```cpp
T query(Node *n, ll a, ll b);
```

根nが表す版の区間[a,b)をfで順番に集約して返します。

**計算量**

- O(log N)。

## PersistentSegmentTree::max_right

```cpp
ll max_right(Node *n, ll l, const C &check);
```

check(query(n,l,r))がtrueとなる最大rを返します。

**制約**

- check(ID)==true、rを伸ばすとtrueからfalseへの単調性がある

**計算量**

- O(log N)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
auto f=[](ll a,ll b){return a+b;};
PersistentSegmentTree<ll,decltype(f),1024> pst(3,f,0LL);
auto r0=pst.build(vl{1,2,3}); auto r1=pst.update(r0,1,10);
assert(pst.query(r0,0,3)==6 && pst.query(r1,0,3)==14);
auto r2=pst.update_apply(r1,0,[](ll x){return x+5;});
assert(pst.query(r2,0,1)==6);
```

## メモリ

O(NODES×sizeof(Node))。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
