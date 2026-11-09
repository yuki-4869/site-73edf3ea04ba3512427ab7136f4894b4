# 重心分解

[説明書の目次](../index.md) · [コード](../../../Tree/centroid_decomposition.cpp)

木から重心を取り除いて残りの各成分を分解し、元の各頂点を重心木の頂点として並べ直します。
各成分の頂点数を半分以下に分けるため、重心木の深さはO(log N)です。
各頂点について、祖先重心と元の木での距離も前計算します。

頂点を順に有効にし、最も近い有効頂点までの距離を求める問題などに使えます。
[木の重心](tree_centroid.md) は重心を1回求める機能で、こちらは成分ごとに繰り返し分解し、重心木と距離の一覧を構築する別の機能です。
型エイリアスと貼り付け方は [共通設定](../common.md) を参照してください。基本テンプレート以外のincludeや別ライブラリは不要です。

## 型・前提

```cpp
using ll = long long;
using vl = vector<ll>;
using pll = pair<ll,ll>;
using graph = vector<vl>;
```

- 入力gは、頂点番号 `0..N-1` の重みなし無向木です。NはINT_MAX以下とします。
- 各辺u-vは `g[u]` にv、`g[v]` にuをそれぞれ1回入れます。自己辺・多重辺・サイクル・非連結な入力は使えません。
- 空の木と1頂点の木にも対応します。
- 距離は元の木における辺数で、llを使います。辺重み・頂点重み・森には対応しません。
- 元の隣接リストは変更せず、構築後に参照も保持しません。入力の寿命や後の変更に依存しません。
- 重心が2個ある成分では、どちらを選ぶかは保証しません。隣接リストの順序によって有効な別の分解になる場合があります。
- 各段階で、頂点数Kの成分から重心を除いた後の各成分は高々floor(K/2)頂点です。
- 辺の追加削除に追従する機能はありません。木を変更した場合はbuildし直します。
- メモリを確保できることが前提です。再帰は使わないため、パス状の木でも再帰スタックを消費しません。

不正な隣接リスト、Nの超過、取得APIに対する範囲外の頂点番号は、NDEBUGでもメッセージなしで停止します。
隣接リストの対称性、辺数、連結性を確認してから分解します。

## 構築方法

```cpp
CentroidDecomposition cd;
CentroidDecomposition cd(g);
```

既定構築では頂点数0の空状態です。gを渡す構築では、その場で分解と距離の前計算を行います。
非空なら重心木の根は1個で、元の全頂点が重心木にも1回ずつ現れます。

**計算量**：既定構築はO(1)。gを渡す構築はO(N log N)時間・領域で、Nが0または1ならO(1)です。
元の入力の領域はO(N)です。分解の作業領域はO(N)、結果の距離一覧を含む保持領域はO(N log N)です。

## build

```cpp
void cd.build(const graph& g);
```

引数gの木で分解結果を置き換えます。既定構築後の初回構築にも使用できます。返り値はありません。
以前取得したancestorsの参照・イテレータは無効になり、呼び出し側で作ったbestなどの集計も作り直す必要があります。

**計算量**：新しい木に対してO(N log N)時間・領域です。Nが0または1なら新規構築分はO(1)です。
以前の保持領域の大きさをS_oldとすると、その破棄に別途O(S_old)時間がかかります。
構築中の最大使用領域は旧状態も含めO(S_old+N log N)です。小さい木へ再構築した後は、旧状態の容量を保持しません。

## size

```cpp
ll cd.size() const;
```

元の木の頂点数Nを返します。重心木の頂点数もNです。

**計算量**：O(1)時間。

## parent

```cpp
ll cd.parent(ll v) const;
```

頂点vの**重心木での親**を返します。重心木の根では-1です。
引数は `0 <= v < N` とします。元の木の親を返す関数ではありません。

**計算量**：O(1)時間。

## depth

```cpp
ll cd.depth(ll v) const;
```

頂点vの重心木での深さを返します。根は0で、非空の木なら高々floor(log2 N)です。
引数は `0 <= v < N` とします。元の木における根からの距離ではありません。

**計算量**：O(1)時間。

## ancestors

```cpp
const vector<pll>& cd.ancestors(ll v) const;
```

引数vについて、`{祖先重心c, 元の木のdist(v,c)}` の一覧をconst参照で返します。
引数は `0 <= v < N` とします。重心木の根からvへ向かう順で、最後は必ず `{v,0}` です。
長さは `depth(v)+1` で、非空なら高々floor(log2 N)+1です。

**計算量**：参照の取得はO(1)時間・領域、全走査またはコピーはO(log N)時間です。
コピーの領域もO(log N)です。1頂点の木ではO(1)です。

返した参照は、同じオブジェクトのbuild・代入・移動・破棄の後は使わないでください。
長く保存したい場合は `auto path=cd.ancestors(v);` としてvectorをコピーします。
`const auto& path=cd.ancestors(v);` はコピーせず内部のvectorを参照します。

## 使用例

実装をグローバルに貼り付け、以下をmain内などで実行します。
追加済み頂点がないときは-1を返し、頂点の追加と最近傍距離の取得をそれぞれO(log N)で行う例です。

```cpp
graph g={{1},{0,2,3},{1},{1,4},{3}};
CentroidDecomposition cd(g);
assert(cd.size()==5);
assert(cd.parent(1)==-1 && cd.depth(1)==0);
assert((cd.ancestors(4).front()==pll{1,2}));
assert((cd.ancestors(4).back()==pll{4,0}));

vl best(cd.size(),inf);
auto mark=[&](ll v){
    for(auto [c,d]:cd.ancestors(v))best[c]=min(best[c],d);
};
auto nearest=[&](ll v){
    ll answer=inf;
    for(auto [c,d]:cd.ancestors(v)){
        if(best[c]!=inf)answer=min(answer,best[c]+d);
    }
    return answer==inf?-1:answer;
};
assert(nearest(2)==-1);
mark(0);
assert(nearest(4)==3);
mark(4);
assert(nearest(2)==2 && nearest(3)==1 && nearest(4)==0);
mark(4);
assert(nearest(2)==2);

auto saved=cd.ancestors(4);
cd.build(graph{{}});
assert(cd.size()==1 && cd.parent(0)==-1 && cd.depth(0)==0);
assert((cd.ancestors(0)==vector<pll>{{0,0}}));
assert((saved.back()==pll{4,0}));
cd.build(graph{});
assert(cd.size()==0);
```

best[c]は、cを祖先に持つ追加済み頂点uについてのdist(u,c)の最小値です。
問い合わせvと追加済み頂点uの重心木上の最小共通祖先cは、元の木のu-vパス上にあります。
そのcを経由する候補は真の距離になり、他の候補も三角不等式で真の距離を下回りません。そのため最小値が答えになります。
追加は何度行っても構いません。このbestだけの実装は、頂点の削除には対応しません。

## アルゴリズム

処理対象の成分を明示的なスタックへ積みます。成分を走査して部分木サイズを計算し、
どの方向へ進んでも残る頂点数が成分の半分以下となる重心を選びます。
次に重心から成分内を走査し、各頂点へ重心と距離の組を記録します。
重心を使用済みにして、その隣接成分を次の処理対象へ追加します。

各頂点が属する成分は、処理を1段深くするたびに半分以下になります。
よって各頂点が走査される回数と祖先重心の個数はO(log N)で、全体の時間・保持領域はO(N log N)です。
親・深さだけを保持する実装より領域を使う代わりに、問い合わせ時のLCAや距離計算を不要にしています。

## 確認結果

専用テスト `tests/centroid_decomposition_test.cpp` で、Prüfer列から生成した小規模の全ラベル付き木を検証します。
各重心の成分分割が半分以下となること、重心木の親・深さ・祖先一覧、全点対BFSとの距離一致を確認します。
ランダム木では最近傍の追加・問い合わせを全有効頂点の直接走査と照合します。
空・1頂点・2重心・隣接順の変更・再構築・不正入力の停止・20万頂点のパスと星を、C++23・NDEBUG・AddressSanitizer／UBSanで検証します。

## 参考

- [Hong Kong Olympiad in Informatics — Graph (V)](https://assets.hkoi.org/training2024/g-v.pdf)：重心分解の性質と構築を扱う講義資料です。
