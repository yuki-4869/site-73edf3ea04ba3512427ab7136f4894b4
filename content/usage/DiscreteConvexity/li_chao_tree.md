# Li Chao Tree

[説明書の目次](../index.md) · [コード](../../../DiscreteConvexity/li_chao_tree.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/DiscreteConvexity/li_chao_tree.cpp)

複数の一次関数について、指定したxでの最小値／最大値を管理します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

MINIMIZE=trueが最小値、falseが最大値。C=R-L+1、l,rは閉区間の座標。

**共通の前提**

- 整数xの定義域は両端を含む[L,R]、L<=R。queryのxもその範囲。a*x+bと内部の座標差がTに収まること。

## 型・テンプレート引数

```cpp
template<class T = long long, bool MINIMIZE = true>
struct LiChaoTree;
```

## コンストラクタ

```cpp
LiChaoTree();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
LiChaoTree(T x_left, T x_right);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(1)。

## LiChaoTree::add_line

```cpp
void add_line(T a, T b);
void add_line(Line line);
```

直線y=a*x+bを登録します。同じ傾きでは目的に応じて優れた切片を残します。

**計算量**

- O(log C)。

## LiChaoTree::add_segment

```cpp
void add_segment( T a, T b, T l, T r );
void add_segment( Line line, T l, T r );
```

直線を閉区間[l,r]上だけに追加します。

**制約**

- l<=r。初期定義域との共通部分に有効

**計算量**

- O(log^2 C)。

## LiChaoTree::query

```cpp
T query(T x) const;
```

登録された有効な直線の最小値（MINIMIZE=falseでは最大値）を返します。有効な直線がなければmin版はnumeric_limits<T>::max、max版はlowest。

**計算量**

- O(log C)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
LiChaoTree<ll> c(-10,10); c.add_line(2,0); c.add_line(-1,3);
assert(c.query(2)==1);
```

## メモリ

追加したノード数に比例。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
