# 行列累乗

固定した遷移を $k$ 回適用します。mod の歩道数、辺数を指定した最小コスト、その歩数での到達可否を同じ呼び出し方で求められます。
添字は 0 始まりです。ベクトルは列ベクトルとして扱います。

## 行列の種類

| 型 | 足し算 | 掛け算 | 零元 | 乗法の単位元 |
| --- | --- | --- | --- | --- |
| `MatrixMod<Mod>` | mod 加算 | mod 乗算 | `0` | `1` |
| `MatrixMinPlus` | `min` | 加算 | `MatrixMinPlus::zero()`（未到達） | `0` |
| `MatrixBool` | OR | AND | `0` | `1` |

- `MatrixMod<>` の既定値は `998244353`。`MatrixMod<1000000007>` も使えます。`2 <= Mod <= 2^30` で、合成数 mod にも対応します。`A[i][j]` に代入する値と、ベクトルの値は $[0,\mathrm{Mod})$ に正規化してください。符号付き整数を行列に代入するには `set` を使います。
- `MatrixMinPlus` の値型は `long long` です。未到達値は `LLONG_MAX` で、負の辺重みも使えます。途中で実際に加える有限値の和は `long long` に収まり、かつ `LLONG_MAX` 未満である必要があります。平行辺は最小の重みを入れます。min-plus の単位行列は対角が `0`、それ以外が未到達です。
- `MatrixBool` の行列・ベクトルは `uint8_t` の `0` と `1` を使います。
- 独自の `SemiringMatrix<Ops>` には `Ops::value_type`、`zero()`、`one()`、`add(a,b)`、`mul(a,b)` を定義します。行列の積や累乗が成立する半環の演算を指定し、値型には `bool` 以外で等値比較できる型を使います。

## API と計算量

$h,w,d$ は行列の次元、$N$ は正方行列の一辺です。$T(N)$ は正方行列積 1 回の時間、$L=\operatorname{bit\_width}(k)$、$q=\operatorname{popcount}(k)$ とします。表の計算量は次元が正で、要素演算が定数時間の場合です。

| 操作 | 戻り値と処理 | 計算量 |
| --- | --- | --- |
| `M A(h, w)` | $h\times w$ の零行列を構築 | $O(hw)$ |
| `A.rows()`, `A.cols()` | `size_type` の行数、列数 | $O(1)$ |
| `A[i]` | `value_type*` または `const value_type*`。`A[i][j]` で要素を参照 | $O(1)$ |
| `A.set(i, j, x)` | `void`。符号付き 64 bit 整数 `x` を正規化して代入。`MatrixMod` 専用 | $O(1)$ |
| `M::mod()` | `value_type` の法。`MatrixMod` 専用 | $O(1)$ |
| `M::zero()`, `M::one()` | `value_type` の加法・乗法の単位元。`SemiringMatrix` 系で使える | $O(1)$ |
| `M::identity(N)` | $N$ 次単位行列 | $O(N^2)$ |
| `A + B` | 同じ形の行列の要素ごとの和 | $O(hw)$ |
| `A * B` | $h\times d$ 行列と $d\times w$ 行列の積 | $O(hdw)$。大きな mod 正方行列は $O(N^{\log_2 7})$ |
| `A.mul_classic(B)` | 常に通常の方法で積を計算。`MatrixMod` 専用 | $O(hdw)$ |
| `A.apply(v)` | `vector<value_type>` で $Av$。`v.size() == A.cols()` | $O(hw)$ |
| `A.pow(k)` | $A^k$ | $O(N^2+(L+q)T(N))$ |
| `A.pow_apply(k, v)` | `vector<value_type>` で $A^kv$ | $O(N+\max(0,L-1)T(N)+qN^2)$ |
| `A.pow_sum(k)` | `pair<M, M>` で $\bigl(A^k,\sum_{0\le i<k}A^i\bigr)$ | $O(N^2+(L+q)T(N))$ |

累乗の対象は正方行列で、指数は `uint64_t` です。`pow(0)` は単位行列、`pow_apply(0,v)` は `v`、`pow_sum(0)` は単位行列と零行列を返します。
`pow_sum` の第2要素は、min-plus では長さ $0$ から $k-1$ まででの最小コスト、真偽値ではそのいずれかの長さで到達可能かを表します。$k=0$ の和は空です。

`MatrixMod` は右行列をパネル単位で転置し、剰余演算をまとめます。$N\ge 288$ の正方行列同士では Strassen 法を使います。しきい値は実装上の設定で、実行環境によって通常積との速度差は変わります。累乗中の行列用メモリは $O(N^2)$ です。

## 同じ行列への複数クエリ

`MatrixPower powers(A, max_k)` は、$B=\operatorname{bit\_width}(\text{max\_k})$ 個の $A^{2^b}$ を保持します。
構築には $O(N^2+\max(0,B-1)T(N))$ 時間、保存には $O(1+BN^2)$ メモリを使います。クエリは $0\le k\le\text{max\_k}$ の範囲です。

| 操作 | 戻り値 | 1クエリの計算量 |
| --- | --- | --- |
| `powers.pow(k)` | $A^k$ | $O(N^2+\max(0,q-1)T(N))$ |
| `powers.pow_apply(k, v)` | `vector<value_type>` で $A^k v$ | $O(N+qN^2)$ |

## 使用例

フィボナッチ数 $F_{10}$、$F_0+\cdots+F_9$ と、辺数を指定した最小コスト・到達可否を求めます。

```cpp
MatrixMod<> A(2, 2);
A[0][0] = A[0][1] = A[1][0] = 1;
auto fib = A.pow_apply(10, {0, 1});
auto [power, sum] = A.pow_sum(10);
MatrixPower powers(A, 1000);
assert(fib[0] == 55 && power[0][1] == 55 && sum[0][1] == 88);
assert(powers.pow_apply(10, {0, 1}) == fib);

MatrixMinPlus cost(3, 3);
cost[0][1] = 4;
cost[1][2] = -1;
auto exact = cost.pow(2);
assert(exact[0][2] == 3 && exact[2][0] == MatrixMinPlus::zero());

MatrixBool edges(3, 3);
edges[0][1] = edges[1][2] = 1;
auto reachable = edges.pow(2);
assert(reachable[0][2] && !reachable[0][1]);
```

## 行列の1要素だけ欲しい場合

素数 mod で $(A^k)_{s,t}$ だけ求めるときは、単位ベクトル $e_t$ から始めて $x\gets Ax$ を繰り返し、毎回の $x_s$ から $2N$ 項を作れます。この列の最短漸化式の次数は高々 $N$ です。[Berlekamp–Massey・Bostan–Mori](../FPSandPolynomial/bostan_mori.md) の `berlekamp_massey` で係数を作り、`linear_recurrence` に同じ長さの先頭項と係数を渡すと第 $k$ 項を得られます。疎な遷移では辺リストで $Ax$ を計算すると、項生成の計算量を $O(N(N+M))$ にできます。ここで $M$ は辺数です。
