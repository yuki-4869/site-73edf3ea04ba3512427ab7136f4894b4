# Mo法

[説明書の目次](../index.md) · [コード](../../../Search/mo.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Search/mo.cpp)

静的配列の区間クエリを並べ替え、区間の両端の追加・削除で答えを計算します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

Q=クエリ数、B=max(1,floor(N/max(1,sqrt(Q))))。add/removeは要素の添字を受け、outputは登録IDを受けます。

**共通の前提**

- 0<=l<=r<=N。すべてのクエリを登録後にrun。run開始時の外部管理状態は空区間[0,0)。

## 型・テンプレート引数

```cpp
struct Mo;
```

## コンストラクタ

```cpp
Mo();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
explicit Mo(int n);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(1)。

## Mo::add_query

```cpp
int add_query(int l, int r);
```

[l,r)のクエリを登録し、0-indexedのIDを返します。

**計算量**

- ならしO(1)。

## Mo::build

```cpp
void build();
```

ブロックと右端の交互順でクエリ順序を構築します。runからも呼ばれます。

**計算量**

- O(Q log Q)。

## Mo::run

```cpp
void run( AddLeft add_left, AddRight add_right, RemoveLeft remove_left, RemoveRight remove_right, Output output );
void run( Add add, Remove remove, Output output );
```

並べ替えた各クエリでoutput(id)を呼びます。5引数版は左右を別操作、3引数版は左右共通操作です。

**計算量**

- O(Q log Q+(N^2/B+QB)×端点更新コスト＋output総時間)。

## Mo::size

```cpp
int size() const;
```

登録クエリ数を返します。配列長ではありません。

**計算量**

- O(1)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
vl a={1,2,3}; Mo mo(3); mo.add_query(0,2); mo.add_query(1,3);
ll sum=0; vl ans(2);
mo.run([&](int i){sum+=a[i];},[&](int i){sum-=a[i];},[&](int id){ans[id]=sum;});
assert((ans==vl{3,5}));
```

## メモリ

O(Q)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
