# レーベンシュタイン距離

文字列・数列 $a$ を $b$ に変えるために必要な最小操作回数を求めます。1 回の操作で、1 要素の挿入・削除・置換のいずれかを行えます。

[共通設定](../common.md) の基本テンプレートに実装を貼り付けて使います。追加の ACL ヘッダーは不要です。

## levenshtein_distance

```cpp
levenshtein_distance(a, b);  // std::size_t
```

- `std::string`、`std::string_view`、文字列リテラル、`std::vector<T>`、`std::array<T, N>` などを渡せます。
- 文字列リテラルは `std::string_view` として扱います。汎用版では両方の列が `size()` と `operator[]` を持ち、要素を両方向から `==` で等値比較できる必要があります。入力は変更しません。
- `std::string` に格納した UTF-8 はバイト単位で比較します。Unicode コードポイント単位の距離は、デコード済みの `std::u32string` を渡して求められます。
- 両方とも空なら `0`、片方だけ空ならもう片方の長さを返します。

**計算量**　両方が非空で、各要素の参照と比較が定数時間の場合、長さを $N,M$ として $O(NM)$ 時間、追加領域 $O(\min(N,M))$ です。長さが分かっている列は、片方が空なら $O(1)$ 時間・領域で返します。文字列リテラルから `std::string_view` を作る際は、長さの取得にその文字列長に比例する時間がかかります。

## アルゴリズム

$D_{i,j}$ を $a$ の先頭 $i$ 要素から $b$ の先頭 $j$ 要素への編集距離とします。境界は $D_{i,0}=i$、$D_{0,j}=j$ です。最後の要素を削除する、末尾に挿入する、末尾同士を対応させる、の 3 通りで更新します。

$$
D_{i,j}=\min\bigl(D_{i-1,j}+1,\ D_{i,j-1}+1,\ D_{i-1,j-1}+[a_{i-1}\ne b_{j-1}]\bigr).
$$

短い方の列を列方向に取り、DP の 1 行を左から上書きします。左上の値を変数に退避すれば、各更新に必要な 3 つの値がそろいます。

## 使用例

```cpp
assert(levenshtein_distance("kitten", "sitting") == 3);
std::string s = "abc", t = "axc";
assert(levenshtein_distance(s, t) == 1);

vl a = {1, 2, 3}, b = {1, 4, 3, 5};
assert(levenshtein_distance(a, b) == 2);
assert(levenshtein_distance(vl{}, b) == 4);
```

## 練習問題

[AtCoder ABC185 E - Sequence Matching](https://atcoder.jp/contests/abc185/tasks/abc185_e) では、$A$ 側から取り除く要素を削除、$B$ 側から取り除く要素を挿入、対応する要素の不一致を置換に対応させます。`levenshtein_distance(A, B)` が答えです。
