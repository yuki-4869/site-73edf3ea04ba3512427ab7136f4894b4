# 二分探索の穴埋めテンプレート

[説明書の目次](../index.md) · [コード](../../../Snippets/binary_search.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Snippets/binary_search.cpp)

判定が単調な整数区間で、false側ngとtrue側okの境界を二分探索する記入用テンプレートです。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

ok,ngは境界を挟む整数。checkは候補の可否をboolで返す関数。

**共通の前提**

- このスニペットのwhile条件はok>ng向け。check(ng)=false、check(ok)=trueで、ngからokへfalse→trueとなるように設定。C++20が必要。

## この実装の注意点

元コードはok/ngとreturnの式が未記入です。ok<ngの向きで使用するなら終了条件も変える必要があります。std::midpointは和のオーバーフローを避けますが、ok-ng自体がllを超えないようにしてください。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
ll ok=100,ng=-1;
auto check=[](ll x){return x>=37;};
while(ok-ng>1){ll mid=std::midpoint(ok,ng); if(check(mid))ok=mid;else ng=mid;}
assert(ok==37);
```

## メモリ

O(1)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

検証の際の準備:

- ok/ng/checkを掲載例の値・判定で記入

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
