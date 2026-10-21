# Disjoint Sparse Table

[説明書の目次](../index.md) · [コード](../../../DataStructureSegment/disjoint_sparse_table.cpp)

更新のない列に対し、結合的な演算の区間積をO(1)で求めます。区間最小値だけでなく、関数合成・行列積などの非可換な演算にも使えます。
通常のSparse Tableによる重なる2区間の取得と異なり、冪等性は不要です。逆元も使いません。

型エイリアスとコードの置き方は [共通設定](../common.md) を参照してください。
基本テンプレートの標準ヘッダーだけで使用でき、ACLへの依存はありません。

## 型・テンプレート引数

```cpp
template<class S, S(*op)(S,S), S(*e)()>
class DisjointSparseTable;
```

`S` は要素と集約値の型、`op(a,b)` はaの右にbを連結する演算、`e()` は単位元です。
例えば区間和では `op(a,b)=a+b`、`e()=0` とします。

**共通の前提**

- `op(op(a,b),c) == op(a,op(b,c))` を満たし、`op(e(),a) == op(a,e()) == a` とします。可換性は不要です。
- `S` はコピー構築・コピー代入が可能な型とします。デフォルトコンストラクタは不要です。
- 演算と単位元は呼出し順や外部状態によって結果が変わらないものとします。整数のオーバーフローを避け、浮動小数点の丸めによる結合則の崩れにも注意してください。
- 添字は0-indexed、区間は半開区間 `[l,r)` です。列長Nは `0 <= N <= INT_MAX` とし、必要なメモリを確保できることを前提とします。
- 以下の計算量は `S` のコピー・`op`・`e` がO(1)の場合です。例えば文字列連結では、実際の文字列長に依存する追加コストがかかります。

## コンストラクタ

```cpp
DisjointSparseTable<S,op,e> st;
DisjointSparseTable<S,op,e> st(const vector<S>& v);
```

引数なしでは空列を構築します。vを渡すと、その内容をコピーして区間積を前計算します。
構築後に元のvを書き換えても、stの内容は変わりません。

**計算量**

- 空列の構築は時間・領域ともO(1)。
- N要素からの構築は時間・領域ともO(N log(N+1))。N=0の場合はO(1)。

## build

```cpp
void st.build(const vector<S>& v);
```

以前の列を捨て、vの内容で再構築します。長さの変更や空列への変更も可能です。
点更新APIはなく、1要素の変更にも列全体の再構築が必要です。

**計算量**

- 新しい列長をNとしてO(N log(N+1))に、以前の表を解放・上書きするコストが加わります。
- 保持する表の要素数はO(N log(N+1))。N=0では保持要素数は0です。再構築時には `vector` の確保済み容量を再利用する場合があり、確保領域は過去の列長にも依存します。

## size

```cpp
int st.size() const;
```

要素数Nを返します。O(1)です。

## get

```cpp
S st.get(int i) const;
```

位置iの要素を値として返します。`0 <= i < N` が必要です。O(1)です。
返された値を書き換えても内部の列は変わりません。

## prod

```cpp
S st.prod(int l, int r) const;
```

`v[l], v[l+1], ..., v[r-1]` をこの順に `op` で集約した値を返します。
`0 <= l <= r <= N` が必要です。
空区間では `e()`、1要素の区間ではその要素を返します。

**計算量**

- O(1)。2要素以上の区間では `op` を1回だけ呼びます。

## all_prod

```cpp
S st.all_prod() const;
```

`st.prod(0,st.size())` を返します。空列では `e()` を返します。O(1)です。

## 使用例

実装をグローバルに置き、以下をmain内などで実行します。`op` と `e` には、通常の関数またはキャプチャなしのconstexprラムダを渡せます。

```cpp
constexpr auto sum_op=[](ll a,ll b){return a+b;};
constexpr auto sum_e=[](){return 0LL;};
DisjointSparseTable<ll,sum_op,sum_e> st(vl{3,1,4,1,5});
assert(st.size()==5);
assert(st.get(2)==4);
assert(st.prod(1,4)==6);
assert(st.prod(3,3)==0);
assert(st.all_prod()==14);
st.build(vl{2,7,1});
assert(st.all_prod()==10);

using F=pair<mint,mint>;
constexpr auto compose=[](F f,F g)->F{
    return {g.first*f.first,g.first*f.second+g.second};
};
constexpr auto identity=[]()->F{return {1,0};};
DisjointSparseTable<F,compose,identity> functions(vector<F>{{2,1},{3,4},{5,6}});
auto [a,b]=functions.prod(0,3);
assert(a*mint(7)+b==mint(251));
auto [c,d]=functions.prod(1,1);
assert(c==mint(1) && d==mint(0));

DisjointSparseTable<ll,sum_op,sum_e> empty;
assert(empty.size()==0 && empty.all_prod()==0);
```

関数の例では `f0(x)=2x+1`、`f1(x)=3x+4`、`f2(x)=5x+6` を順番に適用します。
`prod(l,r)` が返すのは $f_{r-1}\circ\cdots\circ f_l$ です。結合の向きを逆にすると別の関数になるため、`op(f,g)` は「fのあとにg」としています。

## 前計算とクエリの仕組み

段kでは、長さ $2^{k+1}$ のブロックごとに中央を境として、左半分のsuffix積と右半分のprefix積を保存します。
左側は右から左へ `op(v[i],suffix)`、右側は左から右へ `op(prefix,v[i])` と計算し、元の並び順を保ちます。
末尾の不完全なブロックも実際の列長で打ち切るため、2の冪へのパディングは不要です。

長さ2以上のクエリでは、lとr-1のXORの最上位ビットをkとします。
両端は段kの同じブロックの左右に分かれるため、左端から中央までのsuffix積と、中央から右端までのprefix積を1回の `op` で連結できます。
2区間は重ならず、積の順序も保たれます。
空区間と1要素の区間は別に処理し、XORが0のときの段番号計算を避けています。

## 使い分けとメモリ

| 状況 | 選び方 |
| --- | --- |
| 点更新があり、区間積を求めたい | [セグメント木](segtree_wrappers.md) |
| 更新がなく、非可換な演算もO(1)で集約したい | このDisjoint Sparse Table |
| 区間和やXORなど、prefixから逆演算で区間を復元できる | 累積和などでO(N)領域に削減可能 |

N>=1で $L=\lceil\log_2 N\rceil$ とすると、元の列とL段の表に合計 $(L+1)N$ 個のSを保持します。
N=200000、Sが8バイトの整数なら要素本体は約29MiBです。Sが大きい場合やメモリ制限が厳しい場合は注意してください。

不正な添字や `INT_MAX` を超える列長を渡した場合は、メッセージなしで停止します。`NDEBUG` の有無によらず検査します。

## 確認結果

C++23で、空列・1要素・2の冪の前後・再構築を確認しています。
小規模の全区間を逐次集約と照合し、区間和・最小値に加えて、関数合成・行列積・文字列連結で順序も検証しています。
テストはサイト用リポジトリの `tests/disjoint_sparse_table_test.cpp` にあり、サイトの更新時にも実行します。

## 参考

- [Disjoint Sparse Table tutorial（Codeforces）](https://codeforces.com/blog/entry/79108)：中央からの積を使う前計算と、XORの最上位ビットによる段の選択。
