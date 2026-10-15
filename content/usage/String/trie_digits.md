# Trie（数字）

[説明書の目次](../index.md) · [コード](../../../String/trie_digits.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/String/trie_digits.cpp)

重複登録できる文字列Trieです。完全一致の個数・接頭辞の個数を取得し、各ノードに任意の値を持たせられます。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

L=操作対象文字列の長さ、V=作成済みノード数。Node::cntは接頭辞の個数、endは完全一致の個数、valは利用者が使えるllの値。

**共通の前提**

- 文字は数字0..9。ノード番号vは0..node_count()-1。入力範囲外の文字の検証は行いません。コールバックは各接頭辞のノードに対して呼ばれます。

<details>
<summary>API一覧</summary>

- [Trie09::id](#api-1)
- [Trie09::call](#api-2)
- [Trie09::go](#api-3)
- [Trie09::next](#api-4)
- [Trie09::insert](#api-5)
- [Trie09::erase](#api-6)
- [Trie09::find](#api-7)
- [Trie09::count](#api-8)
- [Trie09::prefix_count](#api-9)
- [Trie09::contains](#api-10)
- [Trie09::starts_with](#api-11)
- [Trie09::walk](#api-12)
- [Trie09::operator[]](#api-13)
- [Trie09::size](#api-14)
- [Trie09::node_count](#api-15)
- [Trie09::empty](#api-16)
- [Trie09::reserve](#api-17)
- [Trie09::clear](#api-18)

</details>

## 型・テンプレート引数

```cpp
struct Trie09;
```

## この実装の注意点

goは不足ノードを作りますが文字列の登録個数を増やす操作ではありません。削除後もノードが残るため、find/nextで到達できてもその文字列が登録されているとは限りません。登録判定にはcontains/countを使います。insert/eraseのコールバックはcnt変更前に(node,depth,add)を受け取ります。walkは(node,depth)です。

<a id="api-1"></a>

## Trie09::id

```cpp
ll id(char c)const;
```

'0'～'9'を0～9に変換

**計算量**

- O(1)。

<a id="api-2"></a>

## Trie09::call

```cpp
void call(F& f,Node& n,ll d,bool add);
```

内部のコールバック呼び出し補助。通常は直接使用しません。

**計算量**

- コールバックの時間。

<a id="api-3"></a>

## Trie09::go

```cpp
ll go(ll v,char c);
```

vからcへ進む、なければ作成

**計算量**

- O(1)。

<a id="api-4"></a>

## Trie09::next

```cpp
ll next(ll v,char c)const;
```

vからcへ進む、現在存在しなければ-1

**計算量**

- O(1)。

<a id="api-5"></a>

## Trie09::insert

```cpp
ll insert(const string&s,F f);
ll insert(const string&s);
```

sを1個追加、終点ノードを返す

**計算量**

- O(|s|)。

<a id="api-6"></a>

## Trie09::erase

```cpp
bool erase(const string&s,F f);
bool erase(const string&s);
```

sを1個削除、なければfalse

**計算量**

- O(|s|)。

<a id="api-7"></a>

## Trie09::find

```cpp
ll find(const string&s)const;
```

sのノード番号、なければ-1

**計算量**

- O(|s|)。

<a id="api-8"></a>

## Trie09::count

```cpp
ll count(const string&s)const;
```

sそのものの登録個数

**計算量**

- O(|s|)。

<a id="api-9"></a>

## Trie09::prefix_count

```cpp
ll prefix_count(const string&s)const;
```

sをprefixに持つ文字列数

**計算量**

- O(|s|)。

<a id="api-10"></a>

## Trie09::contains

```cpp
bool contains(const string&s)const;
```

sが存在するか

**計算量**

- O(|s|)。

<a id="api-11"></a>

## Trie09::starts_with

```cpp
bool starts_with(const string&s)const;
```

sをprefixに持つ文字列が存在するか

**計算量**

- O(|s|)。

<a id="api-12"></a>

## Trie09::walk

```cpp
ll walk(const string&s,F f);
```

各prefixでf(node,depth)、辿れた長さを返す

**計算量**

- O(|s|)。

<a id="api-13"></a>

## Trie09::operator[]

```cpp
Node& operator[](ll v);
const Node& operator[](ll v)const;
```

ノードvへの参照を返します。const版ではconst参照です。

**計算量**

- O(1)。

<a id="api-14"></a>

## Trie09::size

```cpp
ll size()const;
```

重複込み登録文字列数

**計算量**

- O(1)。

<a id="api-15"></a>

## Trie09::node_count

```cpp
ll node_count()const;
```

作成済みノード数

**計算量**

- O(1)。

<a id="api-16"></a>

## Trie09::empty

```cpp
bool empty()const;
```

空ならtrue

**計算量**

- O(1)。

<a id="api-17"></a>

## Trie09::reserve

```cpp
void reserve(ll n);
```

ノード領域を予約

**計算量**

- O(現在ノード数)。

<a id="api-18"></a>

## Trie09::clear

```cpp
void clear();
```

全削除

**計算量**

- O(ノード数)。

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `t[v]` | ノードv |
| `Node::nx` | 文字ごとの子ノード番号、なければ-1 |
| `Node::cnt` | この接頭辞を持つ登録文字列数 |
| `Node::end` | このノードで終わる文字列数 |
| `Node::val` | 任意の補助集計 |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
Trie09 t; t.insert("4869"); t.insert("4869");
assert(t.count("4869")==2);
assert(t.prefix_count("4")==2);
assert(t.erase("4869") && t.count("4869")==1);
```

## メモリ

O(SIG×V)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
