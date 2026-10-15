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

検証環境はGCC 13.3.0です。一部の実装は、各ページに記載した配置変更や修正を行う必要があります。

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
元コードに穴埋め・宣言順の問題があるページは、例の前に必要な記入・配置変更を説明しています。

## AtCoderでの出典

コピーした実装の固定URLを提出コードにコメントとして記録します。
使用例のコードをコピーする場合は、この説明ページもコンテスト開始前に公開されていることを確認し、その版のURLを記録してください。
詳しくは [公開と参照URLの運用](../publishing.md) を参照してください。
