# ビーム探索の経路復元

[ビーム探索の候補選択](beam_selection.md) で採用した候補の親と操作を残し、最終ノードから操作列を復元します。各層の候補の `parent` は前層内の添字なので、 `previous_ids[candidate.parent]` で履歴ノードへ変換してから保存します。

## API

| API | 返り値・使い方 | 計算量 |
| --- | --- | --- |
| `ahc::BeamTrace<Move> trace` | 根だけを持つ空の履歴 | O(1) |
| `trace.root()`, `trace.size()` | 根のIDは0、要素数は根を除く | O(1) |
| `trace.reserve(count)` | 操作ノードの領域を予約 | 最悪O(size) |
| `trace.append(parent,move)` | 操作を記録し、新ノードの `NodeId` を返す | 償却O(1) |
| `trace.path(id)` | 根からノードまでの `vector<Move>` | O(1+経路長) |
| `trace.clear()` | 根以外を破棄 | O(size) |

`NodeId` は `size_t` です。親は根またはすでに登録したノードを指定します。 `path()` で操作を復元するには、コピー可能な `Move` が必要です。操作がO(1)サイズなら、領域は同時に保持した候補の最大件数に比例します。

## 使用例

```cpp
ahc::BeamTrace<char> trace;
auto first = trace.append(trace.root(),'D');
auto alternative = trace.append(trace.root(),'R');
auto best = trace.append(first,'R');
assert(trace.path(alternative) == vector<char>{'R'});
assert(trace.path(best) == (vector<char>{'D','R'}));
```
