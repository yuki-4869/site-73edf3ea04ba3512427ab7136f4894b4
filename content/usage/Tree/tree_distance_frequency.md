# 木の距離別頂点対数

## API

`tree_distance_frequency(g)` は重みなし無向木の隣接リスト `graph` を受け取り、長さ $N$ の `vector<long long>` を返します。`result[d]` は距離 $d$ の無順序頂点対の数です。同じ頂点を 2 回選ぶ組を除くため、`result[0]` は 0 です。

各辺を両方向に登録します。頂点は 0 始まりで、空入力にも対応します。整数畳み込みの長さ制限に合わせて $N\le 2^{23}$ です。先に [重心分解](centroid_decomposition.md) を貼り、`codecf` では `<atcoder/convolution>` を追加します。

## 計算量

各重心で、全ての深さの頻度の自己畳み込みから、同じ枝に属する組の寄与を引きます。残るのはその重心を通るパスの数です。時間 $O(N\log^2(N+2))$、領域 $O(N\log(N+2))$ です。

## 使用例

```cpp
graph g={{1},{0,2},{1,3},{2}};
auto count=tree_distance_frequency(g);
if(count!=std::vector<long long>{0,3,2,1})std::abort();
```

[Library Checker の対応問題](https://judge.yosupo.jp/problem/frequency_table_of_tree_distance)
