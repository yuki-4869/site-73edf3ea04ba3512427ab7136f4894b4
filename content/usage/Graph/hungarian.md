# 最小費用割当（Hungarian法）

[説明書の目次](../index.md) · [コード](../../../Graph/hungarian.cpp)

N人へ異なる仕事を1つずつ割り当て、費用の合計を最小化します。M個の仕事からN個を選べる長方形の行列にも対応します。
担当者と仕事、物体と移動先、2つの集合の要素間の対応など、各組の費用が与えられた割当問題に使えます。

[二部最大マッチング](hopcroft_karp.md) が対応数を最大化するのに対し、この実装は**全行を必ず割り当て、その費用を最小化**します。
Hungarian法のポテンシャルと増加路を用いた実装で、計算量はO(N²M)、正方形ならO(N³)です。
型エイリアスと貼り付け方は [共通設定](../common.md) を参照してください。基本テンプレート以外のincludeやACLへの依存はありません。

## 型・前提

```cpp
using ll = long long;
using LL = __int128;
using vl = vector<ll>;
using vvl = vector<vl>;

struct HungarianResult {
    LL cost = 0;
    vl column;
};
```

- 入力はN行M列の費用行列で、`cost[i][j]` は行iを列jへ割り当てた費用です。
- `0 <= N <= M <= INT_MAX` とし、全行の長さが同じである必要があります。非空ならM>=1です。
- 0行の入力は `vvl{}` で表します。この場合は列数を使わず、費用0・空の割当を返します。
- 行番号は `0..N-1`、列番号は `0..M-1` という独立した0-indexedです。
- 各行をちょうど1列へ割り当て、各列は高々1回使います。N<Mの場合、未使用の列があって構いません。
- すべての組を選べる完全な費用行列を受け取ります。禁止辺を示す特別な数値はなく、非常に大きな値も通常の有限費用として扱います。
- 各費用はllの範囲全体、すなわちLLONG_MINからLLONG_MAXまで使用できます。負数・0・重複も許します。
- 費用の差・ポテンシャル・合計は128 bit整数で計算します。入力の寸法と費用の範囲では、これらの計算に十分な範囲があります。必要なメモリを確保できることが前提です。

## 構築方法

実装をグローバルに置き、費用行列をhungarianへ渡します。クラスの構築やbuild操作は不要です。
呼び出すたびに独立して解き直し、過去の計算状態は保持しません。

## hungarian

```cpp
HungarianResult hungarian(const vvl& cost);
```

引数costに対する最小費用割当を返します。入力行列を変更せず、コピーも保持しません。
全行の割当が存在することは、N<=Mと全組を選べるという前提により保証されます。

最適解が複数ある場合は、そのうち1つを返します。割当列の辞書順最小性は保証しません。
行数が列数を超える場合、不揃いの行がある場合、行数・列数がINT_MAXを超える場合は、NDEBUGでもメッセージなしで停止します。
入力が0行なら、それ以上の処理をせず空の結果を返します。

**計算量**：O(1+N²M)時間、入力行列を除くO(N+M+1)領域です。N=MならO(N³)、0行なら時間・領域ともO(1)です。
入力行列そのものの領域はO(NM)です。再帰を使わずに探索します。

## HungarianResult::cost

```cpp
LL result.cost;
```

最小の費用合計を保持します。次の値と一致します。

$$
\sum_{i=0}^{N-1}\mathrm{cost}[i][\mathrm{result.column}[i]].
$$

型はllではなく `LL = __int128` です。個々の費用がllに収まっていても、合計はllを超える場合があります。
通常の `cout << result.cost` は使用できないため、128 bit整数用の出力処理を用意してください。llへ変換する場合は、値がllの範囲に収まることを確認します。

## HungarianResult::column

```cpp
vl result.column;
```

長さNの割当先の列番号です。各iについて `0 <= result.column[i] < M` が成り立ち、すべての列番号が異なります。
0行の場合は空です。返り値は独立したvectorなので、入力行列の寿命や後の書き換えに依存しません。

結果のフィールドは公開されており、書き換えることができます。書き換えた後の最適性やcostとの一致を自動で再計算するものではありません。

## 使用例

実装をグローバルに置き、以下をmain内などで実行します。

```cpp
vvl cost={{4,1,3},{2,0,5}};
auto original=cost;
auto result=hungarian(cost);
assert(result.cost==LL(3));
assert((result.column==vl{1,0}));
assert(cost==original);

LL recomputed=0;
for(ll i=0;i<(ll)cost.size();++i)recomputed+=LL(cost[i][result.column[i]]);
assert(recomputed==result.cost);

vvl negative={{-5,2},{4,-3}};
auto best=hungarian(negative);
assert(best.cost==LL(-8));
assert((best.column==vl{0,1}));

vvl large={{LLONG_MAX,LLONG_MAX},{LLONG_MAX,LLONG_MAX}};
auto wide=hungarian(large);
assert(wide.cost==LL(2)*LLONG_MAX);
assert(wide.column[0]!=wide.column[1]);
auto empty=hungarian(vvl{});
assert(empty.cost==0 && empty.column.empty());
```

最初の例では、行0を列1へ、行1を列0へ割り当てると合計 `1+2=3` になります。列2は使いません。

最大費用の割当を求めたい場合は、各費用の符号を反転して最小化する方法があります。ただしLLONG_MINの符号反転はllに収まらないため、この入力形式では適用できません。

## アルゴリズム

行を1つずつ追加し、その行を含む最適な割当へ増加させます。行と列にポテンシャルを持たせ、元の費用からポテンシャルを引いた縮約費用を使います。
現在の割当をたどって行を増やしながら、各未訪問列へ到達するための最小の余裕を保持します。

余裕が最小の列に到達できるようポテンシャルを更新し、その列が使用済みなら割り当てられた行から探索を続けます。
未使用列に到達したら、保存しておいた経路を逆にたどって対応を付け替えます。各段階で割当数が1増えます。
負の費用を含む場合も、最初の余裕の更新を128 bit整数で計算することで同じ処理を行えます。

k行目を追加する段階では高々k個の行を探索し、1回の探索でM列を走査します。全段階の合計はO(N²M)です。
内部には0番の仮想列を置きますが、APIへ渡す行列と返す列番号は0-indexedのままです。

## 確認結果

専用テスト `tests/hungarian_test.cpp` で、小規模の費用行列を全列挙し、異なる列の選び方をすべて試す別解と照合します。
ランダムな長方形行列は、順列列挙とACLの最小費用流にも照合します。
空・1行・同値の最適解・負数・LLONG_MIN／LLONG_MAX・llを超える合計・不正な形状の停止、大きな正方形・長方形を確認します。
C++23・NDEBUG・AddressSanitizer／UBSanで検証します。

## 参考

- [Kuhn, The Hungarian method for the assignment problem](https://doi.org/10.1002/nav.3800020109)：割当問題を解くHungarian法の原論文です。
- [Library Checker — Assignment Problem](https://judge.yosupo.jp/problem/assignment)：正方形の割当問題です。オンライン提出を実施したことを示すものではありません。
