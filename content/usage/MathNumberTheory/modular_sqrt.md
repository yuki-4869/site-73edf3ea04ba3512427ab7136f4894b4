# 素数 mod の平方根

## API

`modular_sqrt(y, prime)` は、平方して $y$ と合同になる整数を $[0,\mathrm{prime})$ から 1 つ、`long long` で返します。平方根が存在しない場合は `-1`。負の `y` と法 2 にも対応します。

法は `long long` に収まる素数を渡します。法の素数判定は呼び出し側で済ませてください。べき乗には基本テンプレートの `powmod` を使います。

## 計算量

Tonelli–Shanks 法と平方非剰余の昇順探索を使います。非剰余探索で試した個数を $Z$ とすると時間 $O((Z+\log p)\log p)$、基本テンプレートの累乗のコールスタックを含む追加領域 $O(\log p)$ です。

## 使用例

```cpp
auto root=modular_sqrt(10,13);
if(root<0 || root*root%13!=10 || modular_sqrt(2,13)!=-1)std::abort();
```

[Library Checker の対応問題](https://judge.yosupo.jp/problem/sqrt_mod)
