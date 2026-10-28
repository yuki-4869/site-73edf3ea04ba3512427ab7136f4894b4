#include <bits/stdc++.h>
#include <atcoder/modint>
#include "silent-check.hpp"
using namespace std;
using ll=long long;
using LL=__int128;
using vl=vector<ll>;
#include "FPSandPolynomial/bitmask_transforms.cpp"
using B=BitmaskTransform;
size_t cases=0;
void require(bool ok){if(!ok)abort();}
template<class T> vector<T> sums(const vector<T>&a,bool upper,bool inverse){
    vector<T> out(a.size(),T(0));
    for(size_t s=0;s<a.size();++s)for(size_t u=0;u<a.size();++u){
        if(upper?(s&u)==s:(s&u)==u){
            const auto diff=upper?(u^s):(s^u);
            if(inverse&&(popcount(diff)&1))out[s]-=a[u];
            else out[s]+=a[u];
        }
    }
    return out;
}
template<class T> vector<T> brute(const vector<T>&a,const vector<T>&b,bool upper){
    vector<T> c(a.size(),T(0));
    for(size_t i=0;i<a.size();++i)for(size_t j=0;j<a.size();++j)c[upper?(i&j):(i|j)]+=a[i]*b[j];
    return c;
}
template<class T> void transforms(const vector<T>&a){
    auto b=a;B::subset_zeta(b);require(b==sums(a,false,false));B::subset_mobius(b);require(b==a);
    b=a;B::subset_mobius(b);require(b==sums(a,false,true));B::subset_zeta(b);require(b==a);
    b=a;B::superset_zeta(b);require(b==sums(a,true,false));B::superset_mobius(b);require(b==a);
    b=a;B::superset_mobius(b);require(b==sums(a,true,true));B::superset_zeta(b);require(b==a);
    ++cases;
}
template<class T> void convolution(const vector<T>&a,const vector<T>&b){
    auto before_a=a,before_b=b;
    require(B::or_convolution(a,b)==brute(a,b,false));
    require(B::and_convolution(a,b)==brute(a,b,true));
    require(a==before_a&&b==before_b);
    ++cases;
}
vector<vl> all_arrays(size_t n){
    vector<vl> out(1);
    for(size_t i=0;i<n;++i){vector<vl> next;for(auto a:out)for(ll x:{-1,0,1}){auto b=a;b.push_back(x);next.push_back(b);}out=std::move(next);}
    return out;
}
template<class T> void random_cases(mt19937_64&rng){
    for(int t=0;t<300;++t){
        size_t n=size_t(1)<<(rng()%8);
        vector<T>a(n),b(n);
        for(size_t i=0;i<n;++i){a[i]=T(ll(rng()%21)-10);b[i]=T(ll(rng()%21)-10);}
        transforms(a);convolution(a,b);convolution(a,a);
    }
    transforms(vector<T>{});convolution(vector<T>{},vector<T>{});
}
int main(){
    for(size_t n:{1,2,4,8})for(auto a:all_arrays(n))transforms(a);
    auto arrays=all_arrays(4);for(auto a:arrays)for(auto b:arrays)convolution(a,b);
    mt19937_64 rng(4869);
    random_cases<ll>(rng);random_cases<LL>(rng);
    random_cases<atcoder::static_modint<2>>(rng);
    random_cases<atcoder::static_modint<6>>(rng);
    random_cases<atcoder::static_modint<998244353>>(rng);
    transforms(vector<LL>{LL(1)<<80,-(LL(1)<<78),LL(1)<<76,-(LL(1)<<74)});
    for(size_t n:{3,5,6}){
        for(int operation=0;operation<6;++operation)require(stops_silently([=]{
            vl a(n),b(n);
            if(operation==0)B::subset_zeta(a);
            if(operation==1)B::subset_mobius(a);
            if(operation==2)B::superset_zeta(a);
            if(operation==3)B::superset_mobius(a);
            if(operation==4)B::or_convolution(a,b);
            if(operation==5)B::and_convolution(a,b);
        }));
    }
    for(size_t n:{0,1,2})for(size_t m:{0,1,2})if(n!=m){
        require(stops_silently([=]{B::or_convolution(vl(n),vl(m));}));
        require(stops_silently([=]{B::and_convolution(vl(n),vl(m));}));
    }
    const size_t n=size_t(1)<<20;
    vl a(n,1);B::subset_zeta(a);
    for(size_t s=0;s<n;++s)require(a[s]==(ll(1)<<popcount(s)));
    B::subset_mobius(a);require(a==vl(n,1));
    B::superset_zeta(a);
    for(size_t s=0;s<n;++s)require(a[s]==(ll(1)<<(20-popcount(s))));
    B::superset_mobius(a);require(a==vl(n,1));
    auto c=B::or_convolution(a,a),d=B::and_convolution(a,a);
    vl pow3(21,1);for(size_t i=1;i<pow3.size();++i)pow3[i]=pow3[i-1]*3;
    for(size_t s=0;s<n;++s){require(c[s]==pow3[popcount(s)]);require(d[s]==pow3[20-popcount(s)]);}
    cout<<"Passed "<<cases<<" direct-enumeration cases; 2^20-element identities; 30 invalid-length checks\n";
}
