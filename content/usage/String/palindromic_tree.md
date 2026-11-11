# 回文木（Eertree）

[説明書の目次](../index.md) · [コード](../../../String/palindromic_tree.cpp)

英小文字列に含まれる異なる回文をIDで管理します。末尾へ1文字ずつ追加しながら、異なる回文数と最長回文接尾辞を取得できます。
重なりを含む各回文の出現回数、最初の出現区間、最長の真の回文接尾辞へのリンクも取得できます。
異なる回文の集計や、回文を接尾辞に持つ文字列をオンラインで扱う問題に使えます。

[回文半径](palindrome_radius.md) は位置ごとの奇数長回文の半径を扱います。回文木は、異なる回文を1つずつノードにまとめ、奇数長・偶数長の両方を同じ方法で管理します。
型エイリアスと貼り付け方は [共通設定](../common.md) を参照してください。追加includeや別ライブラリへの依存はありません。

## 型・前提

```cpp
using ll = long long;
using vl = vector<ll>;
using pll = pair<ll,ll>;
```

- 文字は英小文字 `a..z` だけを使用できます。英大文字・数字・空白・NUL・UTF-8文字には対応しません。
- 現在の文字数をN、異なる空でない回文数をPとします。`N <= INT_MAX-2` が必要で、PはN以下です。
- メモリを確保できることが前提です。各ノードが26文字分のint遷移表を持つため、遷移表だけで約 `104*(P+2)` byte使います。実際には長さ・リンク・出現数などとvectorの余剰容量も必要です。
- 空文字列にも対応します。空回文は公開IDの対象にしません。
- 回文IDは初登場順の `0..P-1` です。末尾追加で既存IDが変わることはありません。clear後には同じ番号が別の回文を表し得ます。
- 文字列の位置は0-indexed、出現区間は半開区間 `[l,r)` です。
- 元のstringはコピーして保持します。呼び出し側の文字列を書き換えても状態は変わりません。返す区間に対応する文字列は呼び出し側で管理してください。
- 末尾追加だけに対応します。文字の削除・途中への挿入・変更はできません。
- 不正文字・文字数上限の超過・範囲外IDは、NDEBUGでもメッセージなしで停止します。
- 再帰・ハッシュ・乱数を使いません。

## 構築方法

```cpp
PalindromicTree tree;
PalindromicTree tree(s);
```

既定構築は空状態を作ります。sは `const string&` で受け取り、その全文を末尾追加して構築します。引数s自体は変更しません。
構築後もpushで追加できます。別途buildする必要はありません。

**計算量**：空の構築はO(1)。sを指定した構築はO(N)時間・領域です。空ならO(1)です。
固定26文字なので、遷移表の初期化も定数時間として扱います。

## clear

```cpp
void tree.clear();
```

全内容を破棄し、空状態へ戻します。返り値はありません。以前の回文IDは無効になります。
確保済みの文字列・ノード領域も解放します。既に返した出現回数のvectorや区間のpair自体は変化しません。

**計算量**：呼び出し前のNに対してO(N)時間、空ならO(1)。完了後の保持領域はO(1)です。

## push

```cpp
ll tree.push(char c);
```

引数cを末尾へ追加し、**追加後の最長回文接尾辞のID**を返します。
新しい回文が生まれなかった場合も既存のIDを返します。返り値を新規IDや新規追加の成否と解釈しないでください。
新しい回文の有無は、呼び出し前後のdistinct_countを比較すると分かります。増える数は高々1です。

**計算量**：償却O(1)時間。1回だけの最悪時間は、接尾辞リンクの探索やvectorの再確保によりO(N)です。
空からN文字追加する合計はO(N)時間・領域です。1回の呼び出しで作る新ノードは高々1個ですが、再確保時には既存ノードも移動します。

## size

```cpp
ll tree.size() const;
```

現在の文字数Nを返します。回文数ではありません。

**計算量**：O(1)時間。

## distinct_count

```cpp
ll tree.distinct_count() const;
```

異なる空でない回文の数Pを返します。同じ内容の回文が複数の場所に出現しても1つと数えます。

**計算量**：O(1)時間。

## longest_suffix

```cpp
ll tree.longest_suffix() const;
```

現在の文字列の最長回文接尾辞のIDを返します。空文字列なら-1です。
非空なら `length(longest_suffix())` でその長さを取得できます。文字列全体における最長回文部分文字列とは異なります。

**計算量**：O(1)時間。

## length

```cpp
ll tree.length(ll id) const;
```

指定した回文の文字数を返します。引数は `0 <= id < P` とします。返り値は1以上です。

**計算量**：O(1)時間。

## first_occurrence

```cpp
pll tree.first_occurrence(ll id) const;
```

指定した回文が最初に現れた半開区間 `[l,r)` を返します。引数は `0 <= id < P` とします。
`r-l == length(id)` で、同じ回文の出現の中でlが最小となる区間です。文字列そのものは返しません。
元の文字列をsとして保持していれば、`s.substr(l,r-l)` で内容を取り出せます。

**計算量**：区間の取得はO(1)時間。substrで実体をコピーする場合は、その回文長に比例する時間・領域が別途必要です。

## suffix_link

```cpp
ll tree.suffix_link(ll id) const;
```

指定した回文の、最長の空でない真の回文接尾辞のIDを返します。
引数は `0 <= id < P` とします。該当する空でない接尾辞がなければ-1です。1文字の回文では-1になります。
返したIDは指定IDより小さく、その回文長も必ず短くなります。

**計算量**：1回の取得はO(1)時間です。リンクを繰り返して全回文接尾辞を列挙する場合は、その個数に比例し、最悪O(N)時間です。

## occurrence_counts

```cpp
vl tree.occurrence_counts() const;
```

現在の文字列について、各IDの回文の出現回数をID順のvectorで返します。長さはPです。
出現位置の重なりも数えます。例えば `aaa` における `aa` の出現は `[0,2)` と `[1,3)` の2回です。
空状態では空vectorを返します。

本体の状態を変更しないため、繰り返し取得しても数が重複加算されません。取得後にpushして再度取得することもできます。
返り値は独立したvectorで、後からpush・clearしても以前の返り値は変わりません。個々の出現数はN以下なのでllに収まります。

**計算量**：O(P)時間・追加領域、空ならO(1)です。
全接頭辞ごとに呼び出すと、呼び出し回数に応じて合計O(N²)になり得ます。異なる回文数だけが必要ならdistinct_countを使います。

## 使用例

実装をグローバルに置き、以下をmain内などで実行します。

```cpp
string s="ababa";
PalindromicTree tree(s);
assert(tree.size()==5 && tree.distinct_count()==5);
ll id=tree.longest_suffix();
assert(tree.length(id)==5);
assert(tree.length(tree.suffix_link(id))==3);
assert((tree.first_occurrence(id)==pll{0,5}));
auto counts=tree.occurrence_counts();
assert((counts==vl{3,2,2,1,1}));
assert(tree.occurrence_counts()==counts);

vector<string> words;
for(ll i=0;i<tree.distinct_count();++i){
    auto [l,r]=tree.first_occurrence(i);
    words.push_back(s.substr(l,r-l));
}
assert((words==vector<string>{"a","b","aba","bab","ababa"}));

s+='b';
ll next=tree.push('b');
assert(tree.length(next)==5);
assert((tree.occurrence_counts()==vl{3,3,2,2,1,1}));
assert((counts==vl{3,2,2,1,1}));

PalindromicTree equal("aaa");
assert(equal.distinct_count()==3);
assert((equal.occurrence_counts()==vl{3,2,1}));
PalindromicTree existing("abca");
assert(existing.distinct_count()==3);
assert(existing.length(existing.longest_suffix())==1);
tree.clear();
assert(tree.size()==0 && tree.distinct_count()==0 && tree.longest_suffix()==-1);
assert(tree.occurrence_counts().empty());
assert(tree.push('z')==0);
```

回文の実体をすべてコピーすると、長さの総和がO(N²)になる場合があります。ID・長さ・出現区間を使えば、そのコピーを避けられます。

## アルゴリズム

内部には長さ-1と0の仮想根を置きます。各実ノードは異なる空でない回文を1つ表し、文字cによる遷移で両端にcを付けた回文へ移動します。
各ノードから、最長の真の回文接尾辞へのリンクも持ちます。仮想根は公開IDに含めません。

末尾追加では、以前の最長回文接尾辞からリンクをたどり、新しい文字で両側を囲める最長の回文を探します。
対応する遷移がなければ1ノードを作成し、同様の探索でその接尾辞リンクを求めます。追加後の最長回文接尾辞だけに生のカウンタを1加算します。

出現回数を取得するときは生のカウンタをコピーし、IDの降順に接尾辞リンクへ加算します。
リンク先は先に作成済みなのでIDが小さく、この順序で全回文の重なりを含む出現回数が求まります。
作業用コピーだけを更新するため、取得の繰り返しや取得後の追加にも対応できます。

## 確認結果

専用テスト `tests/palindromic_tree_test.cpp` で、小規模文字列を全列挙し、各接頭辞の全部分文字列を直接調べる別解と照合します。
異なる回文数、IDと内容、長さ、最初の出現、最長回文接尾辞、接尾辞リンク、重なりを含む出現数を確認します。
空・偶数回文・同値文字列・再初期化・全英小文字・不正文字やIDの停止、大きな文字列をC++23・NDEBUG・AddressSanitizer／UBSanで検証します。

## 参考

- [Rubinchik, Shur — EERTREE: An Efficient Data Structure for Processing Palindromes in Strings](https://arxiv.org/abs/1506.04862)：回文木の原論文です。
