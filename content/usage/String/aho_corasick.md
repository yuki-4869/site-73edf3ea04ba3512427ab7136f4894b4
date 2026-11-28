# Aho-Corasick

[説明書の目次](../index.md) · [コード](../../../String/aho_corasick.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/String/aho_corasick.cpp)

登録した複数パターンの出現回数を、failure link付きのオートマトンで数えます。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

V=作成ノード数、P=パターン数、L=走査テキストの長さ。重複登録・重なり合う出現も別々に数えます。

**共通の前提**

- SIGMA>=1、文字はBASE..BASE+SIGMA-1。空パターンは登録不可。全パターンをaddした後にbuildを1回呼び、以後追加しません。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [AhoCorasick::char_id](#api-2)
- [AhoCorasick::add](#api-3)
- [AhoCorasick::build](#api-4)
- [AhoCorasick::transition](#api-5)
- [AhoCorasick::count_all](#api-6)
- [AhoCorasick::count_each](#api-7)
- [AhoCorasick::contains](#api-8)
- [AhoCorasick::size](#api-9)
- [AhoCorasick::pattern_count](#api-10)
- [公開メンバ（terminal_node・bfs_order・nodes）](#public-members)

</details>

## 型・テンプレート引数

```cpp
template<int SIGMA = 26, char BASE = 'a'>
struct AhoCorasick;
```

<a id="api-1"></a>

## コンストラクタ

```cpp
AhoCorasick();
```

空の状態で構築します。

**計算量**

- O(SIGMA)。

<a id="api-2"></a>

## AhoCorasick::char_id

```cpp
int char_id(char c) const;
```

文字cを0..SIGMA-1に変換します。

**計算量**

- O(1)。

<a id="api-3"></a>

## AhoCorasick::add

```cpp
int add(const string& s);
```

パターン `s` を追加し、登録順の 0 始まりの ID を返します。`ac.terminal_node[id]` で、このパターンの終端ノード番号を取得できます。[公開メンバ](#public-members)

**計算量**

- O(|s|)。

<a id="api-4"></a>

## AhoCorasick::build

```cpp
void build();
```

failure link と遷移を構築

**計算量**

- O(ノード数 * SIGMA)。

<a id="api-5"></a>

## AhoCorasick::transition

```cpp
int transition(int state, char c) const;
```

文字 c を1文字読んだ後の状態

**計算量**

- O(1)。

<a id="api-6"></a>

## AhoCorasick::count_all

```cpp
long long count_all(const string& s) const;
```

text 中の全パターンの出現数の合計 重複パターンも別々に数える オーバーラップする出現も数える

**計算量**

- O(|text|)。

<a id="api-7"></a>

## AhoCorasick::count_each

```cpp
vector<long long> count_each( const string& s ) const;
```

各登録パターンの出現回数を登録ID順のvector<long long>で返します。重複登録も別々のIDで返します。

**計算量**

- O(|text|+V+P)。

<a id="api-8"></a>

## AhoCorasick::contains

```cpp
bool contains(const string& s) const;
```

登録パターンの少なくとも1つがtextに出現するかを返します。

**計算量**

- O(L)。

<a id="api-9"></a>

## AhoCorasick::size

```cpp
int size() const;
```

オートマトンのノード数を返します。

**計算量**

- O(1)。

<a id="api-10"></a>

## AhoCorasick::pattern_count

```cpp
int pattern_count() const;
```

登録パターンの個数（重複込み）を返します。

**計算量**

- O(1)。

<a id="public-members"></a>

## 公開メンバ（terminal_node・bfs_order・nodes）

オートマトンを DP に使うときは、以下のメンバを参照できます。登録 ID とノード番号はいずれも 0 始まりで、根のノード番号は `0` です。

| 書き方 | 取得できる情報 | 参照できる時点 |
| --- | --- | --- |
| `ac.terminal_node[id]` | `add()` が返した登録 ID に対応する終端ノード番号 | そのパターンの `add()` 後 |
| `ac.bfs_order` | 根から深さの昇順に並んだノード番号の列 | `build()` 後 |
| `ac.nodes[v].fail` | ノード `v` の failure link の行き先 | `build()` 後 |
| `ac.nodes[v].terminal_count` | ノード `v` を終端とする登録パターンの個数 | 登録時から |
| `ac.nodes[v].match_count` | ノード `v` にいるとき、末尾に一致する登録パターンの個数 | `build()` 後 |
| `ac.nodes[v].next[k]` | 文字番号 `k` を読んだ後の遷移先。`k` は `ac.char_id(c)` で求める | `build()` 後 |

添字による参照はいずれも `O(1)` です。`terminal_node` の長さは `ac.pattern_count()`、`build()` 後の `bfs_order` の長さは `ac.size()` です。重複登録では ID ごとに `terminal_node` の要素があり、ノード番号は同じになります。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
AhoCorasick<> ac; auto a=ac.add("aba"); auto b=ac.add("ba"); ac.build();
auto cnt=ac.count_each("ababa");
assert(cnt[a]==2 && cnt[b]==2 && ac.count_all("ababa")==4);
```

### 状態ごとに末尾で一致するパターンをビット集合にする

`terminal_node` で各パターンの終端に印を付け、BFS 順に failure link の先から印を引き継ぎます。`ab` を読んだ直後には `ab` と `b` の両方が末尾に一致します。この例の `unsigned` に収める場合、パターン数は `std::numeric_limits<unsigned>::digits` 以下にします。[応用問題・ABC419 F](https://atcoder.jp/contests/abc419/tasks/abc419_f)

```cpp
AhoCorasick<> ac;
int ab = ac.add("ab");
int b = ac.add("b");
ac.build();

vector<unsigned> hit(ac.size());
for (int id = 0; id < ac.pattern_count(); ++id) {
    hit[ac.terminal_node[id]] |= 1u << id;
}
for (int v : ac.bfs_order) {
    if (v != 0) hit[v] |= hit[ac.nodes[v].fail];
}

int state = ac.transition(ac.transition(0, 'a'), 'b');
assert(state == ac.terminal_node[ab]);
assert(hit[state] == ((1u << ab) | (1u << b)));
```

`build()` 後に、このビット集合を `O(V+P)` 時間・`O(V)` 追加メモリで作れます。

## メモリ

O(V×SIGMA+P)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
