# 極大周期区間の列挙（Run Enumerate）

最小周期が $t$、長さが $2t$ 以上で、周期を保って左右いずれにも延ばせない区間を列挙します。このような区間を run と呼びます。

## API

```cpp
std::vector<std::tuple<int,int,int>> enumerate_runs(std::string_view s);
```

返り値は `(t,l,r)` の辞書順です。`[l,r)` は 0 始まりの半開区間です。文字列はバイト列として扱います。空文字列にも対応し、長さは `INT_MAX` 未満です。`codecf` では `<atcoder/string>` を追加します。

2 種類の辞書順で Lyndon 語の候補を見つけ、LCP と LCS を使って左右に伸ばします。長さ $N$ に対し、時間と追加メモリはともに $O(N\log(N+1))$ です。

## 使用例

```cpp
auto result=enumerate_runs("ababab");
if(result!=std::vector<std::tuple<int,int,int>>{{2,0,6}})std::abort();
```

[Library Checker・極大周期区間の列挙（Run Enumerate）](https://judge.yosupo.jp/problem/runenumerate)
