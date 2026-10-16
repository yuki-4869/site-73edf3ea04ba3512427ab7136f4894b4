# Cartesian Tree

[説明書の目次](../index.md) · [コード](../../../Tree/cartesian_tree.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Tree/cartesian_tree.cpp)

配列の添字順を中間順序とし、親の値が子以下であるCartesian Treeを構築します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=配列長。等しい値は左側が親になる比較です。

**共通の前提**

- Tは比較可能。通常はN>=1。N=0ではroot=0でも有効な頂点ではありません。

## 型・テンプレート引数

```cpp
template<class T>
struct CartesianTree;
```

## この実装の注意点

使用例は末尾のコメント内にまとめています。クラス定義をグローバルに、呼び出し例をmain内に置いてください。

## コンストラクタ

```cpp
CartesianTree();
```

空の状態で構築します。

**計算量**

- O(1)。

```cpp
CartesianTree(const vector<T>& v);
```

指定したサイズ・初期データ・演算でオブジェクトを構築します。

**計算量**

- O(N)。

## 取得できる情報

| 名前 | 内容 |
| --- | --- |
| `root` | 根の添字 |
| `par[v]` | 親、根は-1 |
| `left[v],right[v]` | 左・右の子、存在しなければ-1 |

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
vl v={3,1,2}; CartesianTree<ll> t(v);
assert(t.root==1 && t.par[0]==1 && t.par[2]==1);
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

検証の際の準備:

- 基本テンプレートの後にクラス定義を配置

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
