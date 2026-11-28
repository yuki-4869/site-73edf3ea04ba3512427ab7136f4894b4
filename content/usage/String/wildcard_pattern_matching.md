# ワイルドカード付き文字列照合

## API

`wildcard_pattern_matching(text, pattern)` は `string_view` を 2 つ受け取り、開始位置ごとの一致を `vector<bool>` で返します。添字は 0 始まり。文字は英小文字と `*` で、どちら側の `*` も任意の 1 文字に一致します。

空パターンは全境界に一致します。テキストより長いパターンでは空配列を返します。それ以外では長さを $N,M$ として $N+M-1\le2^{23}$ かつ $625M<998244353$ が必要です。`codecf` では `<atcoder/convolution>` を追加します。

## 計算量

ワイルドカードのない位置だけで文字番号の差の二乗を足し、総和が 0 なら一致です。総和を 3 回の畳み込みで求めます。上の長さ制限では総和が法より小さいため、同じ剰余による誤判定はありません。時間 $O((N+M)\log(N+M+2))$、領域 $O(N+M)$ です。

## 使用例

```cpp
auto match=wildcard_pattern_matching("a*ca","a*");
if(match!=std::vector<bool>{true,true,false})std::abort();
```

[Library Checker の対応問題](https://judge.yosupo.jp/problem/wildcard_pattern_matching)
