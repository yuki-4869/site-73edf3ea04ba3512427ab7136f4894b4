# 永続キュー

末尾追加と先頭削除を行う FIFO キューです。更新の結果を版として受け取り、過去のどの版からでも新しい版を作れます。

## API

`PersistentQueue<T> q(P)` は、全ての版を通して高々 $P$ 回の `push` が行われるものとして記憶域を予約します。空の版は `PersistentQueue<T>::Version{}` で作ります。

| 操作 | 戻り値と内容 | 時間 |
| --- | --- | --- |
| `q.push(v, x)` | `Version`。版 `v` の末尾に `x` を追加した版 | $O(\log(P+2))$ |
| `q.front(v)` | `T`。版 `v` の先頭要素をコピーして返す | $O(\log(P+2))$ |
| `q.pop(v)` | `Version`。版 `v` の先頭を削除した版 | $O(1)$ |
| `q.size(v)` | `int`。版 `v` の要素数 | $O(1)$ |
| `q.empty(v)` | `bool`。版 `v` が空なら真 | $O(1)$ |

`front` と `pop` は空の版に使えません。空版と、そのインスタンスが発行した版を渡します。表の計算量は `T` のコピー・ムーブを $O(1)$ とします。予約する領域は $O(P\log(P+2))$。各版の値は末尾ノードと長さの組なので保持に $O(1)$ です。先頭は末尾から長さ分だけダブリングでさかのぼって求めます。

## 使用例

```cpp
PersistentQueue<int> q(3);
PersistentQueue<int>::Version empty;
auto a=q.push(empty,10);
auto b=q.push(a,20);
auto c=q.pop(b);
auto d=q.push(a,30);
if(q.front(c)!=20 || q.front(q.pop(d))!=30 || q.front(b)!=10)std::abort();
```

[Library Checker・永続キュー](https://judge.yosupo.jp/problem/persistent_queue)
