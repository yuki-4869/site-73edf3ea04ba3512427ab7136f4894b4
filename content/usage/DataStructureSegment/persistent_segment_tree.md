# 永続セグメント木

[説明書の目次](../index.md) · [コード](../../../DataStructureSegment/persistent_segment_tree.cpp)

点更新ごとに新しい根を返し、過去の版を保持できるセグメント木です。任意の過去の根から別の版へ分岐できます。
全要素が単位元の版は、配列長によらずO(1)で用意できます。関数合成や文字列連結などの非可換な演算にも対応します。

型エイリアスとコードの置き方は [共通設定](../common.md) を参照してください。
基本テンプレートの標準ヘッダーだけで使用でき、追加のincludeやACLへの依存はありません。

## 型・テンプレート引数

```cpp
template<class T, class F, int NODES = 10000000>
class PersistentSegmentTree;
```

Tは要素と集約値の型、Fは二項演算fの型です。fは結合則を満たし、IDは左右両方の単位元とします。可換性は不要です。
Tはコピー構築・代入可能な型とし、デフォルトコンストラクタは不要です。
f・更新関数g・述語checkはconstな呼出しに対応する必要があります。

**記号と前提**

- Nは1版の配列長で、`0 <= N <= LLONG_MAX`。添字は0-indexed、取得区間は半開区間 `[l,r)` です。
- NODESは全バージョンを合わせたノード数の上限で、1以上のコンパイル時定数です。番兵ノード1個を含みます。
- 根は同じpstのAPIが返したものを使用してください。別のpstの根、独自に作ったNode、破棄済みのpstの根は使用できません。
- Nodeの `data/l/r` は読み取り専用として扱い、直接変更しないでください。ノードの共有により、複数の過去の版へ影響します。
- fは呼出し順や外部状態によって結果が変わらないものとし、整数演算の中間結果も型に収めてください。
- 以下はTのコピー・f・g・checkがO(1)の場合の計算量です。文字列連結のように要素長に依存する演算では、そのコストが加わります。

## この実装の注意点

更新の返り値を捨てると、新しい版を参照できません。`root = pst.update(root,i,x)` のように保存するか、別の変数へ代入してください。
根ポインタのコピーはO(1)で版を共有します。本体のコピー・ムーブは禁止し、コンパイル時に検出します。
pstを破棄すると、そのpstが返したすべての根が無効になります。

ノードはdequeへ必要な分だけ追加し、追加前にNODESを検査します。増設しても既存ノードのアドレスは変わりません。
既定のNODES=10,000,000は予約数ではなく上限なので、構築時に1,000万個のNodeを初期化することはありません。

## コンストラクタ

```cpp
PersistentSegmentTree<T,F,NODES> pst(ll N, const F& f, const T& ID);
```

配列長、集約演算、単位元を保存し、全要素がIDの共有根を用意します。N=0も使用できます。
IDとfは本体へコピーします。fが参照キャプチャを持つ場合、その参照先の寿命にも注意してください。

**計算量**

- 時間・初期領域O(1)。番兵1個と、コンテナの管理領域を確保します。

## PersistentSegmentTree::size

```cpp
ll size() const;
```

1版の配列長Nを返します。O(1)です。

## PersistentSegmentTree::nodes_used

```cpp
int nodes_used() const;
```

番兵を含む、確保済みのノード数を返します。初期値は1です。O(1)です。
ノードは個別には解放されず、根を変数から捨てても消費数は減りません。

## PersistentSegmentTree::new_tree

```cpp
Node* new_tree() const;
```

全要素がIDの版の根を返します。呼び出すたびに同じ共有根を返し、追加ノードは消費しません。
配列全体を初期化する処理ではなく、既存の版や消費ノード数もリセットしません。O(1)です。

## PersistentSegmentTree::build

```cpp
Node* build(const vector<T>& v);
```

`v.size() == N` の列から新しい根を構築して返します。既存の版は残ります。
N=0では `new_tree()` と同じ根を返します。

**計算量と消費ノード数**

- 時間O(N+1)。N>0では2N-1個、N=0では0個のノードを追加します。
- 初回のN>0のbuild後は、番兵と合わせて `nodes_used() == 2*N` です。

## PersistentSegmentTree::update

```cpp
Node* update(Node* root, ll k, const T& x);
```

rootが表す版の位置kだけをxに置き換え、新しい根を返します。`0 <= k < N` が必要です。
元の根と、その根からすでに分岐した他の版は変更しません。

**計算量と消費ノード数**

- 償却O(log(N+1))時間。根から葉までの経路をコピーし、最大で $1+\lceil\log_2 N\rceil$ 個のノードを追加します。
- xが現在値と同じ場合も、新しい経路を確保します。

## PersistentSegmentTree::update_apply

```cpp
template<class G>
Node* update_apply(Node* root, ll k, const G& g);
```

rootの位置kの値を `g(元の値)` に置き換えた、新しい根を返します。`0 <= k < N` が必要です。
全要素IDの版で未作成の位置に適用する場合、gへIDを渡します。gは1回だけ呼び出します。
元の値を作業用変数へコピーしてgに渡します。gが参照引数を書き換えても、古い版や共有の番兵は変更されません。
gは更新後の値をTとして返してください。

**計算量と消費ノード数**

- 償却O(log(N+1))時間に、gの評価コストを加えたもの。消費ノード数はupdateと同じです。

## PersistentSegmentTree::query

```cpp
T query(Node* root, ll l, ll r) const;
```

rootが表す版の `[l,r)` を、添字の小さい順にfで集約した値を返します。
`0 <= l <= r <= N` が必要です。空区間ではIDを返します。
未作成の部分木は、全要素IDの区間として扱います。

**計算量**

- O(log(N+1))。空区間や全要素IDの共有根への取得はO(1)。ノードは追加しません。

## PersistentSegmentTree::max_right

```cpp
template<class C>
ll max_right(Node* root, ll l, const C& check) const;
```

`check(query(root,l,r))` を満たす最大の右端rを返します。`0 <= l <= N` が必要です。
`l == N` の場合はNを返します。

**述語の前提**

- `check(ID) == true`。この条件は `l == N` でも検査します。
- rを伸ばしたとき、trueからfalseへ高々1回だけ変化すること。
- 副作用がなく、同じ値には同じ判定を返すこと。

例えば非負整数の区間和に対する `sum <= limit` は使用できます。負数を含む列では、この単調性は一般に成立しません。
集約は `f(左からの累積, 次の区間)` の順なので、非可換な演算でも並び順を保ちます。

**計算量**

- O(log(N+1))。ノードは追加しません。

## 使用例

実装をグローバルに置き、以下をmain内などで実行します。

```cpp
auto f=[](ll a,ll b){return a+b;};
PersistentSegmentTree<ll,decltype(f),1024> pst(3,f,0LL);
auto root0=pst.build(vl{1,2,3});
auto root1=pst.update(root0,1,10);
auto root2=pst.update_apply(root0,2,[](ll x){return x+5;});
assert(pst.query(root0,0,3)==6);
assert(pst.query(root1,0,3)==14);
assert(pst.query(root2,0,3)==11);
assert(pst.query(root0,1,1)==0);
assert(pst.max_right(root1,0,[](ll sum){return sum<=11;})==2);
auto blank=pst.new_tree();
assert(pst.query(blank,0,3)==0);
auto root3=pst.update(blank,1,7);
assert(pst.query(root3,0,3)==7);
assert(pst.size()==3 && pst.nodes_used()<=1024);

PersistentSegmentTree<ll,decltype(f),1> empty(0,f,0LL);
auto e=empty.build(vl{});
assert(empty.query(e,0,0)==0);
assert(empty.max_right(e,0,[](ll sum){return sum==0;})==0);
```

root1とroot2はroot0から分岐した別の版です。どの根からでも、さらに更新できます。

## メモリ

確保済みノード数をUとすると、保持領域はO(U)。本体の破棄はO(U)です。
N>0のbuildを1回、その後Q回の点更新を行う場合、必要なノード数の上限は

$$
2N+Q\left(1+\lceil\log_2 N\rceil\right)
$$

です。全要素IDの根から始める場合は、先頭の2Nを1に置き換えます。
別の列からbuildを繰り返す場合は、それぞれの2N-1個も加算してください。
T=long longのNodeは一般的な64ビット環境で24バイトですが、実際の確保領域にはdequeのブロック・管理領域も加わります。

## 入力検査と旧版からの移行

負のN、buildの長さ不一致、添字・区間・探索開始位置の範囲違反、nullptrの根、`check(ID) == false`、ノード容量超過はメッセージなしで停止します。
これらの検査はNDEBUGでも有効です。別のpstの根など、nullptr以外の無効なポインタの所有元・寿命までは検査しません。

| 旧版 | 現行版 |
| --- | --- |
| build / update / update_apply / query / max_right / new_tree | 基本の呼び方を維持 |
| 木本体をコピーして同じpoolを共有 | コピー・ムーブはコンパイルエラー。根ポインタをコピーする |
| NODES個を先に確保し、容量超過時は未定義動作 | 必要なノードだけを確保し、追加前に上限を検査 |
| N>=1のみ | N=0も対応。更新はできず、空区間取得・探索が可能 |
| Nやpidなどの内部フィールドを参照 | `size()`・`nodes_used()` を使用。pool管理と生成補助関数は非公開 |
| `(l+r)/2` による中点 | `l+(r-l)/2` とし、大きな添字の加算オーバーフローを回避 |

## 使用例の確認状況

C++23で、三値列の小規模全列挙、任意の過去の版からの64,000回の分岐更新、通常配列による区間和・max_rightとの比較を行っています。
文字列連結による順序と境界探索、N=0・1・LLONG_MAX、容量ちょうど・容量超過、コピー禁止、デフォルト構築できない型、ノードの破棄も検証しています。
2026-10-22に通常ビルド・NDEBUG・AddressSanitizer/UBSanで、それぞれ約185万件の検査を通過しました。
専用テストはサイト用リポジトリの `tests/persistent_segment_tree_test.cpp` にあり、サイト更新時にも実行します。
