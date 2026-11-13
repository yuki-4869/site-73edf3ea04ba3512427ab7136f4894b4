#include <bits/stdc++.h>
#include "silent-check.hpp"
using namespace std;
using ll=long long;
using LL=__int128;
using pll=pair<ll,ll>;
using U=unsigned __int128;
#include "Geometry/closest_pair.cpp"
size_t comparisons=0;
void require(bool ok){if(!ok)abort();}
U reference_distance(pll a,pll b){
    const auto x=static_cast<unsigned long long>(max(a.first,b.first)-min(a.first,b.first));
    const auto y=static_cast<unsigned long long>(max(a.second,b.second)-min(a.second,b.second));
    return U(x)*x+U(y)*y;
}
void validate(const vector<pll>&p,const ClosestPairResult&r,U expected){
    if(p.size()<2){require(r.indices==pll(-1,-1)&&r.squared_distance==0);return;}
    const auto [i,j]=r.indices;require(i>=0&&i<j&&size_t(j)<p.size());
    require(r.squared_distance>=0&&U(r.squared_distance)==expected);
    require(reference_distance(p[size_t(i)],p[size_t(j)])==expected);
}
void check(const vector<pll>&p){
    const auto original=p;const auto result=closest_pair(p);require(p==original);
    U best=0;
    if(p.size()>=2){
        best=reference_distance(p[0],p[1]);
        for(size_t i=0;i<p.size();++i)for(size_t j=0;j<i;++j)best=min(best,reference_distance(p[i],p[j]));
    }
    validate(p,result,best);++comparisons;
}
int main(){
    ClosestPairResult blank;require(blank.squared_distance==0&&blank.indices==pll(-1,-1));
    mt19937_64 rng(4869);
    const vector<pll> alphabet={{-2,-1},{0,0},{1,2},{3,-1}};
    size_t count=1;
    for(int n=0;n<=7;++n){
        for(size_t code=0;code<count;++code){
            size_t x=code;vector<pll> p(size_t(n),pll{});
            for(auto&point:p){point=alphabet[x%alphabet.size()];x/=alphabet.size();}check(p);
        }
        count*=alphabet.size();
    }
    vector<pll> grid;for(ll x=-1;x<=1;++x)for(ll y=-1;y<=1;++y)grid.emplace_back(x,y);
    for(int mask=0;mask<512;++mask){
        vector<pll> p;for(int i=0;i<9;++i)if((mask>>i)&1)p.push_back(grid[size_t(i)]);
        check(p);shuffle(p.begin(),p.end(),rng);check(p);
    }
    vector<pll> permutation;
    auto enumerate=[&](auto&&self,unsigned mask)->void{
        check(permutation);if(permutation.size()==4)return;
        for(size_t i=0;i<grid.size();++i)if(!((mask>>i)&1)){permutation.push_back(grid[i]);self(self,mask|(1U<<i));permutation.pop_back();}
    };enumerate(enumerate,0);
    const ll B=1000000000000000000LL;
    const array<ll,7> extreme={-B,-B+1,-1,0,1,B-1,B};
    for(int t=0;t<5000;++t){
        vector<pll> p(rng()%71);
        for(auto&[x,y]:p){
            if(t%4==0){x=extreme[rng()%extreme.size()];y=extreme[rng()%extreme.size()];}
            if(t%4==1){x=ll(rng()%2001)-1000;y=ll(rng()%2001)-1000;}
            if(t%4==2){x=ll(rng()%2000000000000000001ULL)-B;y=ll(rng()%2000000000000000001ULL)-B;}
            if(t%4==3){x=t%8==3?0:ll(rng()%101);y=t%8==3?ll(rng()%101):0;}
        }
        check(p);
    }
    check({{-B,-B},{B,B}});check({{-B,0},{B,0}});check({{B,B},{B-1,B},{-B,-B}});
    check({{-5,100},{-4,-100},{-1,0},{1,1},{4,100},{5,-100}});
    for(auto point:vector<pll>{{B+1,0},{-B-1,0},{0,B+1},{0,-B-1},{LLONG_MIN,0},{LLONG_MAX,0},{0,LLONG_MIN},{0,LLONG_MAX}})
        require(stops_silently([&]{closest_pair(vector<pll>{point});}));
    require(stops_silently([&]{closest_pair(vector<pll>{{0,0},{0,0},{B+1,0}});}));
    const ll n=500000;
    for(int pattern=0;pattern<3;++pattern){
        vector<pll> p;p.reserve(size_t(n));
        for(ll i=0;i<n;++i){
            if(pattern==0)p.emplace_back(3*i,0);
            if(pattern==1)p.emplace_back(0,2*i);
            if(pattern==2)p.emplace_back(i/1000,i%1000);
        }
        shuffle(p.begin(),p.end(),rng);const auto original=p;
        validate(p,closest_pair(p),pattern==0?9:(pattern==1?4:1));require(p==original);
    }
    cout<<"Passed "<<comparisons<<" all-pairs comparisons; unsigned 128-bit reference, full coordinate extremes, 3 half-million-point sets, 9 silent-stop cases\n";
}
