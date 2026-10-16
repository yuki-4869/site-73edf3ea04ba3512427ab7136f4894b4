# 動的連結性（オンライン）

[説明書の目次](../index.md) · [コード](../../../DataStructureAdvanced/dynamic_connectivity.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/DataStructureAdvanced/dynamic_connectivity.cpp)

オンラインで無向辺を追加・削除し、連結判定と成分サイズ・成分和を管理します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

L=内部で保持するレベル数。link/cutのboolは辺の追加・削除自体の成功とは異なります。

**共通の前提**

- 0<=s,t<N。同一無向辺の重複追加はしないでください。Sum型での利用を想定し、値と和はlong longに収めます。

<details>
<summary>API一覧</summary>

- [コンストラクタ](#api-1)
- [Sum::et](#api-2)
- [Sum::fn](#api-3)
- [dynamic_connectivity::link](#api-4)
- [dynamic_connectivity::cut](#api-5)
- [dynamic_connectivity::same](#api-6)
- [dynamic_connectivity::size](#api-7)
- [dynamic_connectivity::update](#api-8)
- [dynamic_connectivity::get_sum](#api-9)
- [dynamic_connectivity::get_val](#api-10)

</details>

## 型・テンプレート引数

```cpp
struct Sum;
template<typename T>
class dynamic_connectivity;
```

## この実装の注意点

テンプレート型Tでも公開APIはlong longとメンバvalを使用するため、任意のモノイドへ差し替えるだけでは使えません。cutの厳密な償却計算量について、この実装固有の証明は添付していません。 コメント外の全角空白は除去済みで、基本テンプレートの後にそのまま配置できます。

<a id="api-1"></a>

## コンストラクタ

```cpp
Sum();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
Sum(long long v);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(1)。

```cpp
dynamic_connectivity(int n);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N log(N+1))の上界。

<a id="api-2"></a>

## Sum::et

```cpp
static Sum et();
```

Sumの単位元0を返します。

**計算量**

- O(1)。

<a id="api-3"></a>

## Sum::fn

```cpp
static Sum fn(const Sum& a, const Sum& b);
```

Sumの値を加算して返します。

**計算量**

- O(1)。

<a id="api-4"></a>

## dynamic_connectivity::link

```cpp
bool link(int s, int t);
```

辺s-tを追加します。追加によって2成分が連結された場合true。既に連結でも辺は登録しますがfalse。自己ループは登録せずfalse。

**計算量**

- ならしO(log N)＋hash集合操作。

<a id="api-5"></a>

## dynamic_connectivity::cut

```cpp
bool cut(int s, int t);
```

辺s-tを削除します。削除後に成分が分かれた場合true、代替辺で連結を維持する場合や辺なしではfalse。

**計算量**

- 探索した頂点・辺数とレベル数に依存。単発O(log N)は保証しません。

<a id="api-6"></a>

## dynamic_connectivity::same

```cpp
bool same(int s, int t);
```

s,tが同じ連結成分かを返します。

**計算量**

- ならしO(log N)。

<a id="api-7"></a>

## dynamic_connectivity::size

```cpp
int size(int s);
```

sを含む成分の頂点数を返します。

**計算量**

- ならしO(log N)。

<a id="api-8"></a>

## dynamic_connectivity::update

```cpp
void update(int s, long long x);
```

頂点sの値をxに代入し、全レベルで更新します。

**計算量**

- ならしO(L log N)。

<a id="api-9"></a>

## dynamic_connectivity::get_sum

```cpp
long long get_sum(int s);
```

sを含む成分の頂点値の和をlong longで返します。

**計算量**

- ならしO(log N)。

<a id="api-10"></a>

## dynamic_connectivity::get_val

```cpp
long long get_val(int s);
```

頂点s自身の値をlong longで返します。

**計算量**

- ならしO(log N)。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
dynamic_connectivity<Sum> dc(3);
dc.update(0,2); dc.update(1,5);
assert(dc.link(0,1) && dc.get_sum(0)==7);
assert(dc.same(0,1)); assert(dc.cut(0,1)); assert(!dc.same(0,1));
```

## メモリ

O(NL+内部で保持する辺・木ノード)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

検証の際の準備:

- 基本テンプレートの後に実装を配置

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
