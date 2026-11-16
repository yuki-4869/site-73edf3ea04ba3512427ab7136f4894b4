# Slope Trick（凸区分線形関数）

[説明書の目次](../index.md) · [コード](../../../DiscreteConvexity/slope_trick.cpp)

絶対値や片側だけの線形コストを加えながら、凸区分線形関数の最小値と最小化位置を管理します。
位置や高さを状態に持つDPで、`|x-a|` の追加、前の位置以下／以上からの遷移、一定距離以内からの遷移を高速に処理できます。
CHTが複数の直線の最小値を扱うのに対し、本ライブラリは**1つの凸関数そのものを更新**します。

型エイリアスと貼り付け方は [共通設定](../common.md) を参照してください。
基本テンプレートの `LL = __int128` を使い、追加のinclude、ACL、他の個別ライブラリへの依存はありません。

## 型・前提

```cpp
class SlopeTrick;
```

- 実数全体で有限かつ下に有界な凸区分線形関数を扱います。折れ点の座標・傾きは整数です。
- 初期関数は定数です。以後、以下のAPIで関数を構築・変形します。任意の関数を配列から取り込むAPIはありません。
- 引数の座標・移動量・定数と、返す関数値は符号付き128bit整数 `LL` です。`ll` や `int` はそのまま渡せます。
- 全中間演算を符号付き128bitの範囲内に収めてください。関数値だけでなく、遅延オフセット、保存する座標、座標差、加算途中の値も含みます。
- 基本テンプレートにはLLを直接出力する演算子がありません。結果がllに収まると確認できるときは `(ll)st.min_value()` などに変換できます。
- 状態を添字で参照するAPIはありません。`size()` の折れ点数には重複を含み、異なる座標の個数ではありません。
- 重み付きコストの一括追加、追加済みコストの任意削除、最適な遷移経路の復元には対応しません。整数重みwのコストはw回の追加で表現でき、その分だけ時間・領域を使います。

以下、Kを保持中の折れ点数、N・Mを結合前の2オブジェクトの折れ点数とします。
対数の現れる計算量では、折れ点数が0または1の部分をO(1)と読みます。
ヒープをvectorで保持するため、コスト追加と結合の計算量は再確保を含めた償却計算量です。

## 構築方法

```cpp
explicit SlopeTrick(LL c = 0);
```

全てのxで `f(x)=c` となる関数を作ります。省略時は0です。
折れ点数は0で、最小値はc、最小化位置は実数全体です。buildは不要です。

**計算量**：O(1)時間・領域。

通常のコピー構築はO(K)時間・領域で、以後は独立して更新できます。
コピー代入では元の保持領域を解放する処理も発生します。

## SlopeTrick::size

```cpp
size_t size() const;
```

内部で保持する折れ点数Kを返します。同じ位置の折れ点も別々に数えます。
`add_a_minus_x`・`add_x_minus_a` では1、`add_abs` では2増えます。
`prefix_min`・`suffix_min` では片側の折れ点が削除されるため減ることがあります。
`shift` と `add_const` は個数を変えません。

**計算量**：O(1)。

## SlopeTrick::min_value

```cpp
LL min_value() const;
```

現在の関数の全体の最小値 $\min_x f(x)$ を返します。
初期状態や `clear` 直後にも呼べます。

**計算量**：O(1)。

## SlopeTrick::argmin

```cpp
pair<optional<LL>, optional<LL>> argmin() const;
```

最小値を取る位置全体を、両端を含む区間 `[l,r]` として返します。
第1要素が `nullopt` なら下端は $-\infty$、第2要素が `nullopt` なら上端は $+\infty$ です。
両方が `nullopt` なら定数関数で、全ての位置が最適です。区間が空になることはありません。

有限端点は必ず整数ですが、その間の実数も全て最適です。整数状態のDPでも同じ端点を使えます。
`optional` の値は、存在を確認してから `*l` などで取得してください。

**計算量**：O(1)。

## SlopeTrick::add_const

```cpp
void add_const(LL c);
```

$f(x) \leftarrow f(x)+c$ とします。負のcも指定できます。
最小値はc増え、最小化位置は変わりません。返り値はありません。

**計算量**：O(1)。

## SlopeTrick::add_a_minus_x

```cpp
void add_a_minus_x(LL a);
```

$f(x) \leftarrow f(x)+\max(a-x,0)$ とします。
位置xがaより小さいときに、距離a-xをコストとして加えます。返り値はありません。

**計算量**：償却O(log K)時間。折れ点を1個追加します。

## SlopeTrick::add_x_minus_a

```cpp
void add_x_minus_a(LL a);
```

$f(x) \leftarrow f(x)+\max(x-a,0)$ とします。
位置xがaより大きいときに、距離x-aをコストとして加えます。返り値はありません。

**計算量**：償却O(log K)時間。折れ点を1個追加します。

## SlopeTrick::add_abs

```cpp
void add_abs(LL a);
```

$f(x) \leftarrow f(x)+|x-a|$ とします。
`add_a_minus_x(a)` と `add_x_minus_a(a)` を1回ずつ行う操作です。返り値はありません。

**計算量**：償却O(log K)時間。折れ点を2個追加します。

## SlopeTrick::prefix_min

```cpp
void prefix_min();
```

更新前の関数をgとして、$f(x) \leftarrow \min_{y\le x} g(y)$ とします。
「前の状態y以下には戻れず、新しい状態xはy以上」という単調な遷移に使えます。
右側の折れ点を全て破棄します。全体の最小値は変わりません。返り値はありません。

例えば `|x-3|` は、x<3で3-x、x>=3で0となる関数に変わります。

**計算量**：O(K)以下。破棄した領域を解放します。

## SlopeTrick::suffix_min

```cpp
void suffix_min();
```

更新前の関数をgとして、$f(x) \leftarrow \min_{y\ge x} g(y)$ とします。
新しい状態xが前の状態y以下である遷移に使えます。
左側の折れ点を全て破棄します。全体の最小値は変わりません。返り値はありません。

例えば `|x-3|` は、x<=3で0、x>3でx-3となる関数に変わります。

**計算量**：O(K)以下。破棄した領域を解放します。

## SlopeTrick::shift

```cpp
void shift(LL a);
void shift(LL a, LL b);
```

更新前の関数をgとします。

- 1引数では $f(x) \leftarrow g(x-a)$ とします。グラフを右へaだけ平行移動します。負なら左です。
- 2引数では $f(x) \leftarrow \min_{x-b\le y\le x-a} g(y)$ とします。前の位置yからの移動量 `x-y` が閉区間 `[a,b]` にある遷移です。

2引数版には `a <= b` が必要です。違反時はNDEBUGでもメッセージなしで停止します。
`shift(a,a)` は1引数版と同じです。`shift(-d,d)` は距離d以内からの遷移で、d>=0とします。

最小化区間の有限な下端はa、有限な上端はbだけ動き、無限の端点はそのままです。
全体の最小値は変わりません。返り値はありません。

**計算量**：どちらもO(1)。

## SlopeTrick::eval

```cpp
LL eval(LL x) const;
```

指定した整数位置xでの関数値f(x)を返します。関数や折れ点を変更しません。
毎回全ての折れ点を走査するため、頻繁な点照会には向きません。
全体の最小値だけが必要なら `min_value()` を使ってください。

**計算量**：O(K)時間・追加O(1)領域。K=0ならO(1)。

## SlopeTrick::clear

```cpp
void clear();
```

折れ点と保持領域を全て削除し、`f(x)=0` に戻します。
構築時に指定したcも残りません。返り値はありません。

**計算量**：O(K)以下。

## SlopeTrick::merge

```cpp
void merge(SlopeTrick& other);
```

更新前の関数をそれぞれf・gとして、自身を `f(x)+g(x)` に置き換えます。
otherは消費され、**`g(x)=0` の初期状態に戻ります**。定数項や平行移動も含めて加算します。
返り値はありません。自己結合 `st.merge(st)` では `f(x) *= 2` となります。

結合後の折れ点数はN+Mです。内部では折れ点数の大きい側を受け手にして、小さい側の折れ点を再追加します。
otherを残したいときは、先にコピーしてからそのコピーを渡してください。

**計算量**：償却 $O((\min(N,M)+1)\log(N+M))$ 時間。N+M<=1ならO(1)。
結合中も全体でO(N+M)領域です。自己結合では内部でコピーを作ります。

## 使用例

実装をグローバルに置き、以下をmain内などで実行します。

```cpp
SlopeTrick st;
st.add_abs(1);st.add_abs(5);         // |x-1|+|x-5|
assert(st.min_value()==4);
assert(st.eval(0)==6);
auto [l,r]=st.argmin();
assert(l && r && *l==1 && *r==5);   // 最小化位置は閉区間[1,5]

st.shift(-2,3);                    // -2以上3以下だけ移動してよい
auto range=st.argmin();
assert(range.first && range.second && *range.first==-1 && *range.second==8);
st.add_abs(10);
assert(st.min_value()==6);

SlopeTrick cost(7);cost.add_abs(0);
LL expected=st.eval(3)+cost.eval(3);
st.merge(cost);
assert(st.eval(3)==expected);
assert(cost.size()==0 && cost.min_value()==0);
assert(!cost.argmin().first && !cost.argmin().second);

st.clear();
st.add_a_minus_x(4);               // max(4-x,0)
auto halfline=st.argmin();
assert(halfline.first && *halfline.first==4 && !halfline.second);
st.suffix_min();                  // 全ての位置で0
assert(st.size()==0 && st.eval(-100)==0);
```

## DPへの使い方

例えば、整数列Aに対して、非減少の整数列Bを選び $\sum_i |B_i-A_i|$ を最小化します。
末尾の値をxに固定した最小コストを $f_i(x)$ とすると、初期値 $f_0(x)=0$ に対して

$$
f_i(x)=|x-A_i|+\min_{y\le x}f_{i-1}(y)
$$

です。順に `prefix_min()`、`add_abs(A_i)` を行い、最後に `min_value()` を取得します。

```cpp
vl a={3,1,2};
SlopeTrick dp;
for(ll x:a){dp.prefix_min();dp.add_abs(x);}
assert(dp.min_value()==2);          // 例えばB={2,2,2}
```

折れ点追加の合計は2N個です。破棄される各折れ点は1回だけ処理されるため、全体でO(N log N)時間・O(N)領域です。
`suffix_min()` に置き換えると非増加制約になります。
代わりに `shift(-d,d)` を使うと $|B_i-B_{i-1}|\le d$ という移動制約になります。
これらの例では初期位置を自由に選べます。特定位置から開始する制約や有限区間の外を無限大とする制約は、自動では設定されません。

## アルゴリズム

左右2つの折れ点集合L・Rと最小値mを用いて、関数を

$$
f(x)=m+\sum_{l\in L}\max(l-x,0)+\sum_{r\in R}\max(x-r,0)
$$

と表します。全ての左折れ点が全ての右折れ点以下になるように保ちます。
このとき、最小化位置は $[\max L,\min R]$ で、空集合側の端点は無限です。

片側コストの追加で順序が逆転するときは、逆転する折れ点1個を反対側に移し、交差分をmに加えます。
左右の端点はそれぞれ最大・最小ヒープで取得し、移動量を遅延保持するため、区間内の最小化をO(1)で処理できます。
`prefix_min` はR、`suffix_min` はLを削除する操作です。

## 確認結果

専用テスト `tests/slope_trick_test.cpp` で、小さな操作列の全列挙と独立した関数値表の更新を照合します。
片側コスト・絶対値・定数・平行移動・非対称な区間遷移・前後の最小化を組み合わせ、関数値・最小値・有限／無限の最小化区間を確認します。
非減少列のDPと移動制約付きDPは位置の全列挙とも比較します。
結合、自己結合、コピー、空・定数関数、llの両極値を超える座標とコスト、20万個の絶対値追加も検証します。
C++23の通常・NDEBUG・AddressSanitizer／UBSanで実行します。

## 参考

- [AtCoder ABC217 H — Snuketoon](https://atcoder.jp/contests/abc217/tasks/abc217_h)
- [同問題の公式解説](https://atcoder.jp/contests/abc217/editorial/2620)：凸関数を管理するDPの応用例です。本ページの確認結果はオンライン提出の実施を示すものではありません。
