# XOR基底

[説明書の目次](../index.md) · [コード](../../../LinearAlgebra/xor_basis.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/LinearAlgebra/xor_basis.cpp)

挿入した整数の部分集合XORで作れる値を、GF(2)線形基底で管理します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

B=numeric_limits<T>::digits。

**共通の前提**

- Tは符号なし整数。加算の代わりにXORを使います。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [XorBasis::insert](#api-2)
- [XorBasis::contains](#api-3)
- [XorBasis::max_xor](#api-4)
- [XorBasis::min_xor](#api-5)
- [XorBasis::rank](#api-6)
- [XorBasis::independent](#api-7)
- [XorBasis::merge](#api-8)
- [XorBasis::vectors](#api-9)
- [XorBasis::clear](#api-10)

</details>

## 型・テンプレート引数

```cpp
template<class T = unsigned long long>
struct XorBasis;
```

<a id="api-1"></a>

## コンストラクタ

```cpp
XorBasis();
```

空の状態で構築します。

**計算量**

- O(B)。

<a id="api-2"></a>

## XorBasis::insert

```cpp
bool insert(T x);
```

xが独立なら基底へ追加しtrue、既存基底で表せればfalse。

**計算量**

- O(B)。

<a id="api-3"></a>

## XorBasis::contains

```cpp
bool contains(T x) const;
```

xが基底の部分集合XORで作れるかを返します。0は常にtrue。

**計算量**

- O(B)。

<a id="api-4"></a>

## XorBasis::max_xor

```cpp
T max_xor(T x = 0) const;
```

x xor 部分集合XOR の最大値を返します。x省略時は0。

**計算量**

- O(B)。

<a id="api-5"></a>

## XorBasis::min_xor

```cpp
T min_xor(T x = 0) const;
```

x xor 部分集合XOR の最小値を返します。

**計算量**

- O(B)。

<a id="api-6"></a>

## XorBasis::rank

```cpp
int rank() const;
```

独立な基底数を返します。

**計算量**

- O(1)。

<a id="api-7"></a>

## XorBasis::independent

```cpp
bool independent(T x) const;
```

xを追加するとrankが増えるかを返します。状態は変えません。

**計算量**

- O(B)。

<a id="api-8"></a>

## XorBasis::merge

```cpp
void merge(const XorBasis& other);
```

otherの基底をこの基底へ追加します。otherは変更しません。

**計算量**

- O(B^2)。

<a id="api-9"></a>

## XorBasis::vectors

```cpp
vector<T> vectors() const;
```

保持する非零基底ベクトルを返します。

**計算量**

- O(B)。

<a id="api-10"></a>

## XorBasis::clear

```cpp
void clear();
```

空の基底へ戻します。

**計算量**

- O(B)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
XorBasis<> b; b.insert(3); b.insert(5);
assert(b.rank()==2 && b.contains(6) && !b.contains(1));
assert(b.max_xor()==6);
```

## メモリ

O(B)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
