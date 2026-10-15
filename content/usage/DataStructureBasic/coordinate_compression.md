# 座標圧縮 — Zip

[説明書の目次](../index.md) · [コード](../../../DataStructureBasic/coordinate_compression.cpp) · [確認対象の改訂版](https://github.com/yuki-4869/Algorithm/blob/0eacd5e38b1bf98e3870a77821d54cee82f011e0/DataStructureBasic/coordinate_compression.cpp)

値を比較順に並べ、同値のものを1種類にまとめて0-indexedの番号を割り当てます。
整数だけでなく、pair・tuple・stringや、独自の比較器で順序を定義した型も扱えます。
2026-10-15改訂。宣言順のコンパイルエラーを修正し、未構築・範囲外の検出、厳密な番号取得、逆変換を追加しました。

**記号**

- `N`: 登録配列の長さ。未構築の場合は重複を含みます。
- `U`: 比較器で区別される値の種類数。構築後の表の長さ。
- `M`: 操作に渡す配列の長さ。
- 以下の計算量は、比較・値と比較器のコピー・移動をO(1)としたものです。

## 型と使い始め方

```cpp
namespace coordinate_zip {
template<class T, class Compare = std::less<T>>
class Zip;
}
using coordinate_zip::Zip;
```

ソースには必要な標準ヘッダを含めています。ファイル全体をグローバルに貼り付けるか、ローカルではincludeして使えます。
`Zip<long long>` のように短い名前でも利用できます。
`value_type` はT、`const_iterator` は読み取り専用のイテレータ、`const_reference` は通常 `const T&`（T=boolの場合はbool）です。

**制約**

- Tはコピー・移動・ソートに必要な操作が可能であること。
- Compareはstrict weak ordering（厳密弱順序）を定義し、コピー可能であること。既定はstd::less<T>。
- 同値性は `!compare(a,b) && !compare(b,a)` で判定します。Tのoperator==は不要です。
- 異なる値の種類数UはINT_MAX以下。超過時はbuildがstd::length_errorを投げます。

比較器で同値とした値のうち、どれを代表値として残すかは保証しません。
例えばレコードのkeyだけで比較する場合、同じkeyを持つ別のレコードに逆変換されることがあります。
圧縮は順序を保ちますが、元の数値の差や距離は保ちません。

## 構築状態

| 操作 | 操作後の状態 |
| --- | --- |
| 既定コンストラクタ・比較器だけのコンストラクタ | 構築済みの空表 |
| 配列・初期化子リストから構築、assign | 構築済み |
| 1値または空でない配列・リストをadd | 未構築 |
| build | 構築済み |
| clear | 構築済みの空表 |
| reserve、空の配列・リストのadd | 状態を変更しない |

未構築状態で検索・逆引き・size・empty・values・列挙・配列変換を行うと `std::logic_error` を投げます。NDEBUGでも検出します。
未構築でもadd、build、assign、clear、reserve、ready、pending_sizeは利用できます。
追加して再構築すると、以前の圧縮番号も変わる場合があります。使用する値をすべて登録してから番号を取得してください。

<details>
<summary>API一覧</summary>

- [コンストラクタ・コピー・ムーブ](#constructors)
- [add / build / assign](#build)
- [reserve / clear / ready / pending_size](#state)
- [get / lower / upper](#bounds)
- [index / contains / equal_range / count_range](#lookup)
- [operator[] / at / values / begin / end](#reverse)
- [size / empty](#size)
- [zip / zip_exact / unzip](#convert)
- [一括zip / zip2d](#helpers)
- [使用例](#example)
- [旧版からの移行](#migration)
- [確認結果](#verification)

</details>

<a id="constructors"></a>

## コンストラクタ・コピー・ムーブ

```cpp
Zip();
explicit Zip(Compare compare);
explicit Zip(const std::vector<T>& a, Compare compare = Compare{});
Zip(std::initializer_list<T> a, Compare compare = Compare{});
Zip(const Zip& other);
Zip& operator=(const Zip& other);
Zip(Zip&& other);
Zip& operator=(Zip&& other);
```

配列・リストからの構築ではソートと重複除去まで行います。比較器を省略すると昇順です。
コピーした表は独立しています。ムーブ元は構築済みの空表になり、比較器も引き続き利用できます。
ムーブのnoexcept条件は、比較器のコピー構築・コピー代入が例外を投げないことです。
比較器を省略する構築には比較器の既定構築、コピー・ムーブ代入には比較器のコピー代入が必要です。

**計算量**

- 空表の構築: O(1)。配列・リストからの構築: O(M log(M+1))。
- コピー構築: O(N_other)。コピー代入: O(N+N_other)。
- ムーブ構築: O(1)。ムーブ代入: O(N+1)。

<a id="build"></a>

## add

```cpp
void add(const T& x);
void add(const std::vector<T>& a);
void add(std::initializer_list<T> a);
```

値を追加し、未構築状態にします。空の配列・リストは何も変えません。`z.add(z[i])` や `z.add(z.values())` にも対応しています。

**計算量**

- 1値: ならしO(1)。配列・リスト: ならしO(M+1)。再確保が必要な1回の操作では既存値の移動も行います。

## build

```cpp
void build();
```

登録値を比較順にソートし、比較器で同値な値を除去して構築済みにします。構築済みで再び呼んだ場合は何も行いません。

**計算量**

- 未構築ならO(N log(N+1))。構築済みならO(1)。

## assign

```cpp
void assign(const std::vector<T>& a);
void assign(std::initializer_list<T> a);
```

登録値をaに置き換え、ソート・重複除去まで行います。比較器は維持します。`z.assign(z.values())` にも対応しています。

**計算量**

- O(N+M log(M+1))。

<a id="state"></a>

## reserve / clear / ready / pending_size

```cpp
void reserve(std::size_t n);
void clear();
bool ready() const;
std::size_t pending_size() const;
```

reserveは登録配列の容量を確保します。clearは全値を削除し、構築済みの空表にします。readyは構築済みならtrue。pending_sizeは現在の登録配列の長さを返し、未構築では重複を含みます。

**計算量**

- reserve: O(N)以下。clear: O(N)。ready / pending_size: O(1)。

<a id="bounds"></a>

## get / lower / upper

```cpp
int get(const T& x) const;
int lower(const T& x) const;
int upper(const T& x) const;
```

getとlowerは、比較順序でxより前にある登録値の種類数を返します。upperは、xより前またはxと同値の種類数です。返り値は0以上U以下です。

未登録値にも挿入位置を返します。例えば表が `{10,30}` なら `get(20)==1` ですが、20は登録されていません。既定の昇順ではlowerはx未満、upperはx以下の種類数に対応します。

**計算量**

- O(log(U+1))。

<a id="lookup"></a>

## index / contains

```cpp
int index(const T& x) const;
bool contains(const T& x) const;
```

indexはxと同値の登録値があれば、その圧縮番号を返します。存在しなければ-1です。containsは存在すればtrueを返します。

**計算量**

- O(log(U+1))。

## equal_range / count_range

```cpp
std::pair<int,int> equal_range(const T& x) const;
int count_range(const T& l, const T& r) const;
```

equal_rangeは `{lower(x),upper(x)}` を返します。表は重複を除いているため、その差は登録済みなら1、未登録なら0です。

count_rangeは比較順序でl以上r未満にある登録値の種類数を返します。rがl以前にある場合は0です。元配列に含まれた重複回数は数えません。

**計算量**

- O(log(U+1))。

<a id="reverse"></a>

## operator[] / at

```cpp
const_reference operator[](int i) const;
const_reference at(int i) const;
```

圧縮番号iの代表値を返します。どちらも境界チェックを行い、番号が範囲外ならstd::out_of_rangeを投げます。

**制約**

- 0 <= i < U。違反はNDEBUGでも検出します。

**計算量**

- O(1)。

## values / begin / end

```cpp
const std::vector<T>& values() const;
const_iterator begin() const;
const_iterator end() const;
```

valuesは比較順に並んだ重複なしの表を読み取り専用で返します。begin/endで同じ表を走査できます。コピーが必要なら `auto table=z.values();` とします。

**制約**

- 取得した値の参照・イテレータは、add / build / assign / clear / reserve / 代入などで無効になる場合があります。

**計算量**

- 取得はO(1)、表全体の走査・コピーはO(U)。

<a id="size"></a>

## size / empty

```cpp
int size() const;
bool empty() const;
```

sizeは異なる値の個数U、emptyは表が空ならtrueを返します。未構築の登録配列の長さにはpending_sizeを使います。

**計算量**

- O(1)。

<a id="convert"></a>

## zip / zip_exact / unzip

```cpp
std::vector<int> zip(const std::vector<T>& a) const;
std::vector<int> zip_exact(const std::vector<T>& a) const;
std::vector<T> unzip(const std::vector<int>& ids) const;
```

zipは各値のlower位置を返す旧版互換の変換です。未登録値も許可し、番号Uになる場合もあります。

zip_exactは登録値だけを圧縮し、1つでも未登録値があればstd::out_of_rangeを投げます。新しく圧縮番号を配列の添字として使う場合は、こちらも利用できます。

unzipは番号列を代表値の列に復元します。範囲外の番号があればstd::out_of_rangeを投げます。比較器で同値とした別個の値を、元の個別値へ復元する操作ではありません。

**計算量**

- zip / zip_exact: O(M log(U+1))。unzip: O(M)。返却配列のメモリはO(M)。

<a id="helpers"></a>

## coordinate_zip::zip / zip2d

```cpp
template<class T, class Compare = std::less<T>>
std::vector<int> zip(const std::vector<T>& a, Compare compare = Compare{});
template<class X, class Y, class CompareX = std::less<X>, class CompareY = std::less<Y>>
std::vector<std::pair<int,int>> zip2d(const std::vector<std::pair<X,Y>>& a,
                                    CompareX cx = CompareX{}, CompareY cy = CompareY{});
```

zipは表の構築も行い、aの各値の圧縮番号を返します。入力した値はすべて登録するため、未登録値の問題はありません。

zip2dはpairの第一成分・第二成分を独立に圧縮します。`Zip<pair<X,Y>>` によるpair全体の辞書順圧縮とは、番号の意味が異なります。各軸の比較器も指定できます。

**計算量**

- O(M log(M+1))。追加メモリO(M)。

<a id="example"></a>

## 使用例

ソースをグローバルに用意し、`<cassert>` をincludeして、次をmain内で実行します。

```cpp
Zip<long long> z{30,10,30,20};
assert(z.ready() && z.size()==3);
assert(z.get(20)==1 && z[1]==20);
assert(z.get(15)==1 && z.index(15)==-1 && !z.contains(15));
assert((z.zip_exact({30,10,20})==std::vector<int>{2,0,1}));
assert((z.unzip({2,0,1})==std::vector<long long>{30,10,20}));
assert(z.count_range(10,30)==2);

z.add(5);
assert(!z.ready());
z.build();                              // 以前の番号も変わる
assert(z.index(10)==1 && z.index(30)==3);

Zip<long long,std::greater<long long>> descending({30,10,20});
assert(descending.index(30)==0 && descending.index(10)==2);

std::vector<std::pair<int,long long>> points={{20,100},{10,200},{20,200}};
auto result=coordinate_zip::zip2d(points);
assert((result==std::vector<std::pair<int,int>>{{1,0},{0,1},{1,1}}));
```

## メモリ

登録配列の要素数はN、確保メモリはvectorのcapacityに比例します。通常はO(N)ですが、構築後にも旧容量が残り、reserve(n)で要素数より大きな容量を確保することもあります。
配列を変換・コピーして返すAPIは、別途O(M)またはO(U)の返却用メモリを使います。

<a id="migration"></a>

## 旧版からの移行

| 旧版 | 改訂版 |
| --- | --- |
| Zip<T>、add / build / get / lower / upper / zip / zip2d | 呼び方と主要な番号の意味を維持 |
| 先頭のusing宣言を手動で移動 | ソース全体をそのまま使用可能 |
| get / zipが未登録値の挿入位置を返す | 維持。厳密な取得にはindex / zip_exactを追加 |
| vectorからZipへの暗黙変換 | 配列のコンストラクタはexplicit。`Zip<T> z(A)`で明示的に構築 |
| xsへの直接アクセス | values()または読み取り専用走査を使用 |
| builtフィールド | ready()を使用 |
| 未構築でもsizeが登録配列の長さを返す | sizeは構築後のU。未構築の長さはpending_size() |
| assertだけによる未構築・添字の検査 | logic_error / out_of_rangeでNDEBUGでも検出 |
| 重複除去でTのoperator==を使用 | 比較器の等価性を使用。==がない型も対応 |

既定の空表は構築済みになりました。既存の空表にbuildしても、そのままO(1)で利用できます。

<a id="verification"></a>

## 確認結果

- GCC 13.3.0、GNU C++23でソースの標準ヘッダだけを使う専用テストと、基本テンプレートを併用する使用例を確認。
- 空表、重複、再構築、未登録値、未構築での検索・列挙・変換、範囲外番号、自己参照の追加・置換、コピー・ムーブを確認。
- long longの最小値・最大値、文字列・pair・tuple・bool、operator== / <のない型、独自の比較器を確認。
- 昇順long long / 降順intを各64シード・1024ステップ、計131,072ステップで独立なstd::setと比較。各ステップで複数の検索・逆引き・変換も確認。
- NDEBUGを有効にした実行でも状態・境界チェックが機能。AddressSanitizer・UBSan・デバッグイテレータでの実行も成功。実行環境の制限でLeakSanitizerは無効。

[専用テスト](../../../tests/coordinate_compression_test.cpp) · [C++23の確認結果](../validation.md)

```sh
g++ -std=c++23 -O2 tests/coordinate_compression_test.cpp -o /tmp/coordinate_zip_test
/tmp/coordinate_zip_test
```

テストは全入力に対する正当性の証明ではありません。別のコンパイラや不正な比較器の使用は、この確認結果に含めていません。
