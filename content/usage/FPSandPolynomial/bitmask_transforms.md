# 部分集合変換・OR／AND畳み込み

[説明書の目次](../index.md) · [コード](../../../FPSandPolynomial/bitmask_transforms.cpp)

ビット集合の包含関係を使い、すべての部分集合・上位集合について値を集計します。
持っている能力集合に含まれる条件の個数、ある条件をすべて満たす集合の個数、2つの集合の和集合・共通部分ごとの組数などに使えます。

既存のXOR畳み込みは排他的論理和、約数・倍数変換は整数の整除関係を扱います。この実装はビット集合の包含関係とOR・ANDを扱う別の機能です。
型エイリアス・貼り付け方は [共通設定](../common.md) を参照してください。標準ヘッダーだけを使い、基本テンプレート以外のincludeやACLへの依存はありません。

## 型・前提

```cpp
class BitmaskTransform;
```

- 配列長をNとします。非空なら $N=2^K$ となる非負整数Kが必要です。N=1は空集合だけを表すK=0の場合です。
- 添字は0-indexedです。添字Sの下からi bit目が1なら、集合Sが要素iを含むと考えます。
- $T\subseteq S$ は `(T & S) == T`、$T\supseteq S$ は `(T & S) == S` を意味します。包含には同じ集合と空集合を含みます。
- 値の型Tはコピーと `+=`・`-=` に対応し、加法が可換群をなすことが必要です。各演算をO(1)と数えます。
- 畳み込みではさらに `*=` が必要で、乗法が加法に対する分配法則を満たすことが必要です。通常の整数・modintを想定しています。
- `ll`・`LL`・`mint`、ACLのdynamic_modintなどで使えます。`vector<bool>` は受け付けません。
- 通常の整数型では途中の和・差・積も含めて型の範囲内に収めてください。浮動小数点では丸め誤差が生じます。
- 除算・逆元を使わないため、法が素数である必要はありません。偶数・合成数の法でも変換と畳み込みを使えます。dynamic_modintは操作中に法を変えないでください。

## 構築方法

実装をグローバルに置き、`BitmaskTransform::subset_zeta(a)` のように静的関数を呼びます。インスタンスは構築できず、初期化・build操作は不要です。

変換の4関数は参照渡しで入力列をその場で更新します。畳み込みの2関数は値渡しで受け取り、新しいvectorを返します。
どの関数も配列長を変更しません。

## BitmaskTransform::subset_zeta

```cpp
template<class T>
static void subset_zeta(vector<T>& a);
```

引数aを、元の値をfとして次の列へ置き換えます。返り値はありません。

$$
a[S]=\sum_{U\subseteq S}f[U].
$$

例えば `{1,2,3,4}` は `{1,3,4,10}` になります。添字3は2 bitとも1なので、すべての元の要素を足します。

**計算量**：$O(1+N\log(N+1))$ 時間、入力列を除く追加領域O(1)。空なら何もせず、N=1なら値を変更しません。

## BitmaskTransform::subset_mobius

```cpp
template<class T>
static void subset_mobius(vector<T>& a);
```

部分集合ゼータ変換の逆変換をaへ適用します。返り値はありません。
元の値をgとすると、更新後は次の値です。任意の列に適用でき、ゼータ変換済みかどうかの状態は保持しません。

$$
a[S]=\sum_{U\subseteq S}(-1)^{|S|-|U|}g[U].
$$

同じ列へsubset_zeta、subset_mobiusの順で適用すると元へ戻ります。逆の順でも同じです。空・N=1は変更しません。

**計算量**：$O(1+N\log(N+1))$ 時間、追加領域O(1)。

## BitmaskTransform::superset_zeta

```cpp
template<class T>
static void superset_zeta(vector<T>& a);
```

引数aを、元の値をfとして次の列へ置き換えます。返り値はありません。

$$
a[S]=\sum_{U\supseteq S}f[U].
$$

例えば `{1,2,3,4}` は `{10,6,7,4}` になります。添字0の空集合はすべての集合に含まれるので、全要素の和になります。

**計算量**：$O(1+N\log(N+1))$ 時間、追加領域O(1)。空・N=1は変更しません。

## BitmaskTransform::superset_mobius

```cpp
template<class T>
static void superset_mobius(vector<T>& a);
```

上位集合ゼータ変換の逆変換をaへ適用します。返り値はありません。
元の値をgとすると、更新後は次の値です。

$$
a[S]=\sum_{U\supseteq S}(-1)^{|U|-|S|}g[U].
$$

superset_zetaとの合成はどちらの順でも恒等変換になります。任意の列へ適用でき、空・N=1は変更しません。

**計算量**：$O(1+N\log(N+1))$ 時間、追加領域O(1)。

## BitmaskTransform::or_convolution

```cpp
template<class T>
static vector<T> or_convolution(vector<T> a, vector<T> b);
```

aとbのOR畳み込みを、同じ長さのvector<T>で返します。

$$
c[S]=\sum_{i\mathbin{|}j=S}a[i]b[j].
$$

集合の和集合がSになるすべての順序付きの組を数えます。iとjが共通の要素を持っていても構いません。
互いに素な集合の分割を扱うsubset convolutionではありません。

2列は**同じ長さ**でなければなりません。両方空なら空を返します。非空なら長さは2冪が必要です。既存のFWHT::convolutionと異なり、自動の0埋めは行いません。
短い列を使う場合は、呼び出し側で両列を同じ2冪長へゼロで拡張してください。

入力を値渡しするため、元の列を変更しません。`std::move(a)` で渡した場合、その入力列を以後使用しないでください。

**計算量**：$O(1+N\log(N+1))$ 時間、入力の値渡しコピー・作業領域・返り値を含む領域O(N+1)。

## BitmaskTransform::and_convolution

```cpp
template<class T>
static vector<T> and_convolution(vector<T> a, vector<T> b);
```

aとbのAND畳み込みを、同じ長さのvector<T>で返します。

$$
c[S]=\sum_{i\mathbin{\&}j=S}a[i]b[j].
$$

集合の共通部分がSになるすべての順序付きの組を集計します。
例えば、S=0は共通要素を持たない組です。2列の長さ、空入力、自動拡張、入力を変更しないことはor_convolutionと同じです。

**計算量**：$O(1+N\log(N+1))$ 時間、入力コピー・作業領域・返り値を含む領域O(N+1)。

## 使用例

実装をグローバルに置き、以下をmain内などで実行します。

```cpp
vl a={1,2,3,4};
BitmaskTransform::subset_zeta(a);
assert((a==vl{1,3,4,10}));
BitmaskTransform::subset_mobius(a);
assert((a==vl{1,2,3,4}));
BitmaskTransform::superset_zeta(a);
assert((a==vl{10,6,7,4}));
BitmaskTransform::superset_mobius(a);
assert((a==vl{1,2,3,4}));

vl x={1,2},y={3,4};
auto c=BitmaskTransform::or_convolution(x,y);
auto d=BitmaskTransform::and_convolution(x,y);
assert((c==vl{3,18}));
assert((d==vl{13,8}));
assert((x==vl{1,2}) && (y==vl{3,4}));

auto e=BitmaskTransform::or_convolution(vector<mint>{1,2},vector<mint>{3,4});
assert(e[0]==3 && e[1]==18);
vl empty;
BitmaskTransform::subset_zeta(empty);
assert(BitmaskTransform::and_convolution(empty,empty).empty());
assert((BitmaskTransform::or_convolution(vl{5},vl{7})==vl{35}));
```

各集合の出現回数をa[S]へ入れ、subset_zetaを適用すると「Sに含まれる集合の総数」、superset_zetaなら「Sを含む集合の総数」をまとめて得られます。
同じ頻度列を2回渡した畳み込みは、同じ要素を選ぶ場合も含む順序付きの組を数えます。異なる要素を選ぶ制約や順序を区別しない条件は、呼び出し側で補正してください。

## 原理

部分集合ゼータ変換では、各bitについて「そのbitを含まない集合から、含む集合へ」値を足します。
処理済みのbitだけは自由に落とした部分集合の和が得られ、全bitを処理するとすべての部分集合の和になります。
上位集合では足す向きを逆にします。各段階の加算を減算へ変えたものがメビウス変換です。異なるbitの段階は可換なので、逆変換でも同じbit順を使えます。

OR畳み込みの部分集合ゼータ変換を考えると、$i\mathbin{|}j\subseteq S$ は $i\subseteq S$ かつ $j\subseteq S$ と同値です。
そのため変換後の各成分は、aとbそれぞれの変換値の積になります。ゼータ変換→成分ごとの積→メビウス変換で畳み込みを計算できます。
ANDの場合も、$i\mathbin{\&}j\supseteq S$ と「iとjがともにSを含む」が同値なので、上位集合変換を使って同様に求めます。

## 入力検査・確認結果

長さ不一致や非空の非2冪長はメッセージなしで停止します。NDEBUGでも検査は有効です。型の演算則や整数のオーバーフローを実行時に検査するものではありません。

`tests/bitmask_transforms_test.cpp` で、直接の部分集合列挙と二重ループの畳み込みに照合します。
小配列の全列挙、逆変換の両方向、負数、空・長さ1、法2・法6・998244353、同じ入力列、2^20要素のケース、無効な長さの停止を検証します。
C++23、NDEBUG、AddressSanitizer／UBSanで確認します。

## 関連問題

[Library Checker — Bitwise And Convolution](https://judge.yosupo.jp/problem/bitwise_and_convolution) のAND畳み込みに対応します。オンライン提出を行ったことを示すものではありません。
