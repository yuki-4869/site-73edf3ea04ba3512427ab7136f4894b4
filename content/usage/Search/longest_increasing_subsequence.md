# 最長増加部分列（LIS）

[説明書の目次](../index.md) · [コード](../../../Search/longest_increasing_subsequence.cpp)

整数列から元の順序を保って要素を選び、値が増加する部分列の最大長と、その添字を求めます。
要素は連続していなくても構いません。既定では同じ値を続けて選べない狭義増加です。
`strict=false` を指定すると、同じ値を許す最長非減少部分列を求められます。

列のDP、順序を保った選択、列をソート済みにするために取り除く要素数の最小化などに使えます。
削除後を狭義増加にする最小削除数は `N-result.length` です。通常の昇順（同値を許す）にするなら `strict=false` を使います。
型エイリアスと貼り付け方は [共通設定](../common.md) を参照してください。追加のincludeやACLへの依存はありません。

## 型・前提

```cpp
using ll = long long;
using vl = vector<ll>;

struct LISResult {
    ll length = 0;
    vl indices, ending_length;
};
```

- `a` は長さ `0 <= N <= INT_MAX` の列です。必要なメモリを確保できることが前提です。
- 値にはllの範囲全体（LLONG_MINからLLONG_MAX）、負数、重複を使えます。
- 添字は0-indexedです。選ぶ添字は必ず狭義増加で、同じ位置を2回選ぶことはありません。
- `strict=true` なら選んだ値は狭義増加、`strict=false` なら非減少です。
- 入力を変更せず、値の加減算や番兵値を使いません。座標圧縮も不要です。
- 最適な添字列が複数ある場合は1つを返します。値列・添字列の辞書順最小性は保証しません。
- 空入力では長さ0、2つのvectorは空です。

## 構築方法

実装をグローバルに貼り付け、`longest_increasing_subsequence(a)` を呼び出します。
クラスの構築やbuildは不要です。呼び出すたびに独立して計算し直します。
入力を変更した場合も、以前の返り値は変わりません。

## longest_increasing_subsequence

```cpp
LISResult longest_increasing_subsequence(const vl& a, bool strict = true);
```

引数aに対する最長部分列を返します。strictは同値を許すかどうかの指定です。
`true` では隣り合う選択値の間に `<`、`false` では `<=` を要求します。
返り値の各フィールドの意味は次の節を参照してください。

NがINT_MAXを超える場合は、NDEBUGでもメッセージなしで停止します。

**計算量**：O(N log N)時間、入力列を除いて返り値も含めO(N)領域です。
空入力は時間・領域ともO(1)です。再帰を使いません。

## LISResult::length

```cpp
ll result.length;
```

選べる要素数の最大値Lです。`0 <= L <= N` が成り立ちます。
結果の取得はO(1)時間です。戻り値を既定構築した場合の値は0です。

## LISResult::indices

```cpp
vl result.indices;
```

長さLの復元結果で、値ではなく元の列の添字を保持します。
`indices[0] < indices[1] < ... < indices[L-1]` で、各添字は `0..N-1` の範囲です。
`a[indices[j]]` を順に並べると、strictの指定に従う最長部分列になります。

1要素の参照はO(1)、全走査や値列への変換はO(L)時間です。既定構築時は空です。

## LISResult::ending_length

```cpp
vl result.ending_length;
```

長さNの列で、`ending_length[i]` は**位置iを最後に選ぶ**最長部分列の長さです。
非空入力なら各要素は1以上で、最大値がlengthと一致します。
位置iまでの接頭辞全体の最長長は、`ending_length[0..i]` の最大値です。
例えば `a={1,2,3,0}` の狭義増加では `ending_length={1,2,3,1}` であり、最後の値は接頭辞全体の最長長3とは異なります。

1要素の参照はO(1)、全走査はO(N)時間です。既定構築時は空です。

LISResultの各フィールドは公開され、返り値は入力の寿命に依存しません。
フィールドを書き換えた場合、ほかのフィールドは自動では更新されません。

## 使用例

実装をグローバルに置き、以下をmain内などで実行します。

```cpp
vl a={3,1,2,2,4};
auto original=a;
auto result=longest_increasing_subsequence(a);
assert(result.length==3);
assert((result.ending_length==vl{1,1,2,2,3}));
vl picked;
for(ll i:result.indices)picked.push_back(a[i]);
assert((picked==vl{1,2,4}));
assert(a==original);

auto nondecreasing=longest_increasing_subsequence(a,false);
assert(nondecreasing.length==4);
assert((nondecreasing.indices==vl{1,2,3,4}));
assert((nondecreasing.ending_length==vl{1,1,2,3,4}));

vl prefix_length;
ll best=0;
for(ll x:result.ending_length){best=max(best,x);prefix_length.push_back(best);}
assert((prefix_length==vl{1,1,2,2,3}));

auto empty=longest_increasing_subsequence(vl{});
assert(empty.length==0 && empty.indices.empty() && empty.ending_length.empty());
auto extreme=longest_increasing_subsequence(vl{LLONG_MIN,LLONG_MAX});
assert(extreme.length==2);
```

非増加・狭義減少を求める場合は、列を反転して本関数を使う方法があります。
反転後の添字jを元の `N-1-j` に戻し、復元添字列自体も反転します。
元の列の値の符号を反転する方法は、LLONG_MINでオーバーフローするため使用できません。

## アルゴリズム

左から走査し、長さkの部分列について末尾値の最小値を `tails[k-1]` に保持します。
新しい値xが来たとき、狭義増加ではx以上の最初の末尾（lower_bound）、非減少ではxより大きい最初の末尾（upper_bound）を探します。
見つかった位置をk（0-indexed）とすると、xで終わる最長長はk+1です。
この末尾をxで置き換え、末尾がなければ追加します。各要素につき二分探索を1回行います。

tailsだけを返しても、有効な部分列が復元できるとは限りません。例えば `{2,3,1}` の最終tailsは `{1,3}` ですが、その順序では選べません。
この実装は、各長さの末尾の添字と、各要素を追加した瞬間の直前の添字を記録します。
最後に最長の末尾から直前の添字をたどり、反転して正しい添字順を復元します。

## 確認結果

専用テスト `tests/longest_increasing_subsequence_test.cpp` で、小規模列の全部分列列挙と照合します。
狭義増加と非減少の双方について、最長長・各位置で終わる長さ・復元添字の範囲と順序・選んだ値の条件・入力不変を確認します。
ランダム列はO(N²)のDPとも照合し、空・同値・昇順・降順・負数・llの両極値・100万要素を検証します。
C++23・NDEBUG・AddressSanitizer／UBSanでも実行します。

## 参考

- [Library Checker — Longest Increasing Subsequence](https://judge.yosupo.jp/problem/longest_increasing_subsequence)：狭義増加の長さと添字列の復元を求める問題です。オンライン提出の実施を示すものではありません。
