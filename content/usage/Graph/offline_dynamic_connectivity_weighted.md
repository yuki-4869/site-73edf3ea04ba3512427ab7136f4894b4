# 動的連結性（重み付き・オフライン）

[説明書の目次](../index.md) · [コード](../../../Graph/offline_dynamic_connectivity_weighted.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Graph/offline_dynamic_connectivity_weighted.cpp)

辺の追加・削除に加え、頂点の値を変更し、各時刻の連結成分の和を取得します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

V=頂点数、Q=時刻数、I=辺・頂点重みの有効区間の総数。頂点重みwは代入値です。

**共通の前提**

- initial_ws.size()==V、Q>=1。0<=idx<Q、0<=s,t,v<V。イベントを時刻順に登録し、buildを1回、その後runを呼びます。同じ辺の重複追加は内部で参照回数を管理します。eraseは追加回数を超えて呼べません。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [UnionFindUndo::find](#api-2)
- [UnionFindUndo::unite](#api-3)
- [UnionFindUndo::undo](#api-4)
- [UnionFindUndo::change_weight](#api-5)
- [UnionFindUndo::undo_change_weight](#api-6)
- [UnionFindUndo::component_sum](#api-7)
- [OfflineDynamicConnectivity::insert](#api-8)
- [OfflineDynamicConnectivity::erase](#api-9)
- [OfflineDynamicConnectivity::update_weight](#api-10)
- [OfflineDynamicConnectivity::build](#api-11)
- [OfflineDynamicConnectivity::run](#api-12)

</details>

## 型・テンプレート引数

```cpp
struct UnionFindUndo;
struct OfflineDynamicConnectivity;
```

<a id="api-1"></a>

## コンストラクタ

```cpp
UnionFindUndo(int sz, const vector<long long>& initial_ws);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(V+Q)。

```cpp
OfflineDynamicConnectivity(int V, int Q, const vector<long long>& initial_ws);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(V+Q)。

<a id="api-2"></a>

## UnionFindUndo::find

```cpp
int find(int k);
```

Undo用Union-Findの代表元を返します。

**計算量**

- O(log V)。

<a id="api-3"></a>

## UnionFindUndo::unite

```cpp
bool unite(int x, int y);
```

Undo用Union-Findで成分を併合し、実際に併合できたかを返します。

**計算量**

- O(log V)。

<a id="api-4"></a>

## UnionFindUndo::undo

```cpp
void undo();
```

直前のunite呼び出し1回分を取り消します。

**制約**

- 履歴が空でない

**計算量**

- O(1)。

<a id="api-5"></a>

## UnionFindUndo::change_weight

```cpp
void change_weight(int k, long long x);
```

内部の頂点値を代入し、取り消し用履歴を保存します。

**計算量**

- O(log V)。

<a id="api-6"></a>

## UnionFindUndo::undo_change_weight

```cpp
void undo_change_weight();
```

直前のchange_weightを取り消します。

**制約**

- 重み変更履歴が空でない

**計算量**

- O(1)。

<a id="api-7"></a>

## UnionFindUndo::component_sum

```cpp
long long component_sum(int k);
```

現在の連結成分に属する頂点値の和を返します。runのコールバックからdc.uf.component_sum(v)として使います。

**計算量**

- O(log V)。

<a id="api-8"></a>

## OfflineDynamicConnectivity::insert

```cpp
void insert(int idx, int s, int t);
```

無向辺s-tの参照回数を1増やします。0から1になった時刻から有効です。

**計算量**

- O(log E)。

<a id="api-9"></a>

## OfflineDynamicConnectivity::erase

```cpp
void erase(int idx, int s, int t);
```

辺の参照回数を1減らし、0になれば有効区間を閉じます。

**計算量**

- O(log E)。

<a id="api-10"></a>

## OfflineDynamicConnectivity::update_weight

```cpp
void update_weight(int idx, int v, long long w);
```

時刻idxから頂点vの値をwへ代入します。加算ではありません。

**計算量**

- O(log Q)。

<a id="api-11"></a>

## OfflineDynamicConnectivity::build

```cpp
void build();
```

未終了の辺・頂点値の区間を時間セグメント木へ登録します。

**計算量**

- O(I log Q)。

<a id="api-12"></a>

## OfflineDynamicConnectivity::run

```cpp
void run(const function<void(int)> &f, int k = 0, int l = 0, int r = -1);
```

各時刻でf(t)を呼び、その間だけufに当該時刻の状態を構築します。再帰用引数k,l,rは通常指定しません。

**計算量**

- O((V+Q+I log Q) log V + コールバックの総時間)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
OfflineDynamicConnectivity dc(3,3,vl{1,2,4});
dc.insert(0,0,1); dc.update_weight(1,1,10); dc.erase(2,0,1);
dc.build(); vl ans(3);
dc.run([&](int t){ ans[t]=dc.uf.component_sum(0); });
assert((ans==vl{3,11,1}));
```

## メモリ

O(V+Q+I log Q)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
