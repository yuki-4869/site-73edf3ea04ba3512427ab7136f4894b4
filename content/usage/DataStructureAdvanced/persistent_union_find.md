# 完全永続 Union-Find

全頂点が孤立した版から始め、任意の版で 2 頂点を連結して新しい版を作ります。版を値として保存しておけば、後からその版の連結性を確認できます。

## API

頂点は 0 始まりです。`PersistentUnionFind uf(n)` で管理オブジェクトを作り、`PersistentUnionFind::Version{}` を初期版として使います。初期版と同じ `uf` から得た版を渡してください。

| 操作 | 返り値 |
| --- | --- |
| `uf.merge(v, a, b)` | `Version`。版 `v` の `a,b` を連結した版 |
| `uf.leader(v, a)` | `int`。版 `v` での `a` の代表頂点 |
| `uf.same(v, a, b)` | `bool`。版 `v` で両頂点が連結か |
| `uf.size(v, a)` | `int`。版 `v` での `a` の連結成分のサイズ |

## 計算量

初期化と版のコピーは $O(1)$。成分サイズで併合し、親を永続配列で管理するので各操作は $O(\log^2(N+2))$ 時間。実際に併合した回数を $U$ とすると使用領域は $O(1+U\log(N+2))$ です。

## 使用例

```cpp
PersistentUnionFind uf(3);
PersistentUnionFind::Version empty;
auto a=uf.merge(empty,0,1), b=uf.merge(empty,1,2);
if(!uf.same(a,0,1) || uf.same(a,0,2) || uf.size(b,1)!=2 || uf.same(empty,0,1))std::abort();
```

[Library Checker の対応問題](https://judge.yosupo.jp/problem/persistent_unionfind)
