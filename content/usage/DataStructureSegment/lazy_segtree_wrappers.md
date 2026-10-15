# 遅延セグメント木の用途別ラッパー

[説明書の目次](../index.md) · [コード](../../../DataStructureSegment/lazy_segtree_wrappers.cpp) · [確認対象の改訂版](https://github.com/yuki-4869/Algorithm/blob/f9177dec185cbc540c81fa7c94c944c1e87348ac/DataStructureSegment/lazy_segtree_wrappers.cpp)

ACLのlazy_segtreeを、区間加算・代入・一次変換・0/1反転などで使う17種類のラッパーです。
2026-10-15改訂。末尾コメントのコンパイルエラーを修正し、構築方法・境界検査・空の木の扱いを統一しました。

**記号**

- N: 配列長。添字は0-indexed、区間は半開区間[l,r)。
- S: 集約情報の型。get / prod / all_prodはSを返します。
- F: 更新の型。composition(f,g)はgの後にfを作用させます。
- 各演算・写像・述語の評価をO(1)とした計算量です。

## 型を選ぶ

| 型 | 更新 | 取得する情報 | S | F | 専用API |
| --- | --- | --- | --- | --- | --- |
| `LazyAddMin` | 加算 | 最小 | `long long` | `long long（加算値）` | `add / min` |
| `LazyAddMax` | 加算 | 最大 | `long long` | `long long（加算値）` | `add / max` |
| `LazyAddSum` | 加算 | 和 | `{val,size}` | `long long（加算値）` | `add / sum` |
| `LazyAssignMin` | 代入 | 最小 | `long long` | `LazyAssignF` | `assign / min` |
| `LazyAssignMax` | 代入 | 最大 | `long long` | `LazyAssignF` | `assign / max` |
| `LazyAssignSum` | 代入 | 和 | `{val,size}` | `LazyAssignF` | `assign / sum` |
| `LazyAddMinMax` | 加算 | 最大・最小 | `{mx,mn}` | `long long（加算値）` | `add / min / max` |
| `LazyAssignMinMax` | 代入 | 最大・最小 | `{mx,mn}` | `LazyAssignF` | `assign / min / max` |
| `LazyAddArgMin` | 加算 | 最小値・最左位置 | `{val,id}` | `long long（加算値）` | `add / prod` |
| `LazyAddArgMax` | 加算 | 最大値・最左位置 | `{val,id}` | `long long（加算値）` | `add / prod` |
| `LazyChminMax` | chmin | 最大 | `long long` | `long long（上限）` | `chmin / max` |
| `LazyChmaxMin` | chmax | 最小 | `long long` | `long long（下限）` | `chmax / min` |
| `LazyAffineStats` | 整数一次変換 | 和・最大・最小 | `{sum,mx,mn,size}` | `{a,b}` | `affine / add / mul / assign / sum / min / max` |
| `LazyAffineSum` | 法998244353の一次変換 | modでの和 | `{val,size}` | `{a,b}` | `affine / add / mul / assign / sum` |
| `LazyAffineSumT<Mint>` | 指定modintの一次変換 | modでの和 | `{val,size}` | `{a,b}` | `affine / add / mul / assign / sum` |
| `LazyFlipSum` | 0/1反転 | 1の個数 | `{one,size}` | `bool` | `flip / ones` |
| `LazyFlipInv` | 0/1反転 | 転倒数 | `{zero,one,inv}` | `bool` | `flip / inversions` |
| `LazyAPAddSum` | 等差数列・一次式の加算 | 和 | `{sum,size,idxsum}` | `{a,b}` | `add_linear / add_ap / sum` |

表のTypeは、具体型の名前に置き換えて使います。Sがstructの型では、`get(i)`は整数そのものではなく、その葉の情報です。

## 依存関係・数値範囲

ソースの標準ヘッダとACL（atcoder/lazysegtree・atcoder/modint）が必要です。
個人テンプレートのll・mint・rep・pb・using namespaceには依存しません。

**制約**

- 0<=i<N、0<=l<=r<=N、0<=N<=2^29。無効な添字・区間はstd::out_of_range、負の長さはstd::invalid_argument、長さの上限超過はstd::length_errorです。
- 整数版では要素・区間和・中間積・遅延タグの合成がlong longに収まること。算術オーバーフローを一律に検出する実装ではありません。
- 最小値・最大値・位置付き情報を扱う型の実要素は、`-LSEG_INF < A[i] < LSEG_INF`。`LSEG_INF=2^62`は単位元に使用します。
- 生のSをsetする場合は、葉のsize=1、Arg型のid=iなど、その型のメタデータも正しく設定してください。

LazyAffineSumは法998244353を明示的に使います。別の法には `LazyAffineSumT<atcoder::modint1000000007>` などを使います。
動的modintを使う場合、木の使用中は法を変えないでください。

## コンストラクタ・コピー・ムーブ

```cpp
Type seg;
Type seg(n);
Type seg(values);
Type copied(other);
Type moved(std::move(other));
Type& operator=(const Type& other);
Type& operator=(Type&& other);
```

既定構築は長さ0。具体型のnによる構築は、長さnの実要素をすべて0で初期化します。
modint型ではvector<Mint>などからも構築できます。
コピーは独立した木となり、ムーブ元は長さ0として再利用できます。

汎用の `LazySegBase<S,op,e,F,mp,cp,id>` も使用できます。この汎用基底の(n)はACLと同じく単位元をn個並べるため、具体型の(n)とは区別してください。
一部の型では生のvector<S>からの構築も継承しています。通常は値のvectorか具体型のnによる構築を使うと、葉のメタデータを自動設定できます。

**計算量**

- 既定構築O(1)、配列・nからの構築O(N)。
- コピー構築O(N_other)、コピー代入O(N+N_other)。
- ムーブ構築O(1)、ムーブ代入O(N+1)。破棄はO(N)。

## 共通API

```cpp
int size() const;
bool empty() const;
void set(int i, S value);
S get(int i);
S prod(int l, int r);
S all_prod() const;
void apply(int i, F f);
void apply(int l, int r, F f);
```

sizeは配列長、emptyは長さ0かを返します。setは点を置換し、applyは更新写像を点または区間に作用させます。
get / prod / all_prodは、それぞれ点・区間・全体の集約情報を返します。
具体型には値だけを渡すsetもあり、葉のメタデータを自動設定します。

**計算量**

- size / empty / all_prodはO(1)。それ以外はO(log(N+1))。
- prod(l,l)は単位元、apply(l,l,f)は何もしないためO(1)。

## max_right / min_left

```cpp
template<class G> int max_right(int l, G g);
template<class G> int min_left(int r, G g);
template<bool (*g)(S)> int max_right(int l);
template<bool (*g)(S)> int min_left(int r);
```

max_rightはg(prod(l,r))が成立する最大のr、min_leftはg(prod(l,r))が成立する最小のlを返します。
関数オブジェクト・ラムダの2引数版と、関数ポインタをテンプレート引数にする版があります。

**制約**

- gは副作用がなく、探索方向への区間拡張に対して判定が単調であること。
- g(単位元)==true。falseはNDEBUGでもstd::invalid_argumentになります。
- 和を用いた上限判定では、負の要素を許すと単調性が崩れることがあります。

**計算量**

- O(log(N+1))。

## 加算・代入・chmin・chmax

```cpp
void add(int l, int r, long long x);
void assign(int l, int r, long long x);
void apply(int i, long long x);       // Assign型の値の代入
void apply(int l, int r, long long x);// Assign型の値の代入
void chmin(int l, int r, long long x);// LazyChminMax
void chmax(int l, int r, long long x);// LazyChmaxMin
```

対応する型で利用します。Assign型では `LazyAssignF{x,true}` を生のFとして渡しても同じ更新です。
`LazyAssignF{}` は何も変更しない写像です。
chminはA[i]=min(A[i],x)、chmaxはA[i]=max(A[i],x)です。
この2型は最大値または最小値だけを集約する通常の遅延セグ木で、区間和を持つSegment Tree Beatsとは別です。

**計算量**

- O(log(N+1))。

## 一次変換

```cpp
void affine(int l, int r, Number a, Number b);
void add(int l, int r, Number x);
void mul(int l, int r, Number x);
void assign(int l, int r, Number x);
```

LazyAffineStatsではNumber=long long、LazyAffineSum / LazyAffineSumTではNumber=選択したmodint型です。
affineはA[i]=a*A[i]+b。add、mul、assignはそれぞれ(1,x)、(x,0)、(0,x)の一次変換です。
整数Stats型はa<0で最大・最小を入れ替えます。

**計算量**

- O(log(N+1))。

## 0/1反転・等差数列加算

```cpp
void flip(int l, int r);                     // LazyFlipSum / LazyFlipInv
void add_linear(int l, int r, long long a, long long b); // LazyAPAddSum
void add_ap(int l, int r, long long a, long long d);     // LazyAPAddSum
```

二進列の初期値・setの入力は、0なら0、非0なら1に正規化します。flipは0/1を反転します。
add_linearは配列全体の添字iでA[i]+=a*i+b、add_apは区間左端からの添字jでA[l+j]+=a+d*jです。

**計算量**

- O(log(N+1))。

## 集約情報の取得・変換ヘルパー

```cpp
long long min(int l, int r);       // Min / MinMax / Stats型。Arg型はprod().valを使う
long long max(int l, int r);       // Max / MinMax / Stats型。Arg型はprod().valを使う
Number sum(int l, int r);          // AddSum / AssignSum / Affine系 / AP型
long long ones(int l, int r);      // LazyFlipSum
long long inversions(int l, int r);// LazyFlipInv
```

各型で対応する情報を取り出します。Arg型のprodは{val,id}を返し、同じ値なら最左の添字です。
inversionsはi<jかつ(A[i],A[j])=(1,0)となる組数を返します。

LazyAddArgMin / LazyAddArgMax / LazyFlipSum / LazyFlipInv / LazyAPAddSumには、値のvectorを葉のvector<S>へ変換する公開static関数cv(v)もあります。

**計算量**

- 集約取得はO(log(N+1))。cvはO(N)時間・返却メモリ。

## 空区間の返り値

| 情報 | 空区間 |
| --- | --- |
| 最小値・最大値 | LSEG_INF / -LSEG_INF |
| 和・1の個数・転倒数・size | 0 |
| Arg型のid | LSEG_INF |

単位元に更新を作用させても空情報を保つようにしています。長さ0の木やムーブ元にもprod(0,0)、all_prod、空区間更新・境界探索を使用できます。

## 使用例

ソースをグローバルに置き、cassertをincludeしてmain内で実行します。

```cpp
LazyAddSum add(std::vector<long long>{1,2,3});
add.add(0,2,5);
assert(add.sum(0,3)==16 && add.get(1).val==7);
assert(add.sum(2,2)==0);

LazyAssignMinMax assign(3);
assign.assign(0,3,10); assign.set(1,-2);
assert(assign.min(0,3)==-2 && assign.max(0,3)==10);

LazyAffineStats stats(std::vector<long long>{2,5,3});
stats.affine(0,3,-2,1);
assert(stats.sum(0,3)==-17 && stats.min(0,3)==-9 && stats.max(0,3)==-3);

LazyAffineSumT<atcoder::modint1000000007> mod(std::vector<long long>{1,2,3});
mod.affine(0,3,2,3);
assert(mod.sum(0,3).val()==21);

LazyFlipInv flip(std::vector<long long>{1,0,1});
assert(flip.inversions(0,3)==1);
flip.flip(0,3); assert(flip.inversions(0,3)==1);

LazyAPAddSum ap(4); ap.add_ap(1,4,10,3);
assert(ap.sum(0,4)==39 && ap.get(3).sum==16);
```

## 旧版からの移行

| 旧版 | 改訂版 |
| --- | --- |
| 末尾説明文を手動でコメント化 | ファイル全体をそのまま使用可能 |
| 個人テンプレートのll / mint / rep / pbに依存 | 必要な標準ヘッダと型の明示で依存を解消 |
| 型によって(n)の初期化・対応が不統一 | 17種類すべてに既定・n・値のvectorによる構築。具体型の(n)は0の実要素 |
| global mintに依存したLazyAffineSum | 既定を998244353へ明示。別の法にはLazyAffineSumT<Mint> |
| ACL基底へのキャスト等 | 検査付きAPIを使用。ACL実装への継承は非公開 |
| assertによる添字・探索開始位置の検査 | NDEBUGでも例外で検出 |

## 確認結果

GNU C++23 / GCC 13.3.0、固定版ACLで確認しました。
全17種類の構築・空状態・コピー・ムーブ・例外、単位元の保持、境界探索を確認しています。
32シード×1024ステップの4グループで、加算・代入/chmin/chmax・一次変換/等差数列・反転を通常配列と比較しました。
Beats拡張を含む専用テスト全体では163,840グループステップです。NDEBUG、AddressSanitizer、UBSan、デバッグイテレータでも成功しています。実行環境の制限でLeakSanitizerは無効です。

[専用テスト](../../../tests/segment_wrappers_test.cpp) · [C++23の確認結果](../validation.md)

```sh
g++ -std=c++23 -O2 -I/path/to/ac-library tests/segment_wrappers_test.cpp -o /tmp/segment_wrappers_test
/tmp/segment_wrappers_test
```

メモリO(N)。テストは全入力やオーバーフロー条件を網羅するものではありません。
