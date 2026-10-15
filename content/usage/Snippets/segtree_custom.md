# ACL segtree の穴埋めテンプレート

[説明書の目次](../index.md) · [コード](../../../Snippets/segtree_custom.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Snippets/segtree_custom.cpp)

ACL segtree用のモノイドS・op・eを記入する雛形です。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

S=1要素・区間を表す情報、op(S,S)=順序を保つ結合、e()=空区間。

**共通の前提**

- S,op,eを完成させます。opは結合則を満たしeは両側単位元。0<=l<=r<=N。

## この実装の注意点

元ファイルは記入用です。各関数の空欄を埋める前は実用的な集約を行いません。下の例では区間和として完成させた場合を示します。

## SegXXX::op

```cpp
S op(S a,S b);
```

2つの隣接区間の情報を結合します。未記入のため、問題に合わせて実装します。

**計算量**

- 利用者の実装次第。

## SegXXX::e

```cpp
S e();
```

空区間の情報を返します。未記入のため、問題に合わせて実装します。

**計算量**

- 利用者の実装次第。

## 使用例

共通設定の後に置く、型・演算・mainを含む完成例です。`assert` は期待する結果を確認しています。

```cpp
namespace SegXXX {
    struct S { ll val; };
    S op(S a,S b){return {a.val+b.val};}
    S e(){return {0};}
    using Seg=atcoder::segtree<S,op,e>;
}
int main(){
    SegXXX::Seg seg(vector<SegXXX::S>{{1},{2},{3}});
    assert(seg.prod(0,3).val==6);
    seg.set(1,{10}); assert(seg.prod(0,3).val==14);
}
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

検証の際の準備:

- 未記入の型・演算を、掲載した完成例として定義

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
