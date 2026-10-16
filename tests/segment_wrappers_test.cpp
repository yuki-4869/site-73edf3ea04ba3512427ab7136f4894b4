#include <atcoder/lazysegtree>
#include <atcoder/modint>
#include <bits/stdc++.h>
#include "silent-check.hpp"
#include "DataStructureSegment/lazy_segtree_wrappers.cpp"
#include "DataStructureSegment/segment_tree_beats_extra.cpp"
#include <cstdlib>
#include <iostream>
#include <random>
#include <tuple>

using ll=long long;
using V=std::vector<ll>;
using Mint=atcoder::modint998244353;
void check(bool ok,int line){if(!ok){std::cerr<<"Check failed at line "<<line<<'\n';std::abort();}}
#define CHECK(...) check(static_cast<bool>((__VA_ARGS__)),__LINE__)
template<class Error,class F>void throws(F f){CHECK(stops_silently(f));}
struct Stats{ll sum=0,mn=LSEG_INF,mx=-LSEG_INF;int argmin=-1,argmax=-1;};
Stats scan(const V& a,int l,int r){Stats s;for(int i=l;i<r;++i){s.sum+=a[i];if(a[i]<s.mn){s.mn=a[i];s.argmin=i;}if(a[i]>s.mx){s.mx=a[i];s.argmax=i;}}return s;}
bool accepts_any_ll(ll){return true;}

template<class Tree>void wrapper_state(){
    Tree zero,empty(0),sized(3);
    CHECK(zero.empty()&&empty.empty()&&sized.size()==3);
    (void)zero.prod(0,0);(void)zero.all_prod();
    throws<std::invalid_argument>([]{Tree bad(-1);});
    throws<std::out_of_range>([&]{zero.get(0);});
    throws<std::out_of_range>([&]{sized.prod(-1,1);});
    throws<std::out_of_range>([&]{sized.prod(2,1);});
    throws<std::out_of_range>([&]{sized.prod(0,4);});
    CHECK(sized.max_right(0,[](auto){return true;})==3);
    CHECK(sized.min_left(3,[](auto){return true;})==0);
    throws<std::invalid_argument>([&]{sized.max_right(0,[](auto){return false;});});
    Tree copied=sized,moved=std::move(copied);
    CHECK(copied.empty()&&copied.size()==0&&moved.size()==3);
    (void)copied.prod(0,0);(void)copied.all_prod();
    CHECK(copied.max_right(0,[](auto){return true;})==0);
    CHECK(copied.min_left(0,[](auto){return true;})==0);
    copied=std::move(moved);CHECK(moved.empty()&&copied.size()==3);
    copied=empty;CHECK(copied.empty());
}

void regressions(){
    wrapper_state<LazyAddMin>();wrapper_state<LazyAddMax>();wrapper_state<LazyAddSum>();
    wrapper_state<LazyAssignMin>();wrapper_state<LazyAssignMax>();wrapper_state<LazyAssignSum>();
    wrapper_state<LazyAddMinMax>();wrapper_state<LazyAssignMinMax>();
    wrapper_state<LazyAddArgMax>();wrapper_state<LazyAddArgMin>();
    wrapper_state<LazyChminMax>();wrapper_state<LazyChmaxMin>();
    wrapper_state<LazyAffineStats>();wrapper_state<LazyAffineSum>();
    wrapper_state<LazyFlipSum>();wrapper_state<LazyFlipInv>();wrapper_state<LazyAPAddSum>();
    LazyAddMin mn(3);LazyAddMax mx(3);LazyAddSum sum(3);
    CHECK(mn.get(0)==0&&mx.get(1)==0&&sum.get(2).size==1&&sum.sum(0,3)==0);
    LazyAssignMinMax mm(3);CHECK(mm.prod(0,3).mn==0&&mm.prod(0,3).mx==0);
    LazyAddArgMin amin(3);LazyAddArgMax amax(3);CHECK(amin.get(2).id==2&&amax.get(1).id==1);
    CHECK(LazyAddMin_impl::mp(123,LazyAddMin_impl::e())==LazyAddMin_impl::e());
    CHECK(LazyAddMax_impl::mp(-123,LazyAddMax_impl::e())==LazyAddMax_impl::e());
    CHECK(LazyAssignMin_impl::mp({2,true},LazyAssignMin_impl::e())==LazyAssignMin_impl::e());
    CHECK(LazyAssignMax_impl::mp({2,true},LazyAssignMax_impl::e())==LazyAssignMax_impl::e());
    auto e=LazyMinMax_impl::e();auto x=LazyAddMinMax_impl::mp(2,e);
    CHECK(x.mx==e.mx&&x.mn==e.mn);x=LazyAssignMinMax_impl::mp({2,true},e);CHECK(x.mx==e.mx&&x.mn==e.mn);
    CHECK(mx.max_right<accepts_any_ll>(0)==3&&mx.min_left<accepts_any_ll>(3)==0);
    LazyAffineStats stats(V{2,5,3});stats.affine(0,3,-2,1);
    CHECK(stats.min(0,3)==-9&&stats.max(0,3)==-3&&stats.sum(0,3)==-17);
    LazyAffineSumT<atcoder::modint1000000007> other_mod(V{1,2,3});
    other_mod.affine(0,3,2,3);CHECK(other_mod.sum(0,3).val()==21);
    other_mod.set(1,10);CHECK(other_mod.sum(0,3).val()==24);
    LazyAssignSum assign(V{1,2,3});assign.apply(1,10LL);CHECK(assign.sum(0,3)==14);
    LazyAffineSum affine(V{1,2});affine.apply(0,2,LazyAffineSum::F{2,3});CHECK(affine.sum(0,2).val()==12);
    LazyAPAddSum ap(4);ap.add_ap(1,4,10,3);CHECK(ap.sum(0,4)==39&&ap.get(3).sum==16);
    throws<std::length_error>([]{LazyAddSum huge((1<<29)+1);});
}

void add_random(std::mt19937_64& rng,int steps){
    const int n=1+static_cast<int>(rng()%32);V a(n);for(ll&x:a)x=static_cast<ll>(rng()%31)-15;
    LazyAddMin mn(a);LazyAddMax mx(a);LazyAddSum sum(a);LazyAddMinMax mm(a);LazyAddArgMin amin(a);LazyAddArgMax amax(a);
    for(int t=0;t<steps;++t){
        int l=static_cast<int>(rng()%(n+1)),r=static_cast<int>(rng()%(n+1));if(l>r)std::swap(l,r);
        if(rng()%4!=0){const ll x=static_cast<ll>(rng()%21)-10;for(int i=l;i<r;++i)a[i]+=x;
            mn.add(l,r,x);mx.apply(l,r,x);sum.add(l,r,x);mm.add(l,r,x);amin.add(l,r,x);amax.add(l,r,x);
        }else{const int i=static_cast<int>(rng()%n);const ll x=static_cast<ll>(rng()%101)-50;a[i]=x;
            mn.set(i,x);mx.set(i,x);sum.set(i,x);mm.set(i,x);amin.set(i,x);amax.set(i,x);}
        auto s=scan(a,l,r);CHECK(mn.prod(l,r)==s.mn&&mx.max(l,r)==s.mx&&sum.sum(l,r)==s.sum);
        auto m=mm.prod(l,r);CHECK(m.mn==s.mn&&m.mx==s.mx);
        auto u=amin.prod(l,r);auto v=amax.prod(l,r);CHECK(u.val==s.mn&&v.val==s.mx);
        CHECK(u.id==(l==r?LSEG_INF:s.argmin)&&v.id==(l==r?LSEG_INF:s.argmax));
        const ll limit=static_cast<ll>(rng()%101)-50;int end=l;while(end<n&&a[end]<=limit)++end;
        CHECK(mx.max_right(l,[&](ll value){return value<=limit;})==end);
        int begin=r;while(begin>0&&a[begin-1]<=limit)--begin;
        CHECK(mx.min_left(r,[&](ll value){return value<=limit;})==begin);
        CHECK(sum.all_prod().val==scan(a,0,n).sum);
    }
}
void assign_random(std::mt19937_64& rng,int steps){
    const int n=1+static_cast<int>(rng()%32);V a(n,1);
    LazyAssignMin mn(a);LazyAssignMax mx(a);LazyAssignSum sum(a);LazyAssignMinMax mm(a);
    LazyChminMax cap(a);LazyChmaxMin floor(a);V b=a,c=a;
    for(int t=0;t<steps;++t){
        int l=static_cast<int>(rng()%(n+1)),r=static_cast<int>(rng()%(n+1));if(l>r)std::swap(l,r);
        const ll x=static_cast<ll>(rng()%101)-50;
        if(rng()%3!=0){mn.assign(l,r,x);mx.apply(l,r,x);sum.assign(l,r,x);mm.assign(l,r,x);for(int i=l;i<r;++i)a[i]=x;}
        else{const int i=static_cast<int>(rng()%n);mn.apply(i,x);mx.set(i,x);sum.set(i,x);mm.set(i,x);a[i]=x;}
        auto s=scan(a,l,r);CHECK(mn.min(l,r)==s.mn&&mx.prod(l,r)==s.mx&&sum.sum(l,r)==s.sum);
        CHECK(mm.prod(l,r).mn==s.mn&&mm.prod(l,r).mx==s.mx);
        if(rng()%8==0){const int i=static_cast<int>(rng()%n);cap.set(i,x);floor.set(i,x);b[i]=c[i]=x;}
        else{cap.chmin(l,r,x);floor.chmax(l,r,x);for(int i=l;i<r;++i){b[i]=std::min(b[i],x);c[i]=std::max(c[i],x);}}
        CHECK(cap.max(l,r)==scan(b,l,r).mx&&floor.min(l,r)==scan(c,l,r).mn);
    }
}
void affine_random(std::mt19937_64& rng,int steps){
    const int n=1+static_cast<int>(rng()%32);V a(n,2),b=a;
    LazyAffineStats s(a);LazyAffineSum mod(a);LazyAPAddSum ap(b);
    for(int t=0;t<steps;++t){
        int l=static_cast<int>(rng()%(n+1)),r=static_cast<int>(rng()%(n+1));if(l>r)std::swap(l,r);
        const ll x=static_cast<ll>(rng()%11)-5,coef=static_cast<ll>(rng()%3)-1;
        switch(rng()%7){
        case 0:s.affine(l,r,coef,x);mod.affine(l,r,coef,x);for(int i=l;i<r;++i)a[i]=coef*a[i]+x;break;
        case 1:s.add(l,r,x);mod.add(l,r,x);for(int i=l;i<r;++i)a[i]+=x;break;
        case 2:s.mul(l,r,coef);mod.mul(l,r,coef);for(int i=l;i<r;++i)a[i]*=coef;break;
        case 3:s.assign(l,r,x);mod.assign(l,r,x);for(int i=l;i<r;++i)a[i]=x;break;
        case 4:{const int i=static_cast<int>(rng()%n);s.set(i,x);mod.set(i,Mint(x));ap.set(i,x);a[i]=b[i]=x;break;}
        case 5:ap.add_ap(l,r,x,coef);for(int i=l;i<r;++i)b[i]+=x+coef*(i-l);break;
        default:ap.add_linear(l,r,coef,x);for(int i=l;i<r;++i)b[i]+=coef*i+x;break;
        }
        auto v=scan(a,l,r);CHECK(s.sum(l,r)==v.sum&&s.min(l,r)==v.mn&&s.max(l,r)==v.mx);
        Mint expected=0;for(int i=l;i<r;++i)expected+=Mint(a[i]);CHECK(mod.sum(l,r)==expected);
        CHECK(ap.sum(l,r)==scan(b,l,r).sum);
    }
}
void flip_random(std::mt19937_64& rng,int steps){
    const int n=1+static_cast<int>(rng()%32);V a(n);LazyFlipSum sum(a);LazyFlipInv inv(a);
    for(int t=0;t<steps;++t){
        int l=static_cast<int>(rng()%(n+1)),r=static_cast<int>(rng()%(n+1));if(l>r)std::swap(l,r);
        if(rng()%4==0){const int i=static_cast<int>(rng()%n);const ll x=static_cast<ll>(rng()%3);sum.set(i,x);inv.set(i,x);a[i]=(x!=0);}
        else{sum.flip(l,r);inv.flip(l,r);for(int i=l;i<r;++i)a[i]^=1;}
        ll ones=0,inversions=0;for(int i=l;i<r;++i){if(a[i])++ones;else inversions+=ones;}
        CHECK(sum.ones(l,r)==ones&&inv.inversions(l,r)==inversions);
    }
}

template<class Tree>void beats_state(){
    Tree zero,sized(3);CHECK(zero.empty()&&sized.size()==3&&sized.all_sum()==0);
    auto e=zero.prod(0,0);CHECK(e.len==0&&e.sum==0&&e.mn==Tree::INF&&e.mx==-Tree::INF);
    throws<std::invalid_argument>([]{Tree bad(-1);});
    throws<std::invalid_argument>([]{Tree bad(V{1,-1});});
    throws<std::out_of_range>([&]{zero.get(0);});throws<std::out_of_range>([&]{zero.set(0,1);});
    throws<std::out_of_range>([&]{sized.prod(1,0);});throws<std::out_of_range>([&]{sized.prod(0,4);});
    throws<std::invalid_argument>([&]{sized.set(1,-1);});
    Tree copied=sized,moved=std::move(copied);CHECK(copied.empty()&&moved.size()==3);
    CHECK(copied.prod(0,0).len==0&&copied.all_sum()==0);
    copied=std::move(moved);CHECK(moved.empty()&&copied.size()==3);
    copied.clear();CHECK(copied.empty()&&copied.sum(0,0)==0);copied.init(V{3,4});CHECK(copied.all_sum()==7);
    throws<std::invalid_argument>([&]{copied.init(V{-1});});CHECK(copied.all_sum()==7);
}
ll root_reference(ll x){ll lo=0,hi=3037000500LL;while(lo<hi){const ll mid=lo+(hi-lo+1)/2;if(static_cast<__int128_t>(mid)*mid<=x)lo=mid;else hi=mid-1;}return lo;}
void nonlinear_regressions(){
    beats_state<SegMod>();beats_state<SegDiv>();beats_state<SegSqrt>();beats_state<SegBitwiseBeats>();
    SegMod mod(V{8,3,10});mod.mod(0,3,6);CHECK(mod.all_sum()==9);
    throws<std::invalid_argument>([&]{mod.mod(0,0,0);});throws<std::out_of_range>([&]{mod.mod(-1,2,3);});
    SegDiv div(V{8,9,10,11});div.divide(0,4,4);CHECK(div.all_sum()==8);div.set(1,20);CHECK(div.sum(0,2)==22);
    div.divide(0,4,1);CHECK(div.all_sum()==26);throws<std::invalid_argument>([&]{div.divide(0,0,0);});
    SegSqrt sq(V{9,10,14,15});sq.sqrt(0,4);CHECK(sq.all_sum()==12);sq.set(2,100);CHECK(sq.sum(1,4)==106);sq.sqrt(1,4);CHECK(sq.sum(0,4)==15);
    const ll high=std::numeric_limits<ll>::max();
    for(ll x:V{0,1,2,3,4,15,16,17,high,3037000499LL*3037000499LL}){
        SegSqrt single(V{x});single.sqrt(0,1);CHECK(single.get(0)==root_reference(x));
    }
    SegMod large(V{high});large.mod(0,1,high);CHECK(large.get(0)==0);
    SegDiv large_div(V{high});large_div.divide(0,1,high);CHECK(large_div.get(0)==1);
    SegBitwiseBeats bits(V{(1LL<<62)|3});bits.bit_and(0,1,1LL<<62);CHECK(bits.get(0)==(1LL<<62));
    bits.bit_or(0,1,7);CHECK(bits.get(0)==((1LL<<62)|7));bits.bit_and(0,1,0);CHECK(bits.get(0)==0);
    throws<std::invalid_argument>([&]{bits.bit_or(0,0,-1);});
}
void nonlinear_random(std::mt19937_64& rng,int steps){
    const int n=1+static_cast<int>(rng()%32);V a(n),b(n),c(n),d(n);
    for(int i=0;i<n;++i)a[i]=b[i]=c[i]=d[i]=static_cast<ll>(rng()%10000);
    SegMod mod(a);SegDiv div(b);SegSqrt sq(c);SegBitwiseBeats bits(d);
    for(int t=0;t<steps;++t){
        int l=static_cast<int>(rng()%(n+1)),r=static_cast<int>(rng()%(n+1));if(l>r)std::swap(l,r);
        if(rng()%5==0){const int i=static_cast<int>(rng()%n);const ll x=static_cast<ll>(rng()%100000);
            mod.set(i,x);div.set(i,x);sq.set(i,x);bits.set(i,x);a[i]=b[i]=c[i]=d[i]=x;}
        else{
            const ll m=1+static_cast<ll>(rng()%1000),den=1+static_cast<ll>(rng()%20),mask=static_cast<ll>(rng()%65536);
            mod.mod(l,r,m);div.divide(l,r,den);sq.sqrt(l,r);
            if(rng()%2){bits.bit_and(l,r,mask);for(int i=l;i<r;++i)d[i]&=mask;}
            else{bits.bit_or(l,r,mask);for(int i=l;i<r;++i)d[i]|=mask;}
            for(int i=l;i<r;++i){a[i]%=m;b[i]/=den;c[i]=root_reference(c[i]);}
            if(rng()%6==0){const ll x=static_cast<ll>(rng()%5000);div.assign(l,r,x);for(int i=l;i<r;++i)b[i]=x;}
        }
        auto verify=[&](auto& tree,const V& values){auto v=tree.prod(l,r);auto expected=scan(values,l,r);
            CHECK(v.len==r-l&&v.sum==expected.sum);
            CHECK(v.mn==(l==r?std::numeric_limits<ll>::max():expected.mn));
            CHECK(v.mx==(l==r?-std::numeric_limits<ll>::max():expected.mx));
            CHECK(tree.all_sum()==scan(values,0,n).sum);
            const int i=static_cast<int>(rng()%n);CHECK(tree.get(i)==values[i]);};
        verify(mod,a);verify(div,b);verify(sq,c);verify(bits,d);
        unsigned long long bor=0,band=SegBitwiseBeats::ALL;for(int i=l;i<r;++i){bor|=static_cast<unsigned long long>(d[i]);band&=static_cast<unsigned long long>(d[i]);}
        const auto v=bits.prod(l,r);CHECK(v.bor==bor&&v.band==band);
    }
}
int main(){
    regressions();nonlinear_regressions();constexpr int seeds=32,steps=1024;
    for(int seed=0;seed<seeds;++seed){std::mt19937_64 rng(0x5365676d656e7473ULL+static_cast<unsigned>(seed));
        add_random(rng,steps);assign_random(rng,steps);affine_random(rng,steps);flip_random(rng,steps);nonlinear_random(rng,steps);}
    std::cout<<"Segment wrappers: regressions and "<<seeds*steps*5<<" randomized group steps passed\n";
}
