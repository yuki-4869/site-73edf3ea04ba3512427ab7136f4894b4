# 一般グラフ最大マッチング（Blossom法）

[説明書の目次](../index.md) · [コード](../../../Graph/general_matching.cpp)

無向グラフから、**どの頂点も高々1回だけ使うように選べる辺数の最大値**を求めます。
許可された組の中でできるだけ多くのペアを作る問題に使えます。三角形などの奇数閉路を含んでも構いません。
最大の辺数に加え、各頂点の対応相手と、採用した元の辺番号を返します。

二部グラフなら [Hopcroft–Karp](hopcroft_karp.md) も利用できます。
本実装は左右の頂点集合への分割を必要とせず、EdmondsのBlossom法で一般グラフを扱います。
重みを考慮した最適化は行いません。辺の重みを無視して本関数を呼んでも、最大重みのマッチングにはなりません。

型エイリアスと貼り付け方は [共通設定](../common.md) を参照してください。
基本テンプレートの `ll`・`vl`・`vp` を使い、追加のinclude、ACL、他の個別ライブラリへの依存はありません。

## 型・前提

```cpp
using ll = long long;
using vl = vector<ll>;
using vp = vector<pair<ll, ll>>;

struct GeneralMatchingResult {
    ll cardinality = 0;
    vl mate, edge_ids;
};
```

- Nを頂点数、Mを `edges.size()` とし、`0 <= N <= INT_MAX`、`M <= LLONG_MAX` とします。
- 各辺は `edges[id]={u,v}` で表し、`0 <= u,v < N` が必要です。無向辺なので `{u,v}` と `{v,u}` は同じ頂点対を結びます。
- 頂点番号は `0..N-1`、元の辺番号は `0..M-1` の0-indexedです。
- 孤立点、連結でないグラフ、多重辺、向きだけが逆の重複、自己ループにも対応します。
- 自己ループはマッチングに採用できないため無視します。同じ頂点対の辺を重ねても、その2頂点を何度も使えるようにはなりません。
- N=0では辺も空である必要があります。N>0・辺0本の場合も有効です。
- 最大マッチングが複数あるときは、そのうち1つを返します。選ぶ頂点対は入力順にも依存し、辞書順最小などの優先順位は保証しません。
- 重み付きマッチング、最小頂点被覆、辺の逐次追加・削除に対する差分更新は提供しません。

必要な時間とメモリを確保できることが前提です。頂点数の型の上限まで実用的に計算できるという意味ではありません。
主に数百頂点規模の一般グラフを想定しています。

## 構築方法

実装をグローバルに置き、頂点数Nと辺リストedgesを `general_matching(N,edges)` へ渡します。
クラスの構築やbuild操作は不要です。
呼び出すたびに空の対応から計算し直し、過去の結果やグラフを内部に保持しません。
返り値は入力と独立しているため、入力配列を後から変更・破棄しても結果は変わりません。

## general_matching

```cpp
GeneralMatchingResult general_matching(ll n, const vp& edges);
```

引数n個の頂点と、edgesに含まれる無向辺からなるグラフの最大マッチングを求めます。
入力配列は変更しません。返す各フィールドの意味は以下を参照してください。

入力検査では、負のn、INT_MAXを超えるn、辺番号をllで表せない長さ、範囲外の端点を検出すると、NDEBUGでもメッセージなしで停止します。
自己ループや多重辺は不正入力ではありません。

**計算量**：O(N³+M)時間、入力を除いて返り値も含めO(N+M)領域です。N=M=0はO(1)です。

重複した頂点対は探索前にO(N+M)時間で除きます。
その後、各頂点から高々1回の増加路探索を行い、探索1回が最悪O(N²)です。
入力に大量の多重辺があっても、全ての重複を各探索で走査することはありません。
縮約・探索・増加路の復元のいずれも関数再帰を使いません。

## GeneralMatchingResult::cardinality

```cpp
ll result.cardinality;
```

最大マッチングの辺数Kを保持します。`0 <= K <= N/2` で、`edge_ids.size()` と等しくなります。
**使った頂点数ではなくペア数**です。使った頂点数は2Kです。
取得はO(1)時間です。

全頂点をペアにできるかは `2*result.cardinality == N` で判定できます。
Nが奇数なら成立せず、N=0では成立します。
単独で既定構築した `GeneralMatchingResult` のcardinalityは0です。

## GeneralMatchingResult::mate

```cpp
vl result.mate;
```

長さNの配列で、`mate[v]` は頂点vの対応相手の頂点番号です。未対応なら-1です。
対応している場合は、`mate[v] != v` と `mate[mate[v]] == v` が成り立ち、入力にその2頂点を結ぶ辺が存在します。
孤立点や自己ループだけを持つ頂点は-1になります。

1要素の取得はO(1)、全走査・コピーはO(N)時間です。
既定構築した結果や、N=0の返り値では空です。

## GeneralMatchingResult::edge_ids

```cpp
vl result.edge_ids;
```

長さKの配列で、採用した**元の入力辺番号**を昇順に保持します。
`edges[edge_ids[i]]` により、選んだ辺の両端を取得できます。
端点の向きは元の入力どおりで、常に小さい頂点が先になるという意味ではありません。

同じ採用頂点対を結ぶ入力辺が複数ある場合は、その中で最小のidを返します。
ただし、採用する頂点対自体の選び方や、辺番号列全体の辞書順最小性は保証しません。
自己ループは含まれず、列全体でどの頂点も高々1回しか現れません。

1要素の取得はO(1)、全走査・コピーはO(K)時間です。
既定構築した結果や、対応数0の返り値では空です。

全フィールドは公開されています。書き換えた後のcardinality・mate・edge_idsの整合性を自動で修復するものではありません。
通常の結果のコピーはO(N+K)時間・領域で独立した複製を作ります。

## 使用例

実装をグローバルに置き、以下をmain内などで実行します。

```cpp
vp edges={{0,1},{1,2},{2,0},{2,3}}; // 三角形と、頂点2から伸びる辺
auto original=edges;
auto result=general_matching(4,edges);
assert(result.cardinality==2);
assert((result.mate==vl{1,0,3,2}));
assert((result.edge_ids==vl{0,3}));
assert(edges==original);
assert(2*result.cardinality==4);  // 全頂点をペアにできた
for(ll id:result.edge_ids){
    auto [u,v]=edges[(size_t)id];
    assert(result.mate[(size_t)u]==v && result.mate[(size_t)v]==u);
}

vp duplicate={{0,0},{1,0},{0,1},{1,0}};
auto parallel=general_matching(3,duplicate);
assert(parallel.cardinality==1);
assert((parallel.edge_ids==vl{1})); // 自己ループは除外、頂点対{0,1}の最初の辺
assert(parallel.mate[2]==-1);     // 孤立点

auto odd=general_matching(3,vp{{0,1},{1,2},{2,0}});
assert(odd.cardinality==1);       // 三角形では1ペアが最大
auto isolated=general_matching(4,vp{});
assert(isolated.cardinality==0 && isolated.mate==vl(4,-1));
auto empty=general_matching(0,vp{});
assert(empty.cardinality==0 && empty.mate.empty() && empty.edge_ids.empty());
```

1-indexedの問題を解く場合は入力時に両端を1ずつ減らし、本関数を呼びます。
出力で頂点番号を戻す場合も、未対応を示す-1と実際の頂点番号を区別してください。
辺番号を出力する問題では、問題の指定に応じて `id+1` とします。

## アルゴリズム

選んでいない辺と選んだ辺を交互に通り、未対応の頂点どうしを結ぶ道を**増加路**と呼びます。
道上の辺の採否を入れ替えると、対応数を1増やせます。増加路が存在しないマッチングは最大です。

未対応の頂点から交互木を幅優先に伸ばして増加路を探します。
一般グラフでは、木の偶数深さ側にある2頂点が辺でつながると、交互な辺からなる奇数閉路が現れます。
この閉路を1つの頂点として扱う**blossomの縮約**により、木の探索を続けられます。

縮約の基点を記録し、閉路に入る側に合わせて親の情報を付け替えることで、入れ子の縮約にも対応します。
未対応の別の頂点に到達したら、親をたどって元の頂点間の対応を更新します。
内部の縮約番号は返さず、最終結果には元の頂点番号と辺番号だけを返します。

## 確認結果

専用テスト `tests/general_matching_test.cpp` で、0〜6頂点の全33,868個の単純グラフを、頂点部分集合DPによる全探索と比較します。
ランダムな小規模グラフも同じ別解で照合し、対応数、元の辺の存在、対応の対称性、端点を重ねないこと、最小の元辺番号の復元、入力不変を確認します。

さらに、二部グラフをHopcroft–Karp、より大きい一般グラフを独立したTutte行列の階数計算と照合します。
Tutte行列の照合は有限体上の乱数代入による確率的な検証で、各グラフで独立に3回行います。本体のBlossom法は乱数を使いません。
Petersenグラフの頂点番号変更、共有頂点を持つ奇数閉路の連鎖、601頂点の完全グラフ、2000頂点の長い増加路と多数のblossom、20万本の多重辺、20万孤立点も確認します。
C++23の通常・NDEBUG・AddressSanitizer／UBSanで実行します。

## 参考

- [Edmonds, Paths, Trees, and Flowers](https://doi.org/10.4153/CJM-1965-045-4)：一般グラフ最大マッチングとblossomの原論文です。
- [Library Checker — General Matching](https://judge.yosupo.jp/problem/general_matching)：単純無向グラフで最大マッチングを復元する検証問題です。オンライン提出の実施を示すものではありません。
