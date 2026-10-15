# Implicit Treap

[説明書の目次](../index.md) · [コード](../../../DataStructureAdvanced/implicit_treap.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/DataStructureAdvanced/implicit_treap.cpp)

要素の位置をキーとするTreapで、列の挿入・削除・区間加算・反転・和を管理します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=現在の列長。Treapのランダム優先度に対する期待計算量を記載します。

**共通の前提**

- 位置は0-indexed。区間は0<=l<=r<=N。insertは0<=pos<=N、get/set/eraseは0<=pos<N。llの値・和・加算がオーバーフローしないこと。

<details>
<summary>API一覧</summary>

- [ImplicitTreap::size](#api-1)
- [ImplicitTreap::insert](#api-2)
- [ImplicitTreap::push_back](#api-3)
- [ImplicitTreap::erase](#api-4)
- [ImplicitTreap::get](#api-5)
- [ImplicitTreap::set_val](#api-6)
- [ImplicitTreap::range_add](#api-7)
- [ImplicitTreap::range_reverse](#api-8)
- [ImplicitTreap::range_sum](#api-9)
- [ImplicitTreap::to_vector](#api-10)
- [ImplicitTreap::build](#api-11)

</details>

## 型・テンプレート引数

```cpp
struct ImplicitTreap;
```

## この実装の注意点

固定初期値のxorshiftによる優先度を使います。クラスには所有権を管理するコピー／デストラクタがなく、コピーするとrootを共有します。buildで以前のrootを上書きした際のノード回収もありません。

<a id="api-1"></a>

## ImplicitTreap::size

```cpp
int size() const;
```

現在の要素数を返します。引数なしの版を使用します。

**計算量**

- O(1)。

<a id="api-2"></a>

## ImplicitTreap::insert

```cpp
void insert(int pos, ll x);
```

posの直前へ値xを挿入します。

**計算量**

- 期待O(log(N+1))。

<a id="api-3"></a>

## ImplicitTreap::push_back

```cpp
void push_back(ll x);
```

末尾にxを追加します。

**計算量**

- 期待O(log(N+1))。

<a id="api-4"></a>

## ImplicitTreap::erase

```cpp
void erase(int pos);
```

pos番目の要素を削除します。

**計算量**

- 期待O(log N)。

<a id="api-5"></a>

## ImplicitTreap::get

```cpp
ll get(int pos);
```

pos番目の値を返します。

**計算量**

- 期待O(log N)。

<a id="api-6"></a>

## ImplicitTreap::set_val

```cpp
void set_val(int pos, ll x);
```

pos番目の値をxへ代入します。

**計算量**

- 期待O(log N)。

<a id="api-7"></a>

## ImplicitTreap::range_add

```cpp
void range_add(int l, int r, ll x);
```

[l,r)の全要素へxを加算します。

**計算量**

- 期待O(log N)。

<a id="api-8"></a>

## ImplicitTreap::range_reverse

```cpp
void range_reverse(int l, int r);
```

[l,r)の順序を反転します。

**計算量**

- 期待O(log N)。

<a id="api-9"></a>

## ImplicitTreap::range_sum

```cpp
ll range_sum(int l, int r);
```

[l,r)の要素の和を返します。空区間は0。

**計算量**

- 期待O(log N)。

<a id="api-10"></a>

## ImplicitTreap::to_vector

```cpp
vector<ll> to_vector();
```

現在の列を順番にvector<ll>へ取り出します。

**計算量**

- O(N)。

<a id="api-11"></a>

## ImplicitTreap::build

```cpp
void build(const vector<ll>& v);
```

vから列を構築します。各要素を順に挿入します。

**計算量**

- 期待O(N log(N+1))。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
ImplicitTreap t; t.build({1,2,3});
t.range_add(0,2,5); t.range_reverse(0,3);
assert((t.to_vector()==vl{3,7,6}));
t.insert(1,10); assert(t.range_sum(0,4)==26);
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
