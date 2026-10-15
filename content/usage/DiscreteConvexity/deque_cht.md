# Deque CHT

[説明書の目次](../index.md) · [コード](../../../DiscreteConvexity/deque_cht.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/DiscreteConvexity/deque_cht.cpp)

複数の一次関数について、指定したxでの最小値／最大値を管理します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

MINIMIZE=trueが最小値、falseが最大値。H=保持直線数。a,bは傾きと切片。

**共通の前提**

- Tは64bit以下の符号付き整数を推奨。最小値版では傾きaを非増加、最大値版では非減少に追加。query前に1本以上を登録し、結果はTに収めること。queryのxも非減少順。

## 型・テンプレート引数

```cpp
template<class T = long long, bool MINIMIZE = true>
struct DequeCHT;
```

## この実装の注意点

破棄した直線は後の大きいxに対して不要になるため、xを小さい方へ戻すことはできません。

## コンストラクタ

```cpp
DequeCHT();
```

空の状態で構築します。

**計算量**

- O(1)。

## DequeCHT::add_line

```cpp
void add_line(T a, T b);
```

直線y=a*x+bを登録します。同じ傾きでは目的に応じて優れた切片を残します。

**計算量**

- ならしO(1)。

## DequeCHT::query

```cpp
T query(T x);
```

登録された有効な直線の最小値（MINIMIZE=falseでは最大値）を返します。

**計算量**

- ならしO(1)。

## DequeCHT::size

```cpp
int size() const;
```

現在保持する有効な直線の本数を返します。全登録回数ではありません。

**計算量**

- O(1)。

## DequeCHT::empty

```cpp
bool empty() const;
```

保持する直線が0本かを返します。

**計算量**

- O(1)。

## DequeCHT::clear

```cpp
void clear();
```

直線と順序管理の状態を消去します。

**計算量**

- O(H)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
DequeCHT<ll> c; c.add_line(2,0); c.add_line(-1,3);
assert(c.query(0)==0 && c.query(2)==1);
```

## メモリ

O(登録直線数)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
