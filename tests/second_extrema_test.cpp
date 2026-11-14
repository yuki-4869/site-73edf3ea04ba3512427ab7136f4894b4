#include <bits/stdc++.h>
#include <atcoder/segtree>
#include <atcoder/lazysegtree>
#include <atcoder/modint>
#include "silent-check.hpp"
using namespace std;
using namespace atcoder;
using ll=long long;
using vl=vector<ll>;
using mint=modint998244353;
const ll inf=LLONG_MAX;
template<class T>ll sz(const T&x){return static_cast<ll>(x.size());}
#define rep(i,n) for(ll i=0;i<ll(n);++i)
#define pb push_back
#include "DataStructureSegment/segtree_wrappers.cpp"
#include "DataStructureSegment/lazy_segtree_wrappers.cpp"
size_t checks=0;
void require(bool ok,source_location location=source_location::current()){
    ++checks;if(!ok){cerr<<"Failed line "<<location.line()<<'\n';abort();}
}
template<class A,class B>bool equal(A a,B b){
    return a.count==b.count&&(!a.count||a.first==b.first)&&(a.count<2||a.second==b.second);
}
template<class S>void validate(S result,const vl&a,int l,int r,bool maximum){
    vl values(a.begin()+l,a.begin()+r);sort(values.begin(),values.end());
    values.erase(unique(values.begin(),values.end()),values.end());
    if(maximum)reverse(values.begin(),values.end());
    require(result.count==int(min(size_t(2),values.size())));
    if(result.count)require(result.first==values[0]);
    if(result.count==2)require(result.second==values[1]);
}
template<class Tree>void range(Tree&t,const vl&a,int l,int r,bool maximum){
    validate(t.prod(l,r),a,l,r,maximum);validate(t.all_prod(),a,0,int(a.size()),maximum);
}
bool plain_uniform(SegMaxSecond::S s){return s.count<2;}
bool lazy_uniform(LazyAddMaxSecond::S s){return s.count<2;}
template<class Tree>void search(Tree&t,const vl&a,int l,int r,bool maximum,ll bound){
    auto uniform=[](auto s){return s.count<2;};
    int right=l;while(right<int(a.size())&&(right==l||a[right]==a[l]))++right;
    int left=r;while(left>0&&(left==r||a[left-1]==a[r-1]))--left;
    require(t.max_right(l,uniform)==right);require(t.min_left(r,uniform)==left);
    auto bounded=[&](auto s){return !s.count||(maximum?s.first<=bound:s.first>=bound);};
    right=l;while(right<int(a.size())&&(maximum?a[right]<=bound:a[right]>=bound))++right;
    left=r;while(left>0&&(maximum?a[left-1]<=bound:a[left-1]>=bound))--left;
    require(t.max_right(l,bounded)==right);require(t.min_left(r,bounded)==left);
}
void exhaustive(){
    const vl alphabet={-2,0,3};size_t count=1;
    for(int n=0;n<=7;++n){
        for(size_t code=0;code<count;++code){
            size_t x=code;vl a(n);for(ll&v:a){v=alphabet[x%3];x/=3;}
            SegMaxSecond high(a);SegMinSecond low(a);
            LazyAddMaxSecond ah(a);LazyAddMinSecond al(a);
            LazyAssignMaxSecond sh(a);LazyAssignMinSecond sl(a);
            for(int l=0;l<=n;++l)for(int r=l;r<=n;++r){
                range(high,a,l,r,true);range(low,a,l,r,false);
                range(ah,a,l,r,true);range(al,a,l,r,false);
                range(sh,a,l,r,true);range(sl,a,l,r,false);
            }
        }
        count*=3;
    }
}
template<bool Maximum>void algebra(){
    vl alphabet={LLONG_MIN,-3,0,2,LLONG_MAX};
    vector<SegSecond_impl::S> values{SegSecond_impl::e()};
    for(ll x:alphabet){
        values.push_back({x,0,1});
        for(ll y:alphabet)if(Maximum?x>y:x<y)values.push_back({x,y,2});
    }
    auto op=SegSecond_impl::op<Maximum>;
    auto lop=lazy_second_detail::op<Maximum>;
    for(auto a:values){
        require(equal(op(a,SegSecond_impl::e()),a));require(equal(op(SegSecond_impl::e(),a),a));
        for(auto b:values){
            for(auto c:values)require(equal(op(op(a,b),c),op(a,op(b,c))));
            auto la=lazy_second_detail::S{a.first,a.second,a.count};
            auto lb=lazy_second_detail::S{b.first,b.second,b.count};
            require(equal(op(a,b),lop(la,lb)));
            for(ll value:alphabet){
                LazyAssignF f{value,true};
                require(equal(lazy_second_detail::assign(f,lop(la,lb)),lop(lazy_second_detail::assign(f,la),lazy_second_detail::assign(f,lb))));
                LazyAssignF g{-2,true};
                require(equal(lazy_second_detail::assign(lazy_second_detail::compose_assign(f,g),la),lazy_second_detail::assign(f,lazy_second_detail::assign(g,la))));
            }
            if((!a.count||(a.first>-100&&a.first<100&&(a.count<2||(a.second>-100&&a.second<100))))&&
               (!b.count||(b.first>-100&&b.first<100&&(b.count<2||(b.second>-100&&b.second<100))))){
                for(ll f:vl{-8,0,5})for(ll g:vl{-3,0,6}){
                    require(equal(lazy_second_detail::add(f,lop(la,lb)),lop(lazy_second_detail::add(f,la),lazy_second_detail::add(f,lb))));
                    require(equal(lazy_second_detail::add(lazy_second_detail::compose_add(f,g),la),lazy_second_detail::add(f,lazy_second_detail::add(g,la))));
                }
            }
        }
    }
}
template<class Tree>void lazy_state(){
    Tree blank,zero(0),tree(4);
    require(blank.empty()&&zero.empty()&&tree.size()==4);
    validate(tree.prod(0,4),vl(4,0),0,4,true);
    require(stops_silently([]{Tree t(-1);}));require(stops_silently([]{Tree t(INT_MAX);}));
    require(stops_silently([&]{tree.get(4);}));require(stops_silently([&]{tree.prod(2,1);}));
    require(stops_silently([&]{tree.apply(-1,2,{});}));require(stops_silently([&]{tree.max_right(0,[](auto){return false;});}));
    Tree copy=tree,moved=std::move(copy);require(copy.empty()&&moved.size()==4);
    require(copy.prod(0,0).count==0&&copy.all_prod().count==0);
    copy.apply(0,0,{});require(copy.max_right(0,[](auto){return true;})==0);
    copy=std::move(moved);require(moved.empty()&&copy.size()==4);
    copy.set(0,123);require(tree.get(0).first==0);copy=zero;require(copy.empty());
}
void boundary(){
    SegMaxSecond none,sized(3);SegMinSecond minimum(0);
    require(none.all_prod().count==0&&minimum.prod(0,0).count==0&&sized.all_prod().count==0);
    sized.set(1,LLONG_MIN);require(sized.all_prod().count==1&&sized.all_prod().first==LLONG_MIN);
    sized.set(2,LLONG_MAX);require(sized.all_prod().count==2&&sized.all_prod().second==LLONG_MIN);
    sized.set(1,SegMaxSecond::S{});require(sized.all_prod().count==1);
    require(stops_silently([]{SegMaxSecond t(-1);}));require(stops_silently([]{SegMinSecond t(INT_MAX);}));
    lazy_state<LazyAddMaxSecond>();lazy_state<LazyAddMinSecond>();lazy_state<LazyAssignMaxSecond>();lazy_state<LazyAssignMinSecond>();
    vl a={LLONG_MIN,LLONG_MAX,LLONG_MIN,LLONG_MAX,0};
    SegMaxSecond high(a);SegMinSecond low(a);LazyAddMaxSecond ah(a);LazyAddMinSecond al(a);LazyAssignMaxSecond sh(a);LazyAssignMinSecond sl(a);
    range(high,a,0,5,true);range(low,a,0,5,false);range(ah,a,0,5,true);range(al,a,0,5,false);
    ah.apply(0,5,0);al.apply(0,5,0);range(ah,a,0,5,true);range(al,a,0,5,false);
    sh.assign(0,5,LLONG_MIN);sl.assign(0,5,LLONG_MIN);a=vl(5,LLONG_MIN);
    sh.apply(1,4,LLONG_MAX);sl.apply(1,4,LLONG_MAX);for(int i=1;i<4;++i)a[i]=LLONG_MAX;
    range(sh,a,0,5,true);range(sl,a,0,5,false);
    sh.apply(0,5,LazyAssignF{});sl.apply(0,5,LazyAssignF{});range(sh,a,0,5,true);range(sl,a,0,5,false);
    LazyAddMaxSecond maxadd(3);LazyAddMinSecond minadd(3);
    maxadd.add(0,3,LLONG_MIN);minadd.add(0,3,LLONG_MIN);maxadd.add(0,3,LLONG_MAX);minadd.add(0,3,LLONG_MAX);
    require(maxadd.get(1).first==-1&&minadd.get(1).first==-1);
    require(high.max_right<plain_uniform>(0)==1);require(ah.max_right<lazy_uniform>(0)==1);
    require(high.min_left<plain_uniform>(5)==4);require(ah.min_left<lazy_uniform>(5)==4);
    SegMaxSecond raw(vector<SegMaxSecond::S>{{5,0,1},{3,0,1}});
    LazyAddMinSecond lraw(vector<LazyAddMinSecond::S>{{5,0,1},{3,0,1}});
    require(raw.prod(0,2).second==3&&lraw.prod(0,2).second==5);
}
void randomized(){
    mt19937_64 rng(4869);
    for(int trial=0;trial<80;++trial){
        int n=int(rng()%41);vl a(n),b(n);
        for(ll&x:a)x=ll(rng()%9)-4;
        b=a;
        SegMaxSecond high(a);SegMinSecond low(a);LazyAddMaxSecond ah(a);LazyAddMinSecond al(a);LazyAssignMaxSecond sh(b);LazyAssignMinSecond sl(b);
        for(int step=0;step<1500;++step){
            int l=int(rng()%(n+1)),r=int(rng()%(n+1));if(l>r)swap(l,r);
            ll x=ll(rng()%17)-8;int kind=int(rng()%5);
            if(kind==0&&n){
                int p=int(rng()%n);a[p]=b[p]=x;
                high.set(p,x);low.set(p,x);ah.set(p,x);al.set(p,x);sh.set(p,x);sl.set(p,x);
            }else if(kind==1){
                ah.add(l,r,x);al.apply(l,r,x);
                for(int i=l;i<r;++i){a[i]+=x;high.set(i,a[i]);low.set(i,a[i]);}
            }else if(kind==2){
                sh.assign(l,r,x);sl.apply(l,r,x);for(int i=l;i<r;++i)b[i]=x;
            }else if(kind==3&&n){
                int p=int(rng()%n);ah.apply(p,x);al.apply(p,x);a[p]+=x;high.set(p,a[p]);low.set(p,a[p]);
                sh.apply(p,x);sl.apply(p,LazyAssignF{x,true});b[p]=x;
            }else{
                sh.apply(l,r,LazyAssignF{});sl.apply(l,r,LazyAssignF{});
            }
            range(high,a,l,r,true);range(low,a,l,r,false);range(ah,a,l,r,true);range(al,a,l,r,false);range(sh,b,l,r,true);range(sl,b,l,r,false);
            if(n){int p=int(rng()%n);validate(high.get(p),a,p,p+1,true);validate(low.get(p),a,p,p+1,false);validate(ah.get(p),a,p,p+1,true);validate(al.get(p),a,p,p+1,false);validate(sh.get(p),b,p,p+1,true);validate(sl.get(p),b,p,p+1,false);}
            if(step%7==0){search(high,a,l,r,true,x);search(low,a,l,r,false,x);search(ah,a,l,r,true,x);search(al,a,l,r,false,x);search(sh,b,l,r,true,x);search(sl,b,l,r,false,x);}
            if(step%101==0&&n){auto copy=ah;copy.set(0,123456);require(ah.get(0).first==a[0]);auto normal=high;normal.set(0,123456);require(high.get(0).first==a[0]);}
        }
    }
}
void large(){
    const int n=200000;vl a(n);for(int i=0;i<n;++i)a[i]=i%3;
    SegMaxSecond high(a);SegMinSecond low(a);LazyAddMaxSecond ah(a);LazyAddMinSecond al(a);LazyAssignMaxSecond sh(a);LazyAssignMinSecond sl(a);
    require(high.all_prod().second==1&&low.all_prod().second==1);
    ah.add(0,n,10);al.add(0,n,-10);require(ah.prod(1,n-1).second==11&&al.prod(1,n-1).second==-9);
    sh.assign(0,n,7);sl.assign(0,n,7);require(sh.all_prod().count==1&&sl.all_prod().count==1);
    sh.set(n/2,-9);sl.set(n/2,-9);require(sh.all_prod().second==-9&&sl.all_prod().second==7);
    require(sh.max_right(0,[](auto s){return s.count<2;})==n/2);
}
int main(){
    boundary();algebra<true>();algebra<false>();exhaustive();randomized();large();
    cout<<"Passed "<<checks<<" second-extrema checks: all ternary sequences up to length 7, 120000 random operations across 6 types, algebra laws, LLONG_MIN/MAX, empty/uniform ranges, searches, copies/moves, 200000 elements\n";
}
