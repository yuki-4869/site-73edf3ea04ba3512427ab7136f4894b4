# 複数テストケース用テンプレート

[説明書の目次](../index.md) · [コード](../../../Templates/multiple_test_cases.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Templates/multiple_test_cases.cpp)

競技プログラミングの共通型・入出力・数学補助関数・最短距離をまとめたテンプレートです。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

Nはvector長または二項係数の上側。ll=long long、LL=__int128。

**共通の前提**

- GNU C++とACL modint（通常版はACL全体）が必要。llの中間計算のオーバーフローは自動検知しません。mintはmodint998244353。

<details>
<summary>API一覧</summary>

- [vec2](#api-1)
- [vec3](#api-2)
- [vec4](#api-3)
- [sz](#api-4)
- [operator<<](#api-5)
- [operator>>](#api-6)
- [chmin](#api-7)
- [chmax](#api-8)
- [LB](#api-9)
- [UB](#api-10)
- [vec_min](#api-11)
- [vec_max](#api-12)
- [vec_sum](#api-13)
- [cin_vl](#api-14)
- [cin_vvl](#api-15)
- [cin_xy](#api-16)
- [cin_xyz](#api-17)
- [Cin_vl](#api-18)
- [cin_pairs](#api-19)
- [cin_tuples](#api-20)
- [cout_vl](#api-21)
- [Cout_vl](#api-22)
- [cout_vvl](#api-23)
- [Swap](#api-24)
- [ceil_div](#api-25)
- [floor_div](#api-26)
- [isqrt](#api-27)
- [sort_unique_vec](#api-28)
- [compress](#api-29)
- [prefix_sum](#api-30)
- [prefix_sum2](#api-31)
- [range_sum](#api-32)
- [run_length](#api-33)
- [argmin](#api-34)
- [argmax](#api-35)
- [in_vec](#api-36)
- [even](#api-37)
- [odd](#api-38)
- [grid_id](#api-39)
- [grid_xy](#api-40)
- [gcd](#api-41)
- [lcm](#api-42)
- [Exp](#api-43)
- [powmod](#api-44)
- [square](#api-45)
- [Uni_erase](#api-46)
- [OutIn](#api-47)
- [Random](#api-48)
- [YesNo](#api-49)
- [vl_n](#api-50)
- [vl_N](#api-51)
- [factorial](#api-52)
- [ensure_factorial](#api-53)
- [nCr](#api-54)
- [nPr](#api-55)

- [dijkstra](#dijkstra)
- [Pow2](#pow2-table)

</details>

## この実装の注意点

mainまたはsolveの関数本体が途中で終わる記入用ファイルです。問題の処理と閉じ括弧を追記してください。Graphはvector<vector<pair<ll,ll>>>の重み付き隣接リストです。基本テンプレートと型を統一しています。

<a id="api-1"></a>

## vec2

```cpp
auto vec2(ll n,ll m,T x=T());
```

n×mの二次元vectorをxで初期化して返します。T省略時ll。

**計算量**

- O(nm)。

<a id="api-2"></a>

## vec3

```cpp
auto vec3(ll n,ll m,ll l,T x=T());
```

n×m×lの三次元vectorをxで初期化して返します。

**計算量**

- O(nml)。

<a id="api-3"></a>

## vec4

```cpp
auto vec4(ll a,ll b,ll c,ll d,T x=T());
```

a×b×c×dの四次元vectorをxで初期化して返します。

**計算量**

- O(abcd)。

<a id="api-4"></a>

## sz

```cpp
inline ll sz(const T& x);
```

コンテナのsizeをllへ変換して返します。

**計算量**

- O(1)。

<a id="api-5"></a>

## operator<<

```cpp
ostream& operator<<(ostream& os,const mint& x);
```

mintのval()を標準ストリームへ出力します。

**計算量**

- O(1)。

<a id="api-6"></a>

## operator>>

```cpp
istream& operator>>(istream& is,mint& x);
```

整数を読みmintへ変換します。

**計算量**

- O(1)。

<a id="api-7"></a>

## chmin

```cpp
inline bool chmin(T& a,const T& b);
```

b<aならa=bとしtrue、変更なしはfalse。

**計算量**

- 比較・代入の時間。

<a id="api-8"></a>

## chmax

```cpp
inline bool chmax(T& a,const T& b);
```

a<bならa=bとしtrue、変更なしはfalse。

**計算量**

- 比較・代入の時間。

<a id="api-9"></a>

## LB

```cpp
ll LB(const vector<T>& v,const U& a);
```

ソート済みvectorでa以上となる最初の添字を返します。存在しなければsize。

**制約**

- 昇順でソート済み

**計算量**

- O(log N)。

<a id="api-10"></a>

## UB

```cpp
ll UB(const vector<T>& v,const U& a);
```

ソート済みvectorでaより大きい最初の添字を返します。

**制約**

- 昇順でソート済み

**計算量**

- O(log N)。

<a id="api-11"></a>

## vec_min

```cpp
T vec_min(const vector<T>& v);
```

vectorの最小値を返します。

**制約**

- 非空

**計算量**

- O(N)。

<a id="api-12"></a>

## vec_max

```cpp
T vec_max(const vector<T>& v);
```

vectorの最大値を返します。

**制約**

- 非空

**計算量**

- O(N)。

<a id="api-13"></a>

## vec_sum

```cpp
T vec_sum(const vector<T>& v);
```

vectorの要素の和を返します。

**計算量**

- O(N)。

<a id="api-14"></a>

## cin_vl

```cpp
vector<T> cin_vl(ll n);
```

n個の値を標準入力から読みvector<T>で返します。T省略時ll。

**計算量**

- O(n)。

<a id="api-15"></a>

## cin_vvl

```cpp
vector<vector<T>> cin_vvl(ll h,ll w);
```

h×w個の値を標準入力から読み二次元vectorで返します。

**計算量**

- O(hw)。

<a id="api-16"></a>

## cin_xy

```cpp
pair<vector<T>,vector<T>> cin_xy(ll n);
```

n行の2値を読み、2本のvectorとして返します。

**計算量**

- O(n)。

<a id="api-17"></a>

## cin_xyz

```cpp
tuple<vector<T>,vector<T>,vector<T>> cin_xyz(ll n);
```

n行の3値を読み、3本のvectorとして返します。

**計算量**

- O(n)。

<a id="api-18"></a>

## Cin_vl

```cpp
vector<vector<T>> Cin_vl(ll n,ll k);
```

n行k列の値を読み、列ごとのk本のvectorで返します。

**計算量**

- O(nk)。

<a id="api-19"></a>

## cin_pairs

```cpp
vector<pair<T,T>> cin_pairs(ll n);
```

n個のpairを入力します。

**計算量**

- O(n)。

<a id="api-20"></a>

## cin_tuples

```cpp
vector<tuple<T,T,T>> cin_tuples(ll n);
```

n個の3値tupleを入力します。

**計算量**

- O(n)。

<a id="api-21"></a>

## cout_vl

```cpp
void cout_vl(const vector<T>& a,char sep=' ',char end='\n');
```

vectorを区切りsepと末尾endで出力します。

**計算量**

- O(N)。

<a id="api-22"></a>

## Cout_vl

```cpp
void Cout_vl(const vector<T>& a,const vector<T>& b);
```

2本の同じ長さのvectorを、各行に2値ずつ出力します。

**制約**

- 2本の長さが一致

**計算量**

- O(N)。

<a id="api-23"></a>

## cout_vvl

```cpp
void cout_vvl(const vector<vector<T>>& a);
```

二次元vectorを行ごとに出力します。

**計算量**

- 全要素数に比例。

<a id="api-24"></a>

## Swap

```cpp
void Swap(vector<vector<T>>& v,ll& h,ll& w);
```

h×w行列を転置し、h,wも交換します。

**計算量**

- O(hw)。

<a id="api-25"></a>

## ceil_div

```cpp
ll ceil_div(ll a,ll b);
```

整数a/bの切り上げを返します。負数にも対応する意図です。

**制約**

- b!=0、符号反転・加算がllに収まる

**計算量**

- O(1)。

<a id="api-26"></a>

## floor_div

```cpp
ll floor_div(ll a,ll b);
```

整数a/bの切り下げを返します。

**制約**

- b!=0、符号反転・加算がllに収まる

**計算量**

- O(1)。

<a id="api-27"></a>

## isqrt

```cpp
ll isqrt(ll n);
```

floor(sqrt(n))を返します。

**制約**

- n>=0

**計算量**

- 浮動小数の初期近似と補正に依存、通常定数程度。

<a id="api-28"></a>

## sort_unique_vec

```cpp
vector<T> sort_unique_vec(vector<T> v);
```

コピーした列をソート・重複除去して返します。

**計算量**

- O(N log N)。

<a id="api-29"></a>

## compress

```cpp
vector<ll> compress(const vector<T>& v);
```

各要素をソート・重複除去した列内の添字へ圧縮します。

**計算量**

- O(N log N)。

<a id="api-30"></a>

## prefix_sum

```cpp
vector<T> prefix_sum(const vector<T>& v);
```

長さN+1の累積和を返します。0番目は0。

**計算量**

- O(N)。

<a id="api-31"></a>

## prefix_sum2

```cpp
vector<vector<T>> prefix_sum2(const vector<vector<T>>& a);
```

二次元の累積和を返します。上端と左端に0行・0列を追加します。

**制約**

- 非空の長方形行列

**計算量**

- O(hw)。

<a id="api-32"></a>

## range_sum

```cpp
T range_sum(const vector<T>& s,ll l,ll r);
```

累積和sから[l,r)の和s[r]-s[l]を返します。

**計算量**

- O(1)。

<a id="api-33"></a>

## run_length

```cpp
vector<pair<T,ll>> run_length(const vector<T>& v);
vector<pair<char,ll>> run_length(const string& s);
```

連続する同値を {値,連続数} の列へ圧縮して返します。

**計算量**

- O(N)。

<a id="api-34"></a>

## argmin

```cpp
ll argmin(const vector<T>& v);
```

最小値を持つ最初の添字を返します。

**制約**

- 非空

**計算量**

- O(N)。

<a id="api-35"></a>

## argmax

```cpp
ll argmax(const vector<T>& v);
```

最大値を持つ最初の添字を返します。

**制約**

- 非空

**計算量**

- O(N)。

<a id="api-36"></a>

## in_vec

```cpp
bool in_vec(const vector<T>& v,const T& x);
```

ソート済みvectorにxがあるかを返します。

**制約**

- ソート済み

**計算量**

- O(log N)。

<a id="api-37"></a>

## even

```cpp
bool even(ll x);
```

xが偶数かを返します。

**計算量**

- O(1)。

<a id="api-38"></a>

## odd

```cpp
bool odd(ll x);
```

xが奇数かを返します。

**計算量**

- O(1)。

<a id="api-39"></a>

## grid_id

```cpp
ll grid_id(ll x,ll y,ll w);
```

幅wの座標(x,y)をx*w+yへ変換します。

**計算量**

- O(1)。

<a id="api-40"></a>

## grid_xy

```cpp
pll grid_xy(ll id,ll w);
```

idを{行,列}={id/w,id%w}へ戻します。

**制約**

- w>0

**計算量**

- O(1)。

<a id="api-41"></a>

## gcd

```cpp
ll gcd(ll a,ll b);
```

整数a,bの最大公約数を返します。

**計算量**

- O(log(max(|a|,|b|)+1))。

<a id="api-42"></a>

## lcm

```cpp
ll lcm(ll a,ll b);
```

最小公倍数a/gcd(a,b)*bを返します。

**制約**

- gcd(a,b)!=0、結果がllに収まる

**計算量**

- gcdと同じ。

<a id="api-43"></a>

## Exp

```cpp
ll Exp(ll x,ll y);
```

xをyで割り続け、y進表記の桁数-1を返します。

**制約**

- x>=1、y>=2

**計算量**

- O(log_y x)。

<a id="api-44"></a>

## powmod

```cpp
ll powmod(ll x,ll k,ll m);
```

128bitの中間積でx^k mod mを計算します。

**制約**

- k>=0、m>0

**計算量**

- O(log(k+1))。

<a id="api-45"></a>

## square

```cpp
T square(T x);
```

x*xを返します。

**計算量**

- 乗算の時間。

<a id="api-46"></a>

## Uni_erase

```cpp
void Uni_erase(vector<T>& v);
```

vectorをその場でソート・重複除去します。

**計算量**

- O(N log N)。

<a id="api-47"></a>

## OutIn

```cpp
inline bool OutIn(ll x,ll n);
inline bool OutIn(ll x,ll y,ll h,ll w);
inline bool OutIn(ll x,ll y,ll z,ll a,ll b,ll c);
```

指定された添字が0以上で各サイズ未満かを返します。

**計算量**

- O(1)。

<a id="api-48"></a>

## Random

```cpp
ll Random(ll l,ll r);
```

半開区間[l,r)の一様乱数を返します。

**制約**

- l<r

**計算量**

- 乱数生成器に依存。

<a id="api-49"></a>

## YesNo

```cpp
void YesNo(bool x);
```

trueならYes、falseならNoを出力します。

**計算量**

- O(1)。

<a id="api-50"></a>

## vl_n

```cpp
vl vl_n(ll n);
```

0..n-1のvector<ll>を返します。

**計算量**

- O(n)。

<a id="api-51"></a>

## vl_N

```cpp
vl vl_N(ll n);
```

1..nのvector<ll>を返します。

**計算量**

- O(n)。

<a id="api-52"></a>

## factorial

```cpp
void factorial(ll N=1000000LL);
```

N+100までの階乗・逆階乗を作ります。版によって2の累乗表も作ります。既存表は再構築します。

**制約**

- 0<=N、N+100<mod

**計算量**

- O(N+log mod)。

<a id="api-53"></a>

## ensure_factorial

```cpp
void ensure_factorial(ll N);
```

表が不足している場合にfactorial(N)を呼びます。

**計算量**

- 足りる場合O(1)、不足時factorialと同じ。

<a id="api-54"></a>

## nCr

```cpp
mint nCr(ll N,ll r);
```

C(N,r)をmintで返します。N<0/r<0/r>Nでは0。必要なら階乗表を前計算します。

**計算量**

- 前計算済みならO(1)、不足時O(N+log mod)。

<a id="api-55"></a>

## nPr

```cpp
mint nPr(ll N,ll r);
```

N!/(N-r)!をmintで返します。範囲外では0。

**計算量**

- 前計算済みならO(1)、不足時O(N+log mod)。

<a id="dijkstra"></a>

## dijkstra（単一始点・複数始点の最短距離）

6個のオーバーロードを各1行にまとめ、テンプレートの88〜93行目に配置しています。

```cpp
vl dijkstra(const graph& g,ll s);
vl dijkstra(const graph& g,const vl& starts);
vl dijkstra(const graph& g,initializer_list<ll> starts);
vl dijkstra(const Graph& g,ll s);
vl dijkstra(const Graph& g,const vl& starts);
vl dijkstra(const Graph& g,initializer_list<ll> starts);
```

| 引数 | 内容 |
| --- | --- |
| `g` | `graph` は行き先の列、`Graph` は `(行き先, 重み)` の列からなる隣接リスト |
| `s` | 単一の始点 |
| `starts` | 複数の始点。`vl` または `{s1,s2,...}` で指定 |

`N=g.size()` 項の `vl` を返します。`dist[v]` は始点集合のいずれかから頂点 `v` までの最短距離で、各始点の距離は `0`、未到達は `inf`（`9223372036854775807LL`）です。始点の重複は無視し、始点集合が空なら全要素が `inf` になります。入力グラフは変更しません。

`graph` の各辺は距離 `1` としてBFSで処理します。`Graph` は非負重みをダイクストラ法で処理します。有向グラフに対応し、無向グラフでは両方向の辺を登録してください。自己ループ・多重辺・重み `0` にも対応します。

**制約・添字**

- 頂点番号・始点・各辺の行き先は `0` 以上 `N` 未満です。`dist[0],...,dist[N-1]` が頂点番号順の距離です。
- 1-indexedの隣接リストを `g(N+1)` で用意した場合、返り値も `N+1` 項になります。`dist[1],...,dist[N]` を使い、ちょうど `N` 項必要なら先頭要素を取り除いてください。
- `Graph` の重みは `0` 以上 `inf` 以下です。必要な最短距離は `inf` 未満としてください。距離の和が `inf` 以上になる遷移は加算前に除外するため、符号付き整数のオーバーフローを起こしません。そのような距離しか持たない頂点も `inf` になります。
- 頂点数 `0` の場合は空の始点集合を指定してください。

**計算量**

`M` は隣接リストに登録した辺の総数、`K` は始点リストの長さ（単一始点では `1`）です。

| 型 | 時間 | 返り値を含む追加メモリ |
| --- | --- | --- |
| `graph` | `O(N+M+K)` | `O(N)`。波括弧の始点指定では変換用に別途 `O(K)` |
| `Graph` | `O(N+K+M log(M+2))` | `O(N+M)`。波括弧の始点指定では変換用に別途 `O(K)` |

単純グラフの重み付き版は通常の `O((N+M) log(N+1)+K)` と表せます。既存の基本テンプレートに含まれる定義だけで利用でき、ヘッダーの追加は不要です。

```cpp
graph g={{1},{2},{},{}};
assert((dijkstra(g,0)==vl{0,1,2,inf}));
assert((dijkstra(g,vl{0,2})==vl{0,1,0,inf}));
Graph wg={{{1,5},{2,20}},{{2,7}},{},{}};
assert((dijkstra(wg,0)==vl{0,5,12,inf}));
assert((dijkstra(wg,{0,2})==vl{0,5,0,inf}));
```

<a id="pow2-table"></a>

## Pow2（2の累乗の前計算済み配列）

```cpp
const vl Pow2;
```

`Pow2[i]` は `2^i`（`0<=i<=60`）です。61個の値をすべて `ll` 型の整数リテラルとして展開し、テンプレートの95行目に配置しています。利用時の前計算関数の呼び出しは不要です。

`Pow2[0]=1LL`、`Pow2[60]=1152921504606846976LL` です。全要素が `ll` の正数範囲内に収まります。

| 操作 | 返り値 | 制約 | 計算量 |
| --- | --- | --- | --- |
| `Pow2[i]` | `const ll&`（値は `2^i`） | `0<=i<=60` | `O(1)` |
| `Pow2.size()` | `size_t`、常に61 | なし | `O(1)` |

配列は読み取り専用です。法を取らない整数値を保持し、追加メモリは固定長61項です。

```cpp
ll p=Pow2[10];
assert(p==1024LL);
assert(Pow2.size()==61 && Pow2[60]==1152921504606846976LL);
```

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `ll` | long long |
| `LL` | __int128 |
| `mint` | modint998244353 |
| `vl` | vector<ll> |
| `graph` | vector<vector<ll>> |
| `Graph` | vector<vector<pair<ll,ll>>>、各辺は (行き先, 重み) |
| `rep(i,n)` | 0<=i<nのループ |
| `all(v)` | begin,endのpair |
| `fac,finv` | 階乗と逆階乗の表 |
| `Pow2` | `const vl`、`2^0`〜`2^60` の61項 |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
vl a={3,1,3}; assert(vec_sum(a)==7);
assert((compress(a)==vl{1,0,1}));
assert(ceil_div(-5,2)==-2 && floor_div(-5,2)==-3);
factorial(10); assert(nCr(5,2).val()==10);
assert(Pow2.size()==61 && Pow2[0]==1LL && Pow2[60]==1152921504606846976LL);
graph g={{1},{2},{},{}};
assert((dijkstra(g,0)==vl{0,1,2,inf}));
assert((dijkstra(g,vl{0,2})==vl{0,1,0,inf}));
Graph wg={{{1,5},{2,20}},{{2,7}},{},{}};
assert((dijkstra(wg,0)==vl{0,5,12,inf}));
assert((dijkstra(wg,{0,2})==vl{0,5,0,inf}));
```

## メモリ

入力や補助表のサイズに依存。

## 使用例の確認状況

GNU C++23とACLで、掲載例のコンパイル・実行を確認しています。ダイクストラは4種類のテンプレートそれぞれで、境界条件とFloyd–Warshall法によるランダム比較を行っています。

検証の際の準備:

- 共通定義を使い、mainに使用例を記入

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
