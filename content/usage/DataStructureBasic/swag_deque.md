# 区間集約付き deque（SWAG）

deque の両端に対する追加・削除を行い、現在の全要素の集約を取得できます。`SwagDeque<S,op,e> dq;` を空の状態で構築します。

## API

| 操作 | 返り値と内容 |
| --- | --- |
| `dq.push_front(x)`, `dq.push_back(x)` | `void`。先頭・末尾への追加 |
| `dq.pop_front()`, `dq.pop_back()` | `void`。先頭・末尾を削除 |
| `dq.front()`, `dq.back()` | `const S&`。先頭・末尾の要素 |
| `dq.all_prod()` | `S`。先頭から末尾への順序の集約。空なら `e()` |
| `dq.size()`, `dq.empty()` | `size_t` の要素数、`bool` の空判定 |

`op` は結合則を満たし、`e()` を単位元とします。可換性は不要です。先頭・末尾の参照と削除は、要素があるときに行います。参照は deque を更新するまで使えます。

## 計算量

両端操作は償却 $O(1)$、その他の操作は $O(1)$。保持した最大要素数を $N$ とすると領域は $O(N)$ です。要素のコピー・ムーブと集約演算が定数時間の場合の計算量です。

## 使用例

```cpp
constexpr auto concat=[](const std::string& a,const std::string& b){return a+b;};
constexpr auto identity=[](){return std::string{};};
SwagDeque<std::string,concat,identity> dq;
dq.push_back("b"); dq.push_front("a"); dq.push_back("c");
dq.pop_front(); dq.pop_front();
if(dq.all_prod()!="c" || dq.front()!="c")std::abort();
```

[Library Checker の対応問題](https://judge.yosupo.jp/problem/deque_operate_all_composite)
