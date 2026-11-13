# Link-Cut Tree（遅延更新）

[説明書の目次](../index.md) · [コード](../../../Tree/lazy_link_cut_tree.cpp)

動的な森のパス遅延更新・非可換集計・境界探索、加算和／代入最小の設定済み型を提供します。各版は独立しており、このファイル1本を基本テンプレートのグローバルに貼り付けて使えます。
追加include・別ライブラリは不要です。基本テンプレートの `ll=long long` と標準ライブラリを使います。

## 版の選び方

| 版 | 用途 | 演算の設定 |
| --- | --- | --- |
| [Link-Cut Tree（基本版）](link_cut_tree_basic.md) | 動的な森の辺追加削除・連結判定・根指定LCA・パス上の頂点取得 | 不要 |
| [Link-Cut Tree（パス集計）](link_cut_tree.md) | 動的な森の一点更新・非可換パス集計・境界探索、総和／最小／最大の設定済み型 | S,op,e |
| [Link-Cut Tree（遅延更新）](lazy_link_cut_tree.md) | 動的な森のパス遅延更新・非可換集計・境界探索、加算和／代入最小の設定済み型 | S,op,e,F,mapping,composition,id |

木の形が固定されている場合は [LCA](lca.md) や [HLD](hld.md) も選べます。LCTは辺の付け替えが必要な場合に使います。

## 演算・構築の設定

```cpp
LazyLinkCutTree<S,op,e,F,mapping,composition,id> lct;
LazyLinkCutTree<S,op,e,F,mapping,composition,id> lct(N);
LazyLinkCutTree<S,op,e,F,mapping,composition,id> lct(values);
```

Sは頂点値・集約値の型、opは結合的な二項演算、eはその単位元を返す関数です。引数・戻り値はACLのsegtreeと同じ感覚で指定します。
N構築では各頂点の値がe()になります。valuesはvector<S>で、構築後に入力を保持しません。値や中間演算が型の範囲に収まる必要があります。

### 写像・タグの条件

Fは更新タグ、mapping(f,x)は集約値xへfを適用した結果、id()は恒等写像です。
composition(f,g)は**先にg、後にf**を適用するタグを返します。ACLのlazy_segtreeと同じ順序です。
`mapping(id(),x)=x`、`mapping(composition(f,g),x)=mapping(f,mapping(g,x))` が必要です。
さらに、非空の集約値a,bについて `mapping(f,op(a,b))=op(mapping(f,a),mapping(f,b))` が必要です。
すべての頂点に同じ写像を適用し、パス反転とも両立する更新に限ります。パス上の位置に依存する等差数列加算などは扱いません。

区間加算・区間和では、集約値に `{sum,len}` を持たせ、mappingは `{sum+f*len,len}` とします。
汎用版のN構築はe()の値をN個並べるため、e()のlenが0なら各頂点のlenも0です。
実際の頂点として数える場合はlen=1の配列から構築するか、以下の `LCTAddSum` を使ってください。

### 設定済みの型

| 型 | 構築時の配列 | N構築 | 更新タグ | get/prodの返り値 |
| --- | --- | --- | --- | --- |
| `LCTAddSum` | vector<ll> | 全頂点0 | llの加算値 | ll |
| `LCTAssignMin` | vector<ll> | 全頂点LLONG_MAX | optional<ll>の代入値 | ll |

`LCTAddSum` は内部の `{sum,len}` を隠し、setもll、max_rightの述語もllの総和を受け取ります。
`LCTAssignMin` はnulloptなら変更せず、値が入っていれば代入します。llの全範囲を代入値に使えるため、番兵値との衝突はありません。
両方とも、値・総和・タグ合成・加算値と頂点数の積がllに収まる範囲で使います。

## 型・前提

- 頂点番号は `0..N-1`、Nは0以上INT_MAX以下です。必要なメモリを確保できることが前提です。
- 構築直後は辺のない森です。辺は `link(u,v)` で登録します。自己辺・多重辺・閉路は作れません。
- 頂点の追加・削除はしません。辺の追加・削除にはオンラインで対応します。
- 取得操作も内部の補助木・内部の根を変更するため、`size()` 以外はconstではありません。
- 根付きの操作はすべてrootを引数に取ります。前の操作が内部の根を変えても、返す頂点の意味は変わりません。
- 再帰は使いません。Splay Treeの伝播にはヒープ上の作業用vectorを使います。
- 範囲外の頂点番号、不正なNは、NDEBUGでもメッセージなしで停止します。
- コピーは森の状態を独立に複製します。永続データ構造のようなO(1)の版保存ではありません。
- 部分木・連結成分の値の集計や更新、閉路を許す一般グラフの動的連結性は扱いません。

## 構築・計算量

既定構築は頂点数0、Nを渡す構築はN個の孤立頂点です。
構築・コピーはO(N)時間、保持領域・伝播用作業領域はO(N)です。空の構築はO(1)です。
`size()` はO(1)、以下の木の操作はすべて**償却O(log N)**時間です。1回だけではO(N)になる場合があります。
集計・写像・合成・述語・値のコピーがある版では、それらがO(1)である場合の計算量です。

## 森の共通API

以下の頂点番号の型・返される頂点番号・距離・個数はllです。返す頂点がなければ-1です。

| 呼び出し | 返り値 | 内容・必要な条件 |
| --- | --- | --- |
| `size()` | ll | 頂点数N。辺数・成分サイズではありません |
| `link(u,v)` | bool | 別々の木なら辺を追加してtrue。既に連結ならfalse |
| `cut(u,v)` | bool | 辺u-vが存在すれば削除してtrue。自己辺・非隣接・非連結ならfalse |
| `same(u,v)` | bool | 同じ木に属するか。同じ頂点ならtrue |
| `path_size(u,v)` | ll | 両端を含むパス上の頂点数。非連結なら0 |
| `dist_edges(u,v)` | ll | パス上の辺数。非連結なら-1 |
| `kth(u,v,k)` | ll | uからvへ進むパス上のk番目。k=0はu、範囲外・非連結なら-1 |
| `jump(u,v,k)` | ll | kthと同じ |
| `lca(root,u,v)` | ll | rootを根とするu,vの最小共通祖先。3頂点が同じ木になければ-1 |
| `parent(root,v)` | ll | rootを根とするvの親。v=rootまたは非連結なら-1 |
| `kth_ancestor(root,v,k)` | ll | vからroot方向へk個上。k=0はv、範囲外・非連結なら-1 |

パスの両端はどちらも含みます。`path_size(v,v)=1`、`dist_edges(v,v)=0` です。
`link/cut` がfalseでも、辺の集合・頂点値は変わりません。補助木の形は変わることがあります。
rootを指定し直すことにより、木の辺を変えずに根を変えたときのLCAや親も取得できます。

## 一点更新・パス集計

| 呼び出し | 返り値 | 内容 |
| --- | --- | --- |
| `set(v,x)` | void | 頂点vの値をxへ置き換える |
| `get(v)` | S | 頂点vの現在の値のコピーを返す |
| `prod(u,v)` | S | uからvへ順に、両端を含むパス上の値をopで集計する |
| `max_right(u,v,g)` | ll | uからvへ進むパスのうち、gを満たす最長の接頭部分の頂点数を返す |

各操作は償却O(log N)時間です。`prod/max_right` の両端は連結している必要があります。
`max_right` は `g(e())=true` が必要で、パスの接頭部分を伸ばしたとき、判定がtrueからfalseへ高々1回変化するものとします。
同じ引数に対する判定は一定とし、述語内で同じLCTを操作しないでください。
非連結のパスや `g(e())=false` は、NDEBUGでもメッセージなしで停止します。

`max_right` の返り値は**頂点番号ではなく頂点数**です。空なら0、全体なら `path_size(u,v)` です。
返された長さをkとすると、最初に条件を満たさなくなる頂点は `kth(u,v,k)` で取得できます。全体が条件を満たすなら-1です。
例えば非負の頂点値のパス和に対する `sum<=limit` が使えます。負の値があると単調性を満たさない場合があります。

### 非可換演算

内部では順方向・逆方向の集約値を両方保持します。行列積・関数合成のように順序が重要な演算にも対応します。
`op(a,b)` は「aの区間の後にbの区間をつなぐ」演算です。
関数をuからvへ順番に適用したい場合、一次関数の係数を `{a,b}` として、次の演算を指定します。

```cpp
auto op=[](pair<mint,mint> f,pair<mint,mint> g){
    return pair<mint,mint>{g.first*f.first,g.first*f.second+g.second};
};
auto e=[](){return pair<mint,mint>{1,0};};
```

この場合、prodで得た `{a,b}` は `a*x+b` として使います。`prod(u,v)` と `prod(v,u)` は一般に異なります。

## apply

```cpp
void lct.apply(ll v,const F& f);
void lct.apply(ll u,ll v,const F& f);
```

1つ目は頂点vだけ、2つ目はu-vパスの両端を含む全頂点にfを適用します。どちらも償却O(log N)です。
パス版ではu,vが連結している必要があり、非連結ならNDEBUGでもメッセージなしで停止します。
一点のapplyは、vから内部の根までのパス全体を変更する操作ではありません。
遅延タグを残した状態でlink/cut・根指定LCA・kth・set・境界探索を使えます。
setは、それまでの遅延更新を反映したうえで指定頂点だけを置き換えます。

## 使用例

```cpp
LCTAddSum lct(vector<ll>{1,2,3,4});
lct.link(0,1);lct.link(1,2);lct.link(1,3);
lct.apply(2,3,10);
assert(lct.prod(2,3)==39 && lct.get(1)==12);
lct.apply(0,5);
lct.set(1,7);
assert(lct.prod(0,2)==26);
auto length=lct.max_right(2,0,[](ll sum){return sum<=20;});
assert(length==2);
lct.cut(1,2);lct.link(0,2);
assert(lct.prod(2,3)==40);
LCTAssignMin assign(vector<ll>{8,3,6});
assign.link(0,1);assign.link(1,2);
assign.apply(0,2,optional<ll>{-5});
assert(assign.prod(0,2)==-5);
assign.apply(1,optional<ll>{9});
assert(assign.get(1)==9);
```

## 検証

C++23で、5頂点以下の全ての単純な森と愚直なパス探索を照合します。ランダムな辺の付け替え・失敗操作・根変更も検証します。
3種類を同時に貼り付けた構成、非可換な行列積と共役変換、非可換なアフィン更新の合成順、独立コピー、20万頂点のパス・星、不正入力の無出力停止を回帰テストに含めています。
追加した3ファイルはcode・codet・codecfと組み合わせた使用例も検証します。
