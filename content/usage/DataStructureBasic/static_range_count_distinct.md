# 静的区間の異なる値の個数

## API

`static_range_count_distinct(values, queries)` は、`vector<T>` と、`vector<pair<int,int>>` のクエリ列を受け取り、入力順の答えを `vector<int>` で返します。

クエリ `{l,r}` は 0 始まりの半開区間です。空区間の答えは 0。比較と等値判定が整合する型 `T` を使います。静的な配列と全クエリを一度に渡すオフライン処理です。`codecf` では `<atcoder/fenwicktree>` を追加します。

## 計算量

右端を昇順に進め、各値の最新位置にだけ 1 を置いた Fenwick 木の区間和を取ります。時間 $O((N+Q)\log(N+2))$、領域 $O(N+Q)$ です。

## 使用例

```cpp
auto count=static_range_count_distinct(std::vector<int>{1,2,1,3},{{0,3},{1,4},{2,2}});
if(count!=std::vector<int>{2,3,0})std::abort();
```

[Library Checker の対応問題](https://judge.yosupo.jp/problem/static_range_count_distinct)
