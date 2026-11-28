# 最大独立集合（半分全列挙）

辺で結ばれた頂点の組を含まない集合のうち、頂点数が最大のものを1つ求めます。

## API

```cpp
std::vector<int> maximum_independent_set(int n, const std::vector<std::pair<int,int>>& edges);
```

頂点数は $0\le N\le40$、各端点は $[0,N)$ です。無向辺を片方向だけ登録すれば使えます。自己ループのある頂点は除外し、多重辺も扱います。返り値は選んだ頂点を昇順に並べたものです。

頂点を二分し、右半分の全てのマスクに対して、その中の最適な独立集合を DP で求めます。左半分の独立集合ごとに、隣接する頂点を右半分の候補から取り除いて DP を引きます。時間は $O(M+2^{\lceil N/2\rceil})$、追加メモリは $O(N+2^{\lceil N/2\rceil})$ です。

## 使用例

```cpp
auto selected=maximum_independent_set(4,{{0,1},{1,2},{2,3}});
if(selected.size()!=2)std::abort();
```

[Library Checker・最大独立集合（半分全列挙）](https://judge.yosupo.jp/problem/maximum_independent_set)
