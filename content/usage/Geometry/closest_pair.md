# 平面最近点対（整数座標）

[説明書の目次](../index.md) · [コード](../../../Geometry/closest_pair.cpp)

平面上の点列から、ユークリッド距離が最小となる2点を求めます。
最小距離の二乗と、元の入力における2点の添字を返します。近接する点の探索や、最小距離をしきい値と比較する処理に使えます。

整数を128 bitで計算し、平方根や誤差判定を使いません。
[凸包](convex_hull.md) の外周抽出とは異なり、内部の点も含めて最も近い組を探します。凸包を事前計算する必要はありません。
型エイリアスと貼り付け方は [共通設定](../common.md) を参照してください。追加includeやACL・個別ライブラリへの依存はありません。

## 型・前提

```cpp
using ll = long long;
using LL = __int128;
using pll = pair<ll,ll>;

struct ClosestPairResult {
    LL squared_distance = 0;
    pll indices = {-1,-1};
};
```

- 各点は `pll=(x,y)` で表し、各座標は整数で絶対値が $10^{18}$ 以下です。
- 入力点数NはINT_MAX以下とし、必要なメモリを確保できることが前提です。
- 添字は0-indexedです。順序は任意で、重複座標も入力できます。
- 異なる添字の2点を選びます。同じ座標が2回現れた場合も別の入力点なので、最小二乗距離は0です。
- 最適な組が複数ある場合は1組を返します。添字対の辞書順最小性は保証しません。
- 入力の内容・順序を変更せず、結果に入力への参照も保持しません。
- 座標差を128 bitへ変換してから二乗するため、最小二乗距離がllを超えても扱えます。二乗距離の最大値 $8\times10^{36}$ は符号付き128 bitに収まります。
- 浮動小数点座標、動的な点の追加削除、すべての最適対の列挙には対応しません。
- 点数や座標が範囲外の場合は、NDEBUGでもメッセージなしで停止します。座標検査はNが0または1の場合も行います。

## 構築方法

実装をグローバルに貼り付け、`closest_pair(points)` を呼び出します。
クラスの構築・buildや前処理は不要です。呼び出すたびに独立して計算し直します。

## closest_pair

```cpp
ClosestPairResult closest_pair(const vector<pll>& points);
```

引数pointsの、異なる添字i,j間の距離を最小化し、距離の二乗と元の添字を返します。
返したindicesは `0 <= i < j < N` を満たします。

Nが0または1なら組が存在しないため、`indices={-1,-1}`、`squared_distance=0` を返します。
距離0だけでは組の存在を判定できません。先に `result.indices.first >= 0` を確認してください。

**計算量**：O(N log N)時間、入力を除いてO(N)領域です。Nが0または1なら時間・領域ともO(1)です。
入力をコピーしてソートし、マージ用と帯内走査用の作業領域を使います。分割統治の再帰の深さはO(log N)です。

## ClosestPairResult::indices

```cpp
pll result.indices;
```

最小距離を達成する2点の元の入力添字 `{i,j}` です。ソート後の添字ではありません。
有効な組ではi<jで、`points[i]` と `points[j]` から座標を取得できます。
組が存在しない場合と既定構築時は `{-1,-1}` です。

**計算量**：参照はO(1)時間。

## ClosestPairResult::squared_distance

```cpp
LL result.squared_distance;
```

有効な組i,jについて、次の値を保持します。

$$
(x_i-x_j)^2+(y_i-y_j)^2.
$$

型はllではなく `LL = __int128` です。組が存在しない場合と既定構築時は0です。
通常の `cout << result.squared_distance` は使用できません。128 bit整数用の出力処理を使うか、llの範囲に収まることを確認して変換してください。

**計算量**：参照はO(1)時間。

結果の各フィールドは公開され、入力の寿命に依存しません。
フィールドを書き換えても、ほかのフィールドは自動では更新されません。

## 使用例

実装をグローバルに置き、以下をmain内などで実行します。

```cpp
vector<pll> points={{0,0},{10,10},{3,4},{3,5}};
auto original=points;
auto result=closest_pair(points);
assert((result.indices==pll{2,3}));
assert(result.squared_distance==LL(1));
assert(points==original);

auto [i,j]=result.indices;
if(i>=0){
    LL dx=LL(points[i].first)-points[j].first;
    LL dy=LL(points[i].second)-points[j].second;
    assert(dx*dx+dy*dy==result.squared_distance);
}

auto duplicate=closest_pair(vector<pll>{{7,-2},{0,0},{7,-2}});
assert((duplicate.indices==pll{0,2}) && duplicate.squared_distance==0);
auto missing=closest_pair(vector<pll>{{1,2}});
assert((missing.indices==pll{-1,-1}) && missing.squared_distance==0);
assert((closest_pair(vector<pll>{}).indices==pll{-1,-1}));

const ll B=1000000000000000000LL;
auto wide=closest_pair(vector<pll>{{-B,-B},{B,B}});
assert(wide.squared_distance==LL(8)*B*B);

long double approximate=sqrtl(static_cast<long double>(result.squared_distance));
assert(approximate==1.0L);
```

最後の平方根は実距離が必要な場合の近似です。整数のしきい値Dとの比較なら、制約の範囲で `result.squared_distance <= LL(D)*D` とすれば平方根を避けられます。
この比較はD>=0で、二乗がLLに収まる場合に使用します。

## アルゴリズム

最初にx座標、y座標の順にソートし、隣接する重複座標があれば距離0の組を返します。
以後は点列をx順の中央で半分に分け、左右の中を分割統治で調べます。

左右を処理した後は、それぞれy順になった点列を線形時間でマージします。
現在の最小距離を更新できる組だけを探すため、分割線の近くにある点をy順に走査します。
帯内ではy座標差が現在の最小距離以上となったら比較を打ち切ります。
左右それぞれには現在の最小距離より近い組がないため、1点あたりの比較対象は定数個に抑えられます。

各段階のマージと帯内走査がO(N)、再帰の深さがO(log N)なので、全体はO(N log N)です。
比較はすべて距離の二乗で行い、同値の組では既に見つかった解をそのまま使います。

## 確認結果

専用テスト `tests/closest_pair_test.cpp` で、小規模な点列を全列挙し、全点対を直接比較する別解と照合します。
返した元の添字、二乗距離、入力不変を確認し、ランダム列・重複・同距離の複数解・一直線・分割線をまたぐ最近点対を検証します。
座標の両極値、llを超える二乗距離、50万点規模、不正座標の停止を、C++23・NDEBUG・AddressSanitizer／UBSanで確認します。

## 参考

- [CMU 15-451/651 — Closest Pairs](https://www.cs.cmu.edu/~15451-f21/lectures/lec21-closest-pair.pdf)：分割統治による平面最近点対の講義資料です。
