# 永続遅延セグメント木

[説明書の目次](../index.md) · [コード](../../../DataStructureSegment/persistent_lazy_segment_tree.cpp)

区間更新と区間積を扱いながら、更新前の配列を保存し、任意の版から分岐できる遅延セグメント木です。
ACLの `lazy_segtree<S,op,e,F,mapping,composition,id>` と同じ演算定義・操作名を使います。
`auto old = seg;` で版をO(1)保存し、その後のset・applyはsegだけを更新します。

範囲への加算・代入・アフィン変換を行った各時点を残したい問題や、過去の状態から別の更新を試す問題に使えます。
点更新だけなら [永続セグメント木](persistent_segment_tree.md) のほうがノードが小さく、更新時の追加数も少なくなります。

型エイリアスとコードの置き方は [共通設定](../common.md) を参照してください。
基本テンプレートの標準ヘッダーだけで使え、追加のincludeやACL本体への依存はありません。

## 型・テンプレート引数

```cpp
template<class S, auto op, auto e, class F,
         auto mapping, auto composition, auto id, int NODES = 10000000>
class PersistentLazySegmentTree;
```

| 引数 | 意味 |
| --- | --- |
| S | 要素・区間集約値の型 |
| `op(S a,S b) -> S` | 左の区間aと右の区間bの結合 |
| `e() -> S` | opの左右の単位元 |
| F | 更新を表す型 |
| `mapping(F f,S x) -> S` | xへfを適用した結果 |
| `composition(F f,F g) -> F` | gを適用した後にfを適用する合成、$f\circ g$ |
| `id() -> F` | 何も変更しない更新 |
| NODES | コピーを通じて共有する全版の合計ノード上限。既定10,000,000、1以上 |

**必要な性質**

- opは結合的で、`op(e(),x) == op(x,e()) == x`。可換性は不要です。
- `mapping(id(),x) == x`。
- `mapping(composition(f,g),x) == mapping(f,mapping(g,x))`。合成順はACLと同じです。
- `mapping(f,op(x,y)) == op(mapping(f,x),mapping(f,y))`。更新は区間の分割と整合する必要があります。
- Fは合成に関して閉じ、idは恒等写像を表すものとします。`mapping(f,e()) == e()` は必須ではありません。
- SとFはコピー構築・コピー代入が可能な型とします。デフォルト構築・Fの等値比較は不要です。
- 演算は通常の関数、またはキャプチャなしのconstexprラムダで指定します。副作用を持たず、共有したS/Fの内部も外部から変更しないでください。

以下はNを配列長、Uを共有領域内の確保済みノード数とします。
`0 <= N <= LLONG_MAX`、添字は0-indexed、区間は半開区間 `[l,r)` です。
S/Fのコピー・各演算・述語の評価をO(1)として計算量を記載します。整数の中間結果も型に収めてください。

## ACLとの対応

| ACL | この実装 |
| --- | --- |
| `atcoder::lazy_segtree<S,op,e,F,mapping,composition,id>` | `PersistentLazySegmentTree<S,op,e,F,mapping,composition,id>` |
| `set/get/prod/all_prod` | 同じ引数の並び・返り値・動作 |
| `apply(p,f)` / `apply(l,r,f)` | 両方に対応 |
| `max_right(l,g)` / `max_right<g>(l)` | 両方に対応 |
| `min_left(r,g)` / `min_left<g>(r)` | 両方に対応 |
| N指定の構築 | 同じく全要素e()。こちらはO(1)で仮想的に構築 |
| `auto old=seg;` | O(1)で版を保存し、以降の更新から独立 |
| 読み取り時にも内部でpush | 取得・探索はconstで、既存ノードの変更・追加確保なし |
| 添字・境界の型はint | ll。intもそのまま渡せる |

クラス名を置き換えてACLと同じ操作を使えます。set・applyの返り値はvoidです。
版ごとの根ポインタを利用者が管理する必要はありません。

## コンストラクタ

```cpp
PersistentLazySegmentTree<S,op,e,F,mapping,composition,id,NODES> seg;
PersistentLazySegmentTree<S,op,e,F,mapping,composition,id,NODES> seg(ll N);
PersistentLazySegmentTree<S,op,e,F,mapping,composition,id,NODES> seg(const vector<S>& v);
```

引数なしは長さ0、N指定は全要素e()、v指定はvの内容で初期化します。
N=0・空のvectorも使えます。構築後にvを変えても木には影響しません。

区間和を `S{sum,len}` で管理する場合、`e() == S{0,0}` は長さ0です。
N個の実要素0を扱う場合は `vector<S>(N,S{0,1})` を渡してください。
`seg(N)` はACLと同じくN個のe()を置くので、後から区間加算しても長さ0のままです。

**計算量・追加ノード数**

- 引数なし・N指定は時間・初期領域O(1)、ノード0個。
- v指定は時間・領域O(N+1)。N>0なら2N-1個、N=0なら0個。

## コピー・代入・ムーブ

```cpp
auto old = seg;
auto branch = old;
seg = old;
auto moved = std::move(branch);
```

コピーは同じ時点の配列を表し、共有する不変ノードをO(1)で引き継ぎます。
任意のコピーにset・applyしても、他の版は変わりません。
元の木を破棄しても、コピーが残る限りその版を使用できます。

ムーブ先は元の版を引き継ぎ、ムーブ元は長さ0になります。
ムーブ元には空区間のprod・apply・境界探索、all_prod・size・nodes_usedと、別の版の代入が可能です。

コピー構築・ムーブ構築はO(1)です。コピー代入・ムーブ代入はO(1)に、代入先が以前保持していた領域の解放コストが加わります。
最後の共有先の破棄・上書きで、領域内のU個のノードをO(U)でまとめて解放します。

## PersistentLazySegmentTree::set

```cpp
void set(ll p, S x);
```

この版のa[p]をxで置き換えます。`0 <= p < N` が必要で、返り値はありません。
pに未適用の更新があればその効果を反映したうえで、最終的にxを格納します。
過去の版は変更しません。

償却O(log(N+1))時間、O(log(N+1))個のノードを追加します。

## PersistentLazySegmentTree::get

```cpp
S get(ll p) const;
```

a[p]に至る更新をすべて反映した値を返します。`0 <= p < N` が必要です。
O(log(N+1))時間、追加ノードは0個です。返り値は値のコピーです。

## PersistentLazySegmentTree::prod

```cpp
S prod(ll l, ll r) const;
```

`0 <= l <= r <= N` を満たす `[l,r)` の要素を、添字の小さい順にopで集約して返します。
保留中の更新も反映します。空区間では更新を適用せずe()を返します。
O(log(N+1))時間、追加ノードは0個です。空区間はO(1)です。

## PersistentLazySegmentTree::all_prod

```cpp
S all_prod() const;
```

全区間の集約値をO(1)で返します。保留中の更新も反映済みです。
空列ではe()です。追加ノードは0個です。

## PersistentLazySegmentTree::apply

```cpp
void apply(ll p, F f);
void apply(ll l, ll r, F f);
```

1点版は `a[p] = mapping(f,a[p])`、区間版は `[l,r)` の各要素へfを適用します。
返り値はありません。この版だけを更新し、コピー済みの版は変更しません。
1点版は `0 <= p < N`、区間版は `0 <= l <= r <= N` が必要です。

どちらも償却O(log(N+1))時間、追加ノード数O(log(N+1))です。
空区間はO(1)で、ノードを確保しません。全区間のapplyはO(1)でノードを1個だけ追加します。
fがid()と等しくても比較・省略しないので、空区間以外ではノードを消費します。

## PersistentLazySegmentTree::max_right

```cpp
template<bool (*g)(S)>
ll max_right(ll l) const;
template<class G>
ll max_right(ll l, G g) const;
```

`g(prod(l,r))` がtrueとなる最大の右端rを返します。`0 <= l <= N` が必要です。
`l == N` ではNです。

gは副作用がなく、同じ値に同じ結果を返し、`g(e()) == true` を満たす必要があります。
rを右へ伸ばすにつれtrueからfalseへ高々1回だけ変化することを前提とします。
非負の区間和に対する上限判定などが使えます。負値を許す和では単調性に注意してください。

O(log(N+1))時間、追加ノードは0個です。更新を論理的に反映し、集約順は `op(累積値,次の区間)` です。

## PersistentLazySegmentTree::min_left

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

## PersistentLazySegmentTree::size

```cpp
ll size() const;
```

この版の配列長NをO(1)で返します。

## PersistentLazySegmentTree::nodes_used

```cpp
int nodes_used() const;
```

同じ領域を共有する全版の確保済みノード数UをO(1)で返します。
N指定・空列の構築直後は0です。他の版の更新でも増え、版を捨てるだけでは減りません。
get・prod・all_prod・max_right・min_leftでは一切増えません。

## 使用例

区間アフィン変換 `x -> a*x+b` と区間和の例です。
実装をグローバルに置き、以下をmain内などで実行します。
この例はllの範囲に収まる値だけを扱います。必要ならS/Fの係数にmodintを使ってください。

```cpp
struct S{ll sum,len;};
struct F{ll a,b;};
constexpr auto op=[](S x,S y){return S{x.sum+y.sum,x.len+y.len};};
constexpr auto e=[](){return S{0,0};};
constexpr auto mapping=[](F f,S x){return S{f.a*x.sum+f.b*x.len,x.len};};
constexpr auto composition=[](F f,F g){return F{f.a*g.a,f.a*g.b+f.b};};
constexpr auto id=[](){return F{1,0};};
using Seg=PersistentLazySegmentTree<S,op,e,F,mapping,composition,id,4096>;
Seg seg(vector<S>{{1,1},{2,1},{3,1}});
auto old=seg;
seg.apply(0,2,F{1,5});
seg.apply(2,F{2,0});
assert(seg.get(0).sum==6);
assert(seg.all_prod().sum==19);
assert(old.all_prod().sum==6);

auto branch=old;
branch.set(1,S{10,1});
assert(branch.prod(0,3).sum==14);
assert(seg.max_right(0,[](S x){return x.sum<=13;})==2);
assert(seg.min_left(3,[](S x){return x.sum<=13;})==1);

auto before_assign=seg;
seg.apply(0,3,F{0,4});
assert(seg.all_prod().sum==12);
assert(before_assign.all_prod().sum==19);
int used=seg.nodes_used();
assert(seg.prod(1,3).sum==8);
assert(seg.nodes_used()==used);

Seg zeros(vector<S>(5,S{0,1}));
zeros.apply(0,5,F{1,3});
assert(zeros.all_prod().sum==15);
Seg empty;
empty.apply(0,0,F{1,2});
assert(empty.prod(0,0).sum==0 && empty.nodes_used()==0);
```

`F{1,b}` は加算、`F{a,0}` は乗算、`F{0,b}` は代入です。
`composition(f,g)` を逆に書くと、例えば「2倍して1を加えた後、3倍して4を加える」が正しく反映されません。

## 永続化と遅延伝播の仕組み

区間全体を更新するときは、集約値と遅延タグを持つ新しいノードを1個作ります。
一部だけ更新するときは、保留中のタグを子のコピーへ適用してから対象の経路を作り直します。
元のノードをpushで変更しないので、他の版には影響しません。

読み取りではpushを行わず、祖先の未反映タグを引数として持ち下ろします。
子のタグより祖先のタグのほうが新しいため、`composition(祖先の更新,子の更新)` の順で合成します。
部分木の集約値にはそのノード自身のタグがすでに反映されているので、二重に適用しません。
`mapping(f,e()) != e()` の更新でも、未作成の非空区間と空の取得区間を区別して扱います。

## メモリと入力検査

ノード領域はshared_ptrで寿命を管理し、dequeへ必要な分だけ追加します。
NODESは予約数ではなく上限です。独立に構築した木は別々の領域と上限を持ちます。

vector構築後にQ回更新したときの保持領域は、共有全体でO(N+Q log(N+1))です。
N指定で始める場合はO(1+Q log(N+1))です。V個の版の管理には別にO(V)を使います。
過去の版を捨ててもノードを個別に回収せず、最後の共有先の破棄で全て解放します。
遅延タグと子ポインタを各ノードに持ち、pushのために兄弟のコピーも作るので、通常の永続セグメント木よりメモリを消費します。
S/Fをそれぞれll2個で表す場合、一般的な64ビット環境のNodeは56バイトで、dequeなどの管理領域も加わります。

負のN、範囲外の添字・区間・探索位置、`g(e()) == false`、容量超過はメッセージなしで停止します。
NDEBUGでも検査します。述語の単位元検査は探索対象が空でも行います。

## 確認結果

C++23で小規模三値列の全区間更新と全区間取得を全列挙し、ACL・通常配列と比較しています。
アフィン変換48,000回と区間代入12,000回の過去の版からの分岐、非可換なタグ合成・文字列集約・左右探索も検証しています。
`mapping(f,e()) != e()`、Fに等値比較・デフォルト構築がない場合、N=0・1・LLONG_MAX、容量境界、コピー・ムーブ、元の版の破棄後の利用、読み取り時の追加確保0個を確認しています。
専用テストはサイト用リポジトリの `tests/persistent_lazy_segment_tree_test.cpp` です。

## 参考

- [ACL lazy_segtree](https://atcoder.github.io/ac-library/production/document_ja/lazysegtree.html)：演算定義、更新の合成順、全公開操作。
