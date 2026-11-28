# 部分集合畳み込み（Subset Convolution）

各集合 $S$ を互いに素な $A$ と $S\setminus A$ に分けたときの寄与を、全ての $S$ について求めます。

$$
h[S]=\sum_{A\subseteq S} f[A]g[S\setminus A].
$$

## API

```cpp
template<class T>
std::vector<T> subset_convolution(const std::vector<T>& f, const std::vector<T>& g);
```

`f` と `g` の長さはともに $2^N$ です。空集合は添字 0。値型 `T` は 0 から構築でき、加算・減算・乗算ができる型を渡します。ACL の `modint` を使えます。整数型ではメビウス変換を含めた中間値が表現範囲に収まるようにしてください。

要素数別に部分集合ゼータ変換し、各マスクで次数方向に積を取ります。メビウス変換した結果から、マスクの要素数と次数が等しい項を取り出すと、重複しない組だけが残ります。時間 $O((N+1)^2 2^N)$、追加メモリ $O((N+1)2^N)$ です。

## 使用例

```cpp
std::vector<mint> f={1,2,3,4},g={5,6,7,8};
auto h=subset_convolution(f,g);
if(h[3]!=60)std::abort();
```

[Library Checker・部分集合畳み込み（Subset Convolution）](https://judge.yosupo.jp/problem/subset_convolution)
