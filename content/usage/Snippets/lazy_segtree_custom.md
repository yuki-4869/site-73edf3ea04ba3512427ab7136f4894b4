# ACL lazy_segtree の穴埋めテンプレート

[説明書の目次](../index.md) · [コード](../../../Snippets/lazy_segtree_custom.cpp) · [確認対象の公開版](https://github.com/yuki-4869/Algorithm/blob/933b86ab8c6dde6d09fbe6328e4778a780264ed3/Snippets/lazy_segtree_custom.cpp)

ACL lazy_segtree用のモノイドと写像を記入する雛形です。

型エイリアス・マクロとコードの置き方は [共通設定](../common.md) を参照してください。

**記号**

S=集約情報、F=写像、mp=写像の作用、cp=写像の合成、id=恒等写像。

**共通の前提**

- LazySegBaseを先に定義します。opは結合則、eは単位元、mp(id(),x)=x、mp(cp(f,g),x)=mp(f,mp(g,x))、mp(f,op(x,y))=op(mp(f,x),mp(f,y))を満たすこと。

## 型・テンプレート引数

```cpp
struct LazyXXX;
```

## この実装の注意点

元ファイルは空欄があり、そのままでは使えません。下の例は区間加算・区間和として完成させた場合です。

## 使用例

共通設定の後に置く、型・演算・mainを含む完成例です。`assert` は期待する結果を確認しています。

```cpp
template<class S,S(*op)(S,S),S(*e)(),class F,S(*mp)(F,S),F(*cp)(F,F),F(*id)()>
struct LazySegBase:atcoder::lazy_segtree<S,op,e,F,mp,cp,id>{
    using B=atcoder::lazy_segtree<S,op,e,F,mp,cp,id>;
    using B::B;
};
namespace LazyXXX_impl {
    struct S { ll sum,size; };
    struct F { ll add; };
    S op(S a,S b){return {a.sum+b.sum,a.size+b.size};}
    S e(){return {0,0};}
    S mp(F f,S x){return {x.sum+f.add*x.size,x.size};}
    F cp(F f,F g){return {f.add+g.add};}
    F id(){return {0};}
}
struct LazyXXX:LazySegBase<LazyXXX_impl::S,LazyXXX_impl::op,LazyXXX_impl::e,
    LazyXXX_impl::F,LazyXXX_impl::mp,LazyXXX_impl::cp,LazyXXX_impl::id>{
    using S=LazyXXX_impl::S;
    using B=LazySegBase<S,LazyXXX_impl::op,LazyXXX_impl::e,
        LazyXXX_impl::F,LazyXXX_impl::mp,LazyXXX_impl::cp,LazyXXX_impl::id>;
    using B::B;
};
int main(){
    LazyXXX seg(vector<LazyXXX::S>{{1,1},{2,1},{3,1}});
    seg.apply(0,2,{5}); assert(seg.prod(0,3).sum==16);
}
```

## メモリ

O(N)。

## 使用例の確認状況

GCC 13.3.0、`-std=c++23` とACLで、掲載例のコンパイル・実行および `assert` の成功を確認しました。

検証の際の準備:

- 未記入の型・演算を、掲載した完成例として定義

元コードの未修正時の結果・検証条件は [C++23での確認結果](../validation.md) を参照してください。
