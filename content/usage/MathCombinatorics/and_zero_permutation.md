# ANDが0になる順列

[説明書の目次](../index.md) · [コード](../../../MathCombinatorics/and_zero_permutation.cpp)

連続した整数列Aに対し、各位置で `(A[i] & B[i]) == 0` を満たす順列Bを構築します。
0始まりの値域では常に構築でき、1始まりの値域ではNが偶数のときだけ構築できます。

## and_zero_permutation

```cpp
std::vector<long long> and_zero_permutation(
    long long N, bool zero_indexed = true);
```

N要素の順列を返します。

| zero_indexed | Aの値 | Bの値域 | 構築可能な条件 |
| --- | --- | --- | --- |
| `true`（省略時） | `A[i] = i` | 0からN-1 | すべてのN |
| `false` | `A[i] = i+1` | 1からN | Nが偶数 |

**戻り値**

- 構築可能な場合は、各位置のビットANDが0となる順列Bを返します。
- `false` かつNが奇数の場合は、解がないため空配列を返します。
- N=0は有効な空順列であり、どちらのモードでも空配列を返します。
- 返すvectorの添字は、どちらのモードでも0始まりです。`false` の場合の条件は `((i+1) & B[i]) == 0` です。

**制約**

- N>=0。N<0の場合は `std::invalid_argument` を投げます。
- N要素の配列を確保できること。解なしの場合は、この配列を確保しません。

**計算量**

- 時間O(N)。`false` かつNが奇数の場合の解なし判定はO(1)。
- 返却配列の領域O(N)。それ以外の追加領域O(1)。

## 使用例

実装全体をグローバルに置き、以下をmain内などで実行します。`assert` を使う場合は `<cassert>` もincludeしてください。

```cpp
auto b = and_zero_permutation(5);
assert((b == std::vector<long long>{0, 2, 1, 4, 3}));
for (long long i = 0; i < 5; ++i) {
    assert((i & b[i]) == 0);
}

auto c = and_zero_permutation(4, false);
assert((c == std::vector<long long>{2, 1, 4, 3}));
for (long long i = 0; i < 4; ++i) {
    assert(((i + 1) & c[i]) == 0);
}

auto impossible = and_zero_permutation(3, false);
assert(impossible.empty());
assert(and_zero_permutation(0).empty());
```

## 構築の仕組み

未構築部分の最大値をrとし、r以上の最小の $2^k-1$ をmaskとします。
$l=\mathrm{mask}-r$ と置き、区間 $[l,r]$ をまとめて対応付けます。

```cpp
B[x - first] = mask ^ x;  // firstは0または1
```

maskの有効ビットはすべて1なので、`x & (mask ^ x) == 0` です。
また、区間内では $\mathrm{mask}\mathbin{\mathrm{xor}}x=\mathrm{mask}-x$ となり、区間 $[l,r]$ を反転した順列になります。
これを残った小さい値にも繰り返します。各値への代入は1回で、maskのシフトも全体でO(log(N+1))回です。

1始まりでNが偶数の場合、rは各反復で偶数、lは奇数となるため、正の値だけで最後まで構築できます。
Nが奇数の場合は、1からNに含まれる奇数の数が偶数の数より1つ多くなります。
奇数のAには偶数のBを割り当てる必要があるため、この場合は構築できません。

## 確認結果

両方の値域について、順列性・各位置のAND・解なし判定を検査しています。
N<=24では独立した二部マッチングによる存在判定とも比較し、N=0・負のN・2の冪の前後も確認しています。
テストはサイト用リポジトリの `tests/and_zero_permutation_test.cpp` にあり、サイトの更新時にも実行します。
