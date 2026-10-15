# 回文半径

[説明書の目次](../index.md) · [コード](../../../String/palindrome_radius.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/String/palindrome_radius.cpp)

Manacher法で、各文字を中心とする奇数長回文の半径を線形時間で計算します。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

N=|S|。R[i]は中心文字自身も含む半径で、回文区間は[i-R[i]+1,i+R[i])、長さは2*R[i]-1です。

**共通の前提**

- Sは処理対象の文字列。コードは関数の中に貼り付けます。空文字列ではRは空です。

## この実装の注意点

Rolling Hashのpal_radius_oddが返す半径dとは1違い、このRはd+1です。偶数長回文はこのスニペットでは求めません。

## 使用例

必要な定義を用意して、次の処理を `main` 内で実行します。`assert` で結果を確認しています。

```cpp
string S="abacaba";
// 元コードのvector<ll> R(...)からwhileループ末尾までをここへ貼り付けます。
vector<ll> R(sz(S));//それぞれの文字を中心とした回文半径
  ll I = 0, J = 0;
  while (I < sz(S)) {
    while (I-J >= 0 && I+J < sz(S) && S[I-J] == S[I+J]) ++J;
      R[I] = J;
      int K = 1;
    while (I-K >= 0 && K+R[I-K] < J) R[I+K] = R[I-K], ++K;
    I += K; J -= K;
  }
assert(R[3]==4 && R[0]==1);
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

検証の際の準備:

- 入力文字列Sを掲載例に設定

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
