# 永続セグメント木

[説明書の目次](../index.md) · [コード](../../../DataStructureSegment/persistent_segment_tree.cpp)

点更新と区間積を扱いながら、過去の版を保存・分岐できるセグメント木です。
ACLの `segtree<S,op,e>` と同じ演算定義・操作名を使い、`auto old = seg;` で更新前の状態をO(1)で保存します。
履歴付き配列、木上の経路ごとの状態、過去の状態から分岐する問題などに使えます。

型エイリアスとコードの置き方は [共通設定](../common.md) を参照してください。
基本テンプレートの標準ヘッダーだけで使え、追加のincludeやACL本体への依存はありません。
区間更新が必要なら [永続遅延セグメント木](persistent_lazy_segment_tree.md) を使用してください。

## 型・テンプレート引数

```cpp
template<class S, auto op, auto e, int NODES = 10000000>
class PersistentSegmentTree;
```

- Sは要素・集約値の型、`op(S,S) -> S` は結合演算、`e() -> S` は単位元です。
- `op(op(a,b),c) == op(a,op(b,c))`、`op(e(),a) == op(a,e()) == a` が必要です。可換性は不要です。
- opとeには通常の関数、またはキャプチャなしのconstexprラムダを渡します。ACLの演算定義をそのまま使えます。
- Sはコピー構築・コピー代入が可能な型とします。デフォルト構築は不要です。ポインタなどを内部に持つ場合も、共有された値を外部から変更しないでください。
- NODESは同じノード領域を共有する全版の合計ノード上限です。1以上のコンパイル時定数で、初期確保数ではありません。

以下はNを配列長、Uを共有領域内の確保済みノード数とします。
`0 <= N <= LLONG_MAX`、添字は0-indexed、区間は半開区間 `[l,r)` です。
Sのコピー・演算・述語の評価をO(1)として計算量を記載します。演算は副作用がなく、整数の中間結果も型に収めてください。

## ACLとの対応

| ACL | この実装 |
| --- | --- |
| `atcoder::segtree<S,op,e> seg(v)` | `PersistentSegmentTree<S,op,e> seg(v)` |
| `set/get/prod/all_prod` | 同じ引数の並び・返り値・動作 |
| `max_right(l,g)` / `max_right<g>(l)` | 両方に対応 |
| `min_left(r,g)` / `min_left<g>(r)` | 両方に対応 |
| `auto old = seg;` | 全配列をコピーせず、O(1)で版を保存 |
| 要素数指定の構築O(N) | 全要素e()の仮想的な木をO(1)で構築 |
| `get(p)` はO(1) | O(log(N+1)) |
| 添字・境界の型はint | ll。intもそのまま渡せる |

クラス名の置換で通常のACL操作を使えます。更新メソッドの返り値はACLと同じvoidです。
版番号や根ポインタを各操作に渡す必要はありません。

## コンストラクタ

```cpp
PersistentSegmentTree<S,op,e,NODES> seg;
PersistentSegmentTree<S,op,e,NODES> seg(ll N);
PersistentSegmentTree<S,op,e,NODES> seg(const vector<S>& v);
```

引数なしでは長さ0、Nを指定すると全要素e()の列、vを指定するとvのコピーで初期化します。
N=0・空のvectorも使用できます。構築後にvを変更しても木の値は変わりません。

**計算量・追加ノード数**

- 引数なし・N指定は時間・初期領域O(1)、ノード0個。
- v指定は時間・領域O(N+1)。N>0なら2N-1個、N=0なら0個のノードを確保します。

## コピー・代入・ムーブ

```cpp
auto old = seg;
auto branch = old;
seg = old;
auto moved = std::move(branch);
```

コピー先は同じ時点の配列を表し、ノード領域を共有します。
その後のsetは呼び出したオブジェクトだけを更新し、他の版を変更しません。
元のオブジェクトを破棄しても、コピーが残っていればその版は使えます。

ムーブ先は元の版を引き継ぎ、ムーブ元は長さ0になります。
ムーブ元には空区間のprod・all_prod・境界探索・size・nodes_usedと、別の版の代入が可能です。

コピー構築・ムーブ構築はO(1)です。コピー代入・ムーブ代入はO(1)に、代入先が以前保持していた領域の解放コストが加わります。
共有領域の最後の所有者を破棄・上書きするときは、その領域のU個のノードをO(U)で解放します。

## PersistentSegmentTree::set

```cpp
void set(ll p, S x);
```

現在の版のa[p]をxに置き換えます。返り値はありません。`0 <= p < N` が必要です。
旧版を残す場合はsetの前に木をコピーしてください。
値を加算したい場合は `seg.set(p,seg.get(p)+delta)` のようにします。

**計算量**

償却O(log(N+1))。根から葉までをコピーし、最大 $1+\lceil\log_2 N\rceil$ 個のノードを追加します。
同じ値の代入でも経路を確保します。

## PersistentSegmentTree::get

```cpp
S get(ll p) const;
```

a[p]を値として返します。`0 <= p < N` が必要です。未作成の位置はe()です。
O(log(N+1))時間、追加ノードは0個です。

## PersistentSegmentTree::prod

```cpp
S prod(ll l, ll r) const;
```

a[l]からa[r-1]までを、添字の小さい順にopで集約して返します。
`0 <= l <= r <= N` が必要です。空区間ではe()を返します。
O(log(N+1))時間、追加ノードは0個です。空区間はO(1)です。

## PersistentSegmentTree::all_prod

```cpp
S all_prod() const;
```

`prod(0,size())` と同じ値をO(1)で返します。空列ではe()です。追加ノードは0個です。

## PersistentSegmentTree::max_right

```cpp
template<bool (*g)(S)>
ll max_right(ll l) const;
template<class G>
ll max_right(ll l, G g) const;
```

`g(prod(l,r))` がtrueとなる最大の右端rを返します。`0 <= l <= N` が必要です。
`l == N` ではNです。

gは同じ入力に同じ判定を返し、副作用を持たず、`g(e()) == true` を満たす必要があります。
rを右へ伸ばすにつれtrueからfalseへ高々1回だけ変化することを前提とします。
例えば非負整数の区間和に対する `sum <= limit` が使えます。負数を含む和では一般に使えません。

O(log(N+1))時間、追加ノードは0個です。非可換演算でも `op(累積値,次の区間)` の順を保ちます。

## PersistentSegmentTree::min_left

```cpp
template<bool (*g)(S)>
ll min_left(ll r) const;
template<class G>
ll min_left(ll r, G g) const;
```

`g(prod(l,r))` がtrueとなる最小の左端lを返します。`0 <= r <= N` が必要です。
`r == 0` では0です。

gにはmax_rightと同じ純粋性と `g(e()) == true` が必要です。
lを左へ伸ばすにつれtrueからfalseへ高々1回だけ変化することを前提とします。
O(log(N+1))時間、追加ノードは0個です。集約順は `op(次の左区間,累積値)` です。

## PersistentSegmentTree::size

```cpp
ll size() const;
```

この版の配列長NをO(1)で返します。

## PersistentSegmentTree::nodes_used

```cpp
int nodes_used() const;
```

コピーを通じて共有している領域の確保済みノード数UをO(1)で返します。
N指定・空列の構築直後は0です。別の版を更新すると、この版から見えるUも増えます。
版を捨てても個別のノードは回収しないため、最後の共有先を破棄するまでUは減りません。

## 使用例

実装をグローバルに置き、以下をmain内などで実行します。

```cpp
constexpr auto op=[](ll a,ll b){return a+b;};
constexpr auto e=[](){return 0LL;};
using Seg=PersistentSegmentTree<ll,op,e,1024>;
Seg seg(vl{1,2,3});
auto old=seg;
seg.set(1,10);
auto branch=old;
branch.set(2,branch.get(2)+5);
assert(seg.get(1)==10);
assert(seg.prod(0,3)==14);
assert(seg.all_prod()==14);
assert(old.all_prod()==6);
assert(branch.all_prod()==11);
assert(seg.prod(1,1)==0);
assert(seg.max_right(0,[](ll x){return x<=11;})==2);
assert(seg.min_left(3,[](ll x){return x<=13;})==1);

vector<Seg> versions{old,seg,branch};
versions.push_back(versions[0]);
versions.back().set(0,7);
assert(versions[0].get(0)==1);
assert(versions.back().all_prod()==12);

Seg blank(1000000000000LL);
blank.set(999999999999LL,7);
assert(blank.all_prod()==7);
Seg empty;
assert(empty.size()==0 && empty.prod(0,0)==0);
```

## 実装とメモリ

更新対象の経路だけを新しく作り、変更のない部分木は共有します。ノードは外から触れない不変データとして扱います。
共有領域はshared_ptrで寿命を管理し、dequeの追加で既存ノードのアドレスが変わらないことを利用します。
未作成の部分木は全要素e()として扱うため、巨大な添字範囲でも疎に使えます。

N>0のvectorから構築してQ回更新する場合、必要なノード数の上限は

$$
2N-1+Q\left(1+\lceil\log_2 N\rceil\right)
$$

です。N指定で始める場合は先頭の2N-1を0に置き換えます。
保持領域は共有全体でO(U)、版をV個保存するための管理領域はO(V)です。
S=llのNodeは一般的な64ビット環境で24バイトですが、dequeなどの管理領域も必要です。
独立に構築した木は別領域を持ち、別々にNODESを数えます。

## 入力検査と旧APIからの移行

負のN、範囲外の添字・区間・探索位置、`g(e()) == false`、ノード容量超過はメッセージなしで停止します。
NDEBUGでも検査は有効です。単位元の述語検査は探索対象が空でも行います。

従来の根ポインタ方式からAPIを変更しました。

| 以前 | 現在 |
| --- | --- |
| `PersistentSegmentTree<T,decltype(f)> pst(N,f,ID)` | `PersistentSegmentTree<S,op,e> seg(N)` |
| `auto root = pst.build(v)` | `Seg seg(v)` |
| `auto blank = pst.new_tree()` | `Seg blank(N)` |
| `auto next = pst.update(root,p,x)` | `auto next = root; next.set(p,x);`。rootも木オブジェクトに置き換える |
| `pst.update_apply(root,p,g)` | `auto next=root; next.set(p,g(next.get(p)));` |
| `pst.query(root,l,r)` | `root.prod(l,r)` |
| `pst.max_right(root,l,g)` | `root.max_right(l,g)` |
| 本体のコピー・ムーブ禁止、根だけを共有 | 木オブジェクトをコピー・ムーブして版を保持 |
| 番兵を含めてノード数を計上 | 番兵なし。N指定の初期ノード数は0 |

## 確認結果

C++23で小規模三値列の全区間、二値列の左右境界探索、48,000回の過去の版からの分岐更新を通常配列・ACLと比較しています。
文字列連結による非可換な集約と左右探索、N=0・1・LLONG_MAX、容量境界、コピー・ムーブ、元の版の破棄後の利用、デフォルト構築できない型も検証しています。
専用テストはサイト用リポジトリの `tests/persistent_segment_tree_test.cpp` です。

## 参考

- [ACL segtree](https://atcoder.github.io/ac-library/production/document_ja/segtree.html)：演算定義、操作名、境界探索の前提。
