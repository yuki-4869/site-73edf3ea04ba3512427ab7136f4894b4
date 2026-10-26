#include <bits/stdc++.h>
#include "silent-check.hpp"
using namespace std;
using ll=long long;
using LL=__int128;
using pll=pair<ll,ll>;
#include "Geometry/convex_hull.cpp"
using cpp_int=__int128;
size_t cases=0;
void require(bool ok){if(!ok)abort();}
cpp_int turn(pll a,pll b,pll c){return (cpp_int(b.first)-a.first)*(cpp_int(c.second)-a.second)-(cpp_int(b.second)-a.second)*(cpp_int(c.first)-a.first);}
cpp_int dist(pll a,pll b){cpp_int x=cpp_int(a.first)-b.first,y=cpp_int(a.second)-b.second;return x*x+y*y;}
vector<pll> oracle(vector<pll> p,bool keep){
    sort(p.begin(),p.end());p.erase(unique(p.begin(),p.end()),p.end());
    if(p.size()<=2)return p;
    bool line=true;for(auto c:p)if(turn(p[0],p[1],c)!=0)line=false;
    if(line)return keep?p:vector<pll>{p.front(),p.back()};
    map<pll,pll> next;
    for(auto a:p)for(auto b:p)if(a!=b){
        bool edge=true;
        for(auto c:p){
            auto cross=turn(a,b,c);
            if(cross<0){edge=false;break;}
            if(cross==0&&c!=a&&c!=b){
                if(keep){if(dist(a,c)<dist(a,b)&&dist(b,c)<dist(a,b)){edge=false;break;}}
                else if(dist(a,c)>dist(a,b)||dist(b,c)>dist(a,b)){edge=false;break;}
            }
        }
        if(edge){require(!next.count(a));next[a]=b;}
    }
    vector<pll> h;pll a=p[0];
    do{h.push_back(a);require(next.count(a));a=next[a];require(h.size()<=p.size());}while(a!=p[0]);
    return h;
}
void check(const vector<pll>&p){
    auto original=p;
    for(bool keep:{false,true})require(convex_hull(p,keep)==oracle(p,keep));
    require(p==original);++cases;
}
int main(){
    const ll invalid=1000000000000000001LL;
    for(auto p:vector<pll>{{invalid,0},{-invalid,0},{0,invalid},{0,-invalid}})require(stops_silently([&]{convex_hull({p});}));
    vector<pll> grid;for(ll x=-1;x<=1;++x)for(ll y=-1;y<=1;++y)grid.emplace_back(x,y);
    mt19937_64 rng(4869);
    for(int mask=0;mask<512;++mask){
        vector<pll> p;for(int i=0;i<9;++i)if(mask>>i&1)p.push_back(grid[i]);
        check(p);auto dup=p;p.insert(p.end(),dup.begin(),dup.end());shuffle(p.begin(),p.end(),rng);check(p);
    }
    const ll B=1000000000000000000LL;
    check({{-B,-B},{B,-B},{B,B},{-B,B},{0,0},{0,-B},{B,0},{0,B},{-B,0}});
    check({{-B,B},{B,-B},{0,0},{-B,B}});
    for(int t=0;t<2000;++t){
        vector<pll> p;int n=int(rng()%19);
        for(int i=0;i<n;++i)p.emplace_back(ll(rng()%21)-10,ll(rng()%21)-10);
        if(t%2)for(auto&[x,y]:p){x*=B/10;y*=B/10;}
        check(p);
    }
    vector<pll> large;for(ll i=0;i<500000;++i)large.emplace_back(i,i*i);
    auto hull=convex_hull(large);require(hull==large);require(convex_hull(large,true)==large);
    for(ll i=0;i<500000;++i)large[i]={i,2*i};
    require((convex_hull(large)==vector<pll>{large.front(),large.back()}));require(convex_hull(large,true)==large);
    cout<<"Passed "<<cases<<" independent oracle cases, both modes; 500000-point convex/collinear cases\n";
}
