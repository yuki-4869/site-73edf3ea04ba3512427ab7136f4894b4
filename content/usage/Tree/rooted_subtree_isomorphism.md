# 根付き部分木の同型分類

## API

`rooted_subtree_ids(parent)` は `vector<int>` の親配列を受け取り、各頂点を根とする部分木の ID を `vector<int>` で返します。頂点は 0 始まりで、根の親を `-1` とします。木・森・空入力に対応します。親番号と子番号の大小関係に制約はありません。

子の順序を区別せず、同じ呼び出し内で、同型な部分木に同じ ID を付けます。ID は 0 からの連番です。同じ木でも頂点番号や入力順が変われば ID の番号自体は変わり得ます。

## 計算量

子の ID 列をソートして登録し、葉から順に分類します。時間 $O(N\log(N+2))$、領域 $O(N)$ です。

## 使用例

```cpp
auto ids=rooted_subtree_ids({-1,0,0,1,2});
if(ids[1]!=ids[2] || ids[3]!=ids[4] || ids[0]==ids[1])std::abort();
```

[Library Checker の対応問題](https://judge.yosupo.jp/problem/rooted_tree_isomorphism_classification)
