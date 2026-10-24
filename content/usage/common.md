# 共通設定

[使い方の目次](index.md)

## コードの置き方

このリポジトリは貼り付け用スニペット集です。基本テンプレートの型エイリアス・マクロを先に定義し、必要なクラス・関数をグローバルに、呼び出し例をmain等の関数内に置きます。
同名クラスの別版を同時に貼り付けないでください。
各ページの使用例はmain内の処理部分です。関数・クラス本体はリンク先のコードを使います。

## 最低限の設定の例

```cpp
#include <bits/stdc++.h>
#include <atcoder/all> // ACLを利用するコードの場合
using namespace std;
using namespace atcoder;
using ll = long long;
using mint = modint998244353;
using vl = vector<ll>;
using vb = vector<bool>;
using vvl = vector<vl>;
using vmi = vector<mint>;
using pll = pair<ll,ll>;
using vp = vector<pll>;
using graph = vector<vector<ll>>;
using Graph = vector<vector<pair<ll,ll>>>;
#define rep(i,n) for(ll i=0;i<(ll)(n);i++)
#define rep1(i,n) for(ll i=1;i<=(ll)(n);i++)
#define pb push_back
template<class T> ll sz(const T& x){return (ll)x.size();}
```

これはすべての依存関係を満たす万能ヘッダではありません。nCr・定数inf・補助関数などを参照する実装は基本テンプレートの対応する定義も用意してください。
今回の検証ではGNU C++23を使用し、ACLのatcoderディレクトリをincludeパスに置きます。
```sh
g++ -std=c++23 -O2 -I/path/to/ac-library main.cpp
```

検証環境はGCC 13.3.0です。基本テンプレートと貼り付け用の実装を組み合わせて確認しています。穴埋め用の雛形は各ページの記入条件に従ってください。

## 添字・区間

- 頂点や配列位置は、特記がなければ0-indexedです。
- `[l,r)` はlを含みrを含まない半開区間で、長さはr-lです。
- Li Chao Treeの座標区間やSortedImplicitTreapの値域 `[L,R]` は両端を含みます。
- 「k番目」は0-indexed。「k個の和」は個数であり、k=0を含む場合があります。
- optionalのnullopt、頂点番号の-1、単位元など、値が存在しない場合の返り値は各APIを参照してください。

## 計算量

Nは要素・頂点数、Mは辺数、sigmaは異なる値数など、各ページで定義します。
「ならし・償却」は操作列全体での評価、「期待」は乱数による平均の評価です。いずれも1回ごとの最悪時間と同じ意味ではありません。
ユーザー定義の演算を使う型では、その演算がO(1)である場合の計算量を示します。
メモリ量は主に管理する情報量を示し、標準コンテナ・アロケータ等の定数倍は含めません。

## 例と確認範囲

使用例のassertは説明した具体例の結果を確認するためのものです。全入力の正当性や、全APIの組み合わせの保証ではありません。
穴埋め用の雛形は、例の前に必要な記入・配置を説明しています。

## AtCoderでの出典

コピーした実装の固定URLを提出コードにコメントとして記録します。
使用例のコードをコピーする場合は、この説明ページもコンテスト開始前に公開されていることを確認し、その版のURLを記録してください。
詳しくは [公開と参照URLの運用](../publishing.md) を参照してください。

## コードと使用例の統一方針

貼り付け用ライブラリの実装本体には説明コメント・Sourceコメント・includeを書かず、末尾の1つのコメントに「細かい注意点 → 使える関数一覧 → 基本の関数」の順でまとめています。基本テンプレート自身のincludeは残しています。`code`・`codet`・`codecf` の先頭には、対応するGitHubコードページへのリンクを必ず1行のコメントで記載します。旧版にあった共通補助関数などの短い説明コメントは行末に残します。`codecf` ではコメント済みのACLヘッダー候補とexpanderのコマンド案内も残します。
検出した前提違反では `std::abort()` により、ライブラリから標準出力・標準エラーへメッセージを出さず停止します。例外で回復するAPIではありません。メモリ不足や利用者定義の演算など、外部の例外までは置き換えません。

### 基本テンプレートに追加が必要なもの

`code` と `codet` は `<bits/stdc++.h>` と `<atcoder/all>` で必要なヘッダーを満たします。`codecf` はACL modintのみなので、以下が必要です。

| 使用するライブラリ | codecfへの追加 |
| --- | --- |
| セグ木ラッパー・汎用セグ木雛形 | `#include <atcoder/segtree>` |
| 遅延セグ木ラッパー・汎用遅延セグ木雛形 | `#include <atcoder/lazysegtree>` |

汎用遅延セグ木雛形は、別途 `LazySegBase`（遅延セグ木ラッパーに定義）も先に配置します。基本テンプレートの `Graph` は全て重み付き隣接リストに揃えています。

## 基本テンプレート内の最短距離

`dijkstra(g,s)` または `dijkstra(g,vl{s1,s2})` で、各頂点までの最短距離を `vl` で取得できます。`graph` は各辺を距離1、`Graph` は `(行き先, 非負重み)` として扱います。`dist[v]` が頂点 `v` の距離、未到達は `inf` です。返り値の長さは `g.size()` です。

全オーバーロード・制約・計算量は [基本テンプレートのdijkstra](template.md#dijkstra) を参照してください。

## 2の累乗の整数表

`Pow2[i]` で `2^i`（`0<=i<=60`）を取得します。`code`（`template.cpp`）の95行目に61項を定数として展開済みで、型は `const vl` です。詳しくは [Pow2の説明](template.md#pow2-table) を参照してください。

## テンプレート間の順序と行番号

`code` を共通関数・定数・型定義の順序の基準にします。`codet`・`codecf` も同じ順序に揃えます。行数指定は `code` に対して適用し、他の2種類ではヘッダー候補やコメントによる行数のずれを許容して順序を優先します。

`codecf` の展開コマンド案内は `// python /home/yuki4869/expander.py    .cpp` の1行コメントで、`void solve();` の前に配置します。3種類とも記入用の `main` / `solve` は閉じず、最終行のTab1つを維持します。

## 型に応じた1行出力

`p(x)` は `x` が `long double`（`ld`）のとき小数点以下12桁の固定小数点形式で出力します。例：`p(1.25L)` は `1.250000000000`。式は1回だけ評価し、出力後は元の `cout` の書式と精度を復元します。他の型は従来の出力形式です。詳しくは [pの説明](template.md#print-line) を参照してください。
