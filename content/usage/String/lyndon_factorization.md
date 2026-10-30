# Lyndon分解・最小巡回シフト

[説明書の目次](../index.md) · [コード](../../../String/lyndon_factorization.cpp)

文字列を辞書順に基づく一意なLyndon因子列へ分解し、巡回シフトの中で辞書順最小となる開始位置を求めます。
円環上の文字列の代表形、回転を同一視する文字列の比較、文字列の周期性や辞書順構造を利用する処理の部品に使えます。
どちらもDuval法を用い、文字列長に対して線形時間で計算します。

型エイリアス・貼り付け方は [共通設定](../common.md) を参照してください。基本テンプレートの標準ヘッダーだけで動作し、追加includeや他の個別ライブラリへの依存はありません。

## 型・前提

```cpp
using ll = long long;
using pll = pair<ll,ll>;
```

- 入力は `const string&` で受け取り、文字列を変更しません。長さをNとし、`N <= INT_MAX` が必要です。
- 長さ超過時はメッセージなしで停止します。NDEBUGでも検査は有効です。
- 添字は0-indexed、区間は半開区間 `[l,r)` です。
- 文字の大小は `unsigned char` に変換した値で比較します。`char` がsignedかunsignedかによって結果は変わりません。
- 埋め込みNULや上位bitの立ったbyteを含むstringも使用できます。UTF-8を渡した場合はbyte列として処理し、文字の途中からの巡回シフトも候補になります。Unicodeの文字単位の処理ではありません。
- 辞書順では、最初に異なる文字が小さい方を小さくし、一方が他方の接頭辞なら短い方を小さくします。
- 再帰・乱数・ハッシュを使いません。必要な返り値のメモリを確保できることを前提とします。

## Lyndon語とLyndon分解

Lyndon語は、空でなく、すべての空でない真の接尾辞より**厳密に辞書順で小さい**文字列です。
1文字の文字列はLyndon語です。例えば `ab` は `b` より小さいのでLyndon語ですが、`aa` は接尾辞 `a` より大きいためLyndon語ではありません。

任意の文字列は、Lyndon語を辞書順で非増加に並べた列へ一意に分解できます。因子を $w_1,w_2,\ldots,w_m$ とすると、$s=w_1w_2\cdots w_m$、$w_1\ge w_2\ge\cdots\ge w_m$ を満たします。
例えば `banana` は `b | an | an | a` に分かれます。同じ因子が続いてもまとめません。

## 構築方法

実装をグローバルに置き、文字列を関数へ直接渡します。クラスの初期化・build操作は不要です。2つの関数は独立に呼び出せます。

## lyndon_factorization

```cpp
vector<pll> lyndon_factorization(const string& s);
```

引数sのLyndon分解を、各因子の半開区間 `(l,r)` のvectorとして左から順に返します。
因子そのものの文字列はコピーしません。必要なら `s.substr(l,r-l)` で取り出します。

- 空文字列なら空のvectorを返します。
- 非空なら各区間は空でなく、先頭のlは0、隣接区間はつながり、末尾のrはNです。
- `s.substr(l,r-l)` がそれぞれLyndon語になり、因子列は辞書順で非増加です。
- 全文字が同じ場合は、各文字を長さ1の因子として返します。
- 返り値は添字だけを保持する独立したvectorです。入力文字列への参照を内部に残しません。元の文字列を書き換えた場合、区間の内容が元の分解を表すとは限りません。

**計算量**：O(N+1)時間。返り値は因子数をmとしてO(m+1)領域、最悪O(N+1)領域です。返り値以外の作業領域はO(1)です。
各因子の文字列をすべてコピーする場合は、その総長Nに比例する時間・領域が別にかかります。

## minimum_rotation

```cpp
ll minimum_rotation(const string& s);
```

引数sの巡回シフトのうち、辞書順最小となる開始位置pを返します。巡回シフトは `s.substr(p) + s.substr(0,p)`、すなわち先頭p文字を末尾へ移した文字列です。

- 非空なら `0 <= p < N` です。
- 同じ最小文字列を作る位置が複数ある場合は、**最小のp** を返します。例えば `baba` は位置1と3で `abab` になり、返り値は1です。
- 空文字列は便宜上0を返します。この場合、文字列の添字として `s[0]` を参照しないでください。
- 全文字が同じ場合と長さ1の場合は0です。
- 返り値は整数だけで、最小文字列そのものは生成しません。内部でもs+sのコピーは作りません。

**計算量**：O(N+1)時間、追加領域O(1)。返り値から実際の巡回シフト文字列を生成する場合は、別にO(N+1)時間・領域がかかります。

## 使用例

実装をグローバルに置き、以下をmain内などで実行します。

```cpp
string s="banana";
auto original=s;
auto factors=lyndon_factorization(s);
assert((factors==vector<pll>{{0,1},{1,3},{3,5},{5,6}}));
vector<string> words;
for(auto [l,r]:factors)words.push_back(s.substr(l,r-l));
assert((words==vector<string>{"b","an","an","a"}));
ll p=minimum_rotation(s);
assert(p==5);
string canonical=s.substr(p)+s.substr(0,p);
assert(canonical=="abanan");
assert(s==original);

assert(minimum_rotation("baba")==1);
assert(minimum_rotation("aaaa")==0);
assert(minimum_rotation("")==0);
assert(lyndon_factorization("").empty());
assert((lyndon_factorization("aaaa")==vector<pll>{{0,1},{1,2},{2,3},{3,4}}));

string binary;binary.push_back(char(255));binary.push_back(char(0));
assert(minimum_rotation(binary)==1);
assert((lyndon_factorization(binary)==vector<pll>{{0,1},{1,2}}));
```

### 回転を同一視した比較

2つの文字列についてminimum_rotationで得た代表形を比較すると、一方がもう一方の回転かどうかを判定できます。文字列長が異なれば回転ではありません。
反転も同一視する場合は、元の文字列と反転した文字列についてそれぞれ最小巡回シフトを求め、2つの代表形の小さい方を使います。反転はこの関数が自動で行う操作ではありません。

## アルゴリズム

Duval法では、因子の開始候補i、比較対象j、すでに一致している繰り返し部分を指すkを管理します。
`s[k] < s[j]` なら候補の接頭辞を更新してkをiへ戻し、等しい場合はkを進めます。
`s[k] > s[j]` または末尾に達した時点で `j-k` を因子長として、確定した同じ因子の繰り返しを出力します。

文字の大小が変わる位置までの走査と、確定済み部分をまとめて飛ばす処理を共有することで、比較回数の合計はO(N)になります。因子を実際に切り出さず、区間だけを記録します。

最小巡回シフトではs+sを仮想的に同じ手順で走査し、先頭のN位置にある開始候補を順に確定します。位置tの文字は `s[t%N]` から取得するため、2倍長の文字列を確保しません。
最後に残る開始候補が最小巡回シフトの最小開始位置です。同じ候補の繰り返しはまとめて飛ばすため、周期を持つ文字列でもO(N)で処理できます。

## 確認結果

専用テスト `tests/lyndon_factorization_test.cpp` で、小文字列の全部分割を列挙し、Lyndon語の定義と因子の非増加条件を満たす分解に照合します。
最小巡回シフトはすべての開始位置を直接比較する別解と照合し、同値時の最小位置も検証します。
空・単一文字・同一文字の反復・周期列・埋め込みNUL・上位bit付きbyte・長い入力を確認し、C++23・NDEBUG・AddressSanitizer／UBSanで検証します。

## 関連資料

- [Library Checker — Lyndon Factorization](https://judge.yosupo.jp/problem/lyndon_factorization)：分解の確認用問題です。このAPIは区間列を返すため、必要な境界位置列へ変換して出力します。
- [Duval, Factorizing words over an ordered alphabet](https://doi.org/10.1016/0196-6774(83)90017-2)：線形時間の分解法の原論文です。
