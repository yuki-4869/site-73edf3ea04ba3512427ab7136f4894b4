# 無向グラフの三角形列挙

## API

`enumerate_triangles(n, edges, visit)` は、各三角形について `visit(u,v,w)` を 1 回呼びます。戻り値は `void` です。件数や重みの総和などはコールバック側で集計できます。

頂点は 0 始まり。単純無向グラフで、各辺を 1 回だけ `vector<pair<int,int>>` に登録してください。渡される三角形の頂点は次数、同次数なら頂点番号の昇順です。入力の辺を列挙中に変更しないでください。

## 計算量

次数と頂点番号で辺を向き付け、各頂点から出る長さ 2 の有向パスと直接の辺を照合します。時間は $O(N+M\sqrt{M+1})$ とコールバックにかかる時間の和で、追加領域は $O(N+M)$ です。

## 使用例

```cpp
int triangles=0;
enumerate_triangles(4,{{0,1},{1,2},{2,0},{2,3}},[&](int,int,int){++triangles;});
if(triangles!=1)std::abort();
```

[Library Checker の対応問題](https://judge.yosupo.jp/problem/enumerate_triangles)
