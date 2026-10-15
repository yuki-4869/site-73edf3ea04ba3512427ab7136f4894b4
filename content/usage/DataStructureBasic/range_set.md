# RangeSet — 整数の区間集合

[説明書の目次](../index.md) · [コード](../../../DataStructureBasic/range_set.cpp) · [確認対象の改訂版](https://github.com/yuki-4869/Algorithm/blob/56ecc78753f54a85b653e23783dcab7ea0c48eb6/DataStructureBasic/range_set.cpp)

整数集合を、重なりも隣接もない最大の半開区間 `[l,r)` にまとめて管理します。
区間・1点の追加と削除、被覆判定、mex、被覆数の取得に使用できます。

2026-10-15改訂。旧版の削除APIのコンパイルエラー、隣接区間の未統合、削除数・総被覆数の誤りを修正しました。

**記号**

- `K`: 現在保持している区間数。被覆している整数の個数とは異なります。
- `D`: 操作で統合・削除・走査する保持区間数。
- `[l,r)`: `l` を含み、`r` を含まない半開区間。整数の個数は `r-l`。
- `l >= r` は空区間として扱います。追加・削除・個数取得は0、被覆判定はtrueです。

**型・使用可能な範囲**

```cpp
template<class T = long long, class Count = T>
class RangeSet;
```

- `T`: 64bit以下の符号付き整数型。座標・区間端点・mexの型です。
- `Count`: 被覆数の型。64bit以下の整数型（boolを除く）または `__int128_t`。省略時は `T` です。
- 区間端点は `T` の最小値から最大値まで使用できます。内部の差分計算は `__int128_t` を使い、符号付き整数のオーバーフローを避けています。
- 格納可能な整数は `T` の最小値以上、最大値未満です。最大値を含む半開区間の右端は `T` で表現できません。
- 新しい総被覆数が `Count` の最大値を超える `insert` は、集合を変更せず `std::overflow_error` を投げます。

ソースには必要な標準ヘッダを含めています。ファイル全体を貼り付けるか、ローカルではincludeして使えます。GNU C++23で確認しています。`__int128_t` はGNU拡張です。

<details>
<summary>API一覧</summary>

- [コンストラクタ・コピー・ムーブ](#constructors)
- [insert](#insert)
- [erase](#erase)
- [covered / contains](#covered)
- [covering_interval / covered_by](#covering)
- [overlaps](#overlaps)
- [mex](#mex)
- [count_covered / cover_length](#count)
- [size / empty / clear](#state)
- [intervals / begin / end / output](#traverse)
- [swap](#swap)
- [使用例](#example)
- [旧版からの移行](#migration)
- [確認結果](#verification)

</details>

<a id="constructors"></a>

## コンストラクタ・コピー・ムーブ

```cpp
RangeSet();
RangeSet(std::initializer_list<ptt> ranges);
RangeSet(const RangeSet& other);
RangeSet& operator=(const RangeSet& other);
RangeSet(RangeSet&& other) noexcept;
RangeSet& operator=(RangeSet&& other) noexcept;
```

既定コンストラクタは空集合を作ります。初期化子リストでは区間を順に追加し、重複・隣接を統合します。
コピーした集合は独立しています。ムーブ元は空集合となり、再利用できます。
`ptt` は `std::pair<T,T>`、`count_type` は `Count`、`value_type` は `T` です。

**計算量**

- 空集合の構築: `O(1)`。
- A個の区間から構築: `O(A log(A+1))`。
- コピー: `O(K)`。コピー代入は代入先の既存区間の解放も行います。
- ムーブ構築: `O(1)`。ムーブ代入は代入先の既存区間の解放が必要です。

<a id="insert"></a>

## insert

```cpp
Count insert(T l, T r);
Count insert(T x);
```

`[l,r)` または1点 `x` を追加し、今回新たに被覆した整数の個数を返します。
重なっている区間に加え、左右で接している区間も統合します。同じ区間の重複追加は0を返します。

**制約**

- 1点追加では `x < std::numeric_limits<T>::max()`。最大値を指定すると `std::out_of_range` を投げ、集合は変わりません。
- 追加後の総被覆数が `Count` に収まること。超過時は上記の `std::overflow_error` となります。

**計算量**

- `O(log(K+1)+D)`。ここでDは統合する区間数。

<a id="erase"></a>

## erase

```cpp
Count erase(T l, T r);
Count erase(T x);
```

`[l,r)` または1点 `x` を削除し、今回実際に削除した整数の個数を返します。
被覆されていない部分は無視します。削除範囲の左右に残る部分は保持します。
例えば `[1,10)` から `[3,7)` を削除すると、返り値は4、残る区間は `[1,3)` と `[7,10)` です。
`T` の最大値は格納されないため、その1点の削除は0を返します。

**計算量**

- `O(log(K+1)+D)`。ここでDは削除範囲と交わる区間数。

<a id="covered"></a>

## covered / contains

```cpp
bool covered(T l, T r) const;
bool covered(T x) const;
bool contains(T x) const;
```

2引数版は `[l,r)` 全体が被覆されているかを返します。空区間はtrueです。
1引数版と `contains` は `x` が含まれるかを返します。

**計算量**

- `O(log(K+1))`。空区間の判定は `O(1)`。

<a id="covering"></a>

## covering_interval / covered_by

```cpp
std::optional<ptt> covering_interval(T x) const;
std::optional<ptt> covering_interval(T l, T r) const;
ptt covered_by(T l, T r) const;
```

`covering_interval` は指定点または区間全体を含む保持区間を返し、存在しなければ `std::nullopt` を返します。
2引数版で `l >= r` の場合は、保持区間を検索せず `{l,l}` を返します。

`covered_by` は旧版との互換APIです。区間全体を含む保持区間、見つからなければ `{-TINF,-TINF}`、空区間なら `{0,0}` を返します。
`TINF = std::numeric_limits<T>::max()/2` はこの返り値だけのために残しており、内部の番兵や座標制限には使用しません。

**計算量**

- `O(log(K+1))`。空区間は `O(1)`。

<a id="overlaps"></a>

## overlaps

```cpp
bool overlaps(T l, T r) const;
```

`[l,r)` と集合の共通部分が1点以上あるかを返します。空区間はfalseです。
例えば `[1,3)` だけが被覆されているとき、`overlaps(2,4)` はtrue、`overlaps(3,4)` はfalseです。

**計算量**

- `O(log(K+1))`。

<a id="mex"></a>

## mex

```cpp
T mex(T x = 0) const;
```

`x` 以上で集合に含まれない最小の整数を返します。`x` が未被覆なら、そのまま `x` を返します。
区間を最大限に統合しているため、`x` が被覆されていれば、その保持区間の右端が答えです。
最大値は常に未被覆なので、返り値は `T` で表現できます。

**計算量**

- `O(log(K+1))`。

<a id="count"></a>

## count_covered / cover_length

```cpp
Count count_covered(T l, T r) const;
Count cover_length() const;
```

`count_covered` は `[l,r)` 内で被覆されている整数の個数を返します。空区間は0です。
`cover_length` は集合全体の被覆数を返します。

**計算量**

- `count_covered`: `O(log(K+1)+D)`。最悪 `O(K)`。
- `cover_length`: `O(1)`。

<a id="state"></a>

## size / empty / clear

```cpp
std::size_t size() const;
bool empty() const;
void clear();
```

`size` は保持する区間数を返します。整数の個数は `cover_length` を使います。
`empty` は集合が空かを返し、`clear` はすべての区間を削除して被覆数を0にします。

**計算量**

- `size`、`empty`: `O(1)`。
- `clear`: `O(K)`。

<a id="traverse"></a>

## intervals / begin / end / output

```cpp
std::vector<ptt> intervals() const;
const_iterator begin() const;
const_iterator end() const;
void output() const;
```

`intervals` は左端の昇順で区間をコピーして返します。返したvectorを変更しても元の集合は変わりません。
`begin` / `end` は読み取り専用のイテレータで、`for (auto [l,r] : rs)` と書けます。
`output` は `RangeSet: [l,r) ...` の形式で標準出力に表示します。

**制約**

- 区間走査中に集合を更新しないでください。追加・削除・clear等で保持区間やイテレータが変わる場合があります。

**計算量**

- `intervals`、全区間の走査、`output`: `O(K)`。
- `begin`、`end`: `O(1)`。
- `intervals` の返却vectorには追加で `O(K)` のメモリを使います。

<a id="swap"></a>

## swap

```cpp
void swap(RangeSet& other) noexcept;
```

区間集合と総被覆数を、同じ型の `other` と交換します。

**計算量**

- `O(1)`。

<a id="example"></a>

## 使用例

ソースをグローバルに用意し、`<cassert>` をincludeして、次を `main` 内で実行します。

```cpp
RangeSet<long long> r;
assert(r.insert(5,10)==5);
assert(r.insert(1,5)==4);            // 隣接区間も統合
assert(r.covered(1,10) && r.mex(1)==10);

assert(r.erase(3,7)==4);             // 左右の断片を残す
assert(r.cover_length()==5);
assert((r.intervals()==std::vector<std::pair<long long,long long>>{{1,3},{7,10}}));
assert(r.count_covered(2,9)==3);
assert(r.overlaps(2,4) && !r.covered(2,4));

auto interval=r.covering_interval(8);
assert((interval==std::optional{std::pair{7LL,10LL}}));
assert(r.erase(8)==1 && !r.contains(8));
assert(r.insert(8)==1 && r.mex(7)==10);

RangeSet<long long,__int128_t> wide;
const auto low=std::numeric_limits<long long>::min();
const auto high=std::numeric_limits<long long>::max();
const __int128_t span=static_cast<__int128_t>(high)-low;
assert(wide.insert(low,high)==span);
assert(wide.cover_length()==span && wide.mex(low)==high);
```

## メモリ

保持区間数に対して `O(K)`。番兵は格納しません。

<a id="migration"></a>

## 旧版からの移行

| 旧版 | 改訂版 |
| --- | --- |
| `RangeSet<long long>` と主要APIの呼び出し | そのまま使用可能。Count省略時の個数の型もlong long |
| `st` への直接アクセス | `intervals()` または読み取り専用の区間走査を使用 |
| `total_covered` フィールド | `cover_length()` を使用 |
| `size()` の返り値がint | std::size_tへ変更 |
| `TINF` を変更し、番兵・許容座標を調整 | TINFは互換返り値用の定数。座標にこの制限はない |
| `covered_by` の未発見・空区間の返り値 | 維持。新規コードにはoptionalを返す `covering_interval` も利用可能 |

内部データは非公開にしており、外部からの変更で区間の不変条件が壊れないようにしています。

<a id="verification"></a>

## 確認結果

- GCC 13.3.0、`-std=c++23` でコンパイル・実行。
- 旧版の不具合、空区間、完全削除、左右の切り詰め・分割、重複・隣接追加、負の座標を確認。
- int / long longを各64シード・4096操作、計524,288操作で `std::set` と比較。更新の返り値、保持区間、被覆数、被覆判定、mex、overlaps、区間取得も確認。
- 座標の最小値・最大値付近、Countの容量超過時に集合が変わらないこと、unsigned long long / __int128_tの個数、コピー・ムーブを確認。
- AddressSanitizer、UndefinedBehaviorSanitizer、libstdc++のデバッグイテレータを使用した実行でエラーなし。実行環境の制限によりLeakSanitizerは無効にしています。

[テストコード](../../../tests/range_set_test.cpp) · [C++23の確認結果](../validation.md)

```sh
g++ -std=c++23 -O2 tests/range_set_test.cpp -o /tmp/range_set_test
/tmp/range_set_test
```

テストは全入力の正当性の証明ではありません。計算量やGNU以外のコンパイラについては、上記の使用条件を参照してください。
