# 偏角ソート

[説明書の目次](../index.md) · [コード](../../../Geometry/argument_sort.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Geometry/argument_sort.cpp)

点を原点から見た偏角順に並べます。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=点数。正のx軸から反時計回りの順。負のx軸は後半に入ります。

**共通の前提**

- 外積・二乗距離の積と和がlong longに収まる整数座標。原点はすべての非原点より前になります。

## 型・テンプレート引数

```cpp
struct point;
```

## cross

```cpp
long long cross(const point &a, const point &b);
```

ベクトルa,bの外積a.x*b.y-a.y*b.xを返します。

**計算量**

- O(1)。

## cmp

```cpp
bool cmp(const point &a, const point &b);
```

偏角順の比較。偏角が同じなら二乗距離が短い方を先にします。

**計算量**

- O(1)。

## argument_sort

```cpp
void argument_sort(vector<point> &points);
```

pointsをその場でソートします。返り値なし。

**計算量**

- O(N log N)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
vector<point> a={{0,-1},{-1,0},{0,1},{1,0}};
argument_sort(a);
assert(a[0].x==1 && a[1].y==1 && a[2].x==-1 && a[3].y==-1);
```

## メモリ

ソートの追加領域O(log N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
