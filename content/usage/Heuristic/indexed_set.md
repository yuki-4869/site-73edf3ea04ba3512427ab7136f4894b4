# 整数IDの動的集合

`0..universe-1` のうち、現在有効なIDを密な配列で保持します。候補の追加・削除が多く、有効なIDから一様に1つ選びたいときに使えます。

## API

| API | 返り値・使い方 | 計算量 |
| --- | --- | --- |
| `ahc::IndexedSet(universe)` | 0以上の `universe` で空集合を構築 | O(universe) |
| `active.size()`, `active.empty()` | 要素数、空なら `true` | O(1) |
| `active.values()` | 要素列への `span<const int>` | O(1) |
| `active.contains(id)` | 所属していれば `true` | O(1) |
| `active.insert(id)` | 新規に追加したら `true` | 償却O(1) |
| `active.erase(id)` | 削除できたら `true` | O(1) |
| `active.clear()` | 全要素を削除 | O(size) |

IDは0-indexedです。削除時に列の末尾要素を空いた位置に移すため、列の順序は変わります。 `values()` は変更後に取得し直します。非空なら `values()[rng.below(active.size())]` で一様に抽選できます。 `rng.below()` は [高速乱数](random.md) の整数抽選です。領域はO(universe)です。

## 使用例

```cpp
ahc::IndexedSet vacant(10);
for (int id : {2,4,8}) vacant.insert(id);
vacant.erase(4);
assert(!vacant.contains(4) && vacant.size() == 2);
auto ids = vacant.values();
assert(ids[0] == 2 && ids[1] == 8);
```
