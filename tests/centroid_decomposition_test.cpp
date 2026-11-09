#include <bits/stdc++.h>
#include "silent-check.hpp"
using namespace std;
using ll=long long;
using vl=vector<ll>;
using pll=pair<ll,ll>;
using graph=vector<vl>;
#include "Tree/centroid_decomposition.cpp"
size_t exhaustive_trees=0,random_trees=0,nearest_queries=0;
void require(bool ok){if(!ok)abort();}
graph prufer(int n,size_t code){
    graph g(static_cast<size_t>(n));
    if(n<=1)return g;
    vector<int> sequence(size_t(n-2)),degree(size_t(n),1);
    for(auto&x:sequence){x=int(code%size_t(n));code/=size_t(n);++degree[size_t(x)];}
    for(int x:sequence){
        int leaf=0;while(degree[size_t(leaf)]!=1)++leaf;
        g[size_t(leaf)].push_back(x);g[size_t(x)].push_back(leaf);--degree[size_t(leaf)];--degree[size_t(x)];
    }
    int u=0;while(degree[size_t(u)]!=1)++u;
    int v=u+1;while(degree[size_t(v)]!=1)++v;
    g[size_t(u)].push_back(v);g[size_t(v)].push_back(u);return g;
}
vector<vl> distances(const graph&g){
    const size_t n=g.size();vector<vl> d(n,vl(n,-1));
    for(size_t s=0;s<n;++s){
        vl q{ll(s)};d[s][s]=0;
        for(size_t i=0;i<q.size();++i)for(ll u:g[size_t(q[i])])if(d[s][size_t(u)]<0){
            d[s][size_t(u)]=d[s][size_t(q[i])]+1;q.push_back(u);
        }
    }
    return d;
}
void validate(const graph&g,const CentroidDecomposition&cd,const vector<vl>&d){
    const ll n=ll(g.size());require(cd.size()==n);ll roots=0;
    for(ll v=0;v<n;++v){
        if(cd.parent(v)==-1){++roots;require(cd.depth(v)==0);}
        else{require(cd.parent(v)>=0&&cd.parent(v)<n);require(cd.depth(v)==cd.depth(cd.parent(v))+1);}
        const auto&path=cd.ancestors(v);require(path.size()==size_t(cd.depth(v)+1));
        require(path.size()<=size_t(bit_width(size_t(n))));require((path.back()==pll{v,0}));
        ll above=-1;
        for(auto [c,length]:path){require(c>=0&&c<n);require(cd.parent(c)==above);require(d[size_t(v)][size_t(c)]==length);above=c;}
        vector<char> blocked(size_t(n),false);
        for(ll c=cd.parent(v);c!=-1;c=cd.parent(c))blocked[size_t(c)]=true;
        vl component{v};vector<char> seen=blocked;seen[size_t(v)]=true;
        for(size_t i=0;i<component.size();++i)for(ll u:g[size_t(component[i])])if(!seen[size_t(u)]){seen[size_t(u)]=true;component.push_back(u);}
        seen=blocked;seen[size_t(v)]=true;
        for(ll s:g[size_t(v)])if(!seen[size_t(s)]){
            vl part{s};seen[size_t(s)]=true;
            for(size_t i=0;i<part.size();++i)for(ll u:g[size_t(part[i])])if(!seen[size_t(u)]){seen[size_t(u)]=true;part.push_back(u);}
            require(part.size()<=component.size()/2);
        }
    }
    require(roots==(n==0?0:1));
}
void check(const graph&g,bool queries,mt19937_64&rng){
    const graph original=g;CentroidDecomposition cd(g);require(g==original);
    const auto d=distances(g);validate(g,cd,d);
    if(queries&&!g.empty()){
        const ll n=ll(g.size()),unreached=LLONG_MAX;vl best(size_t(n),unreached);vector<char> active(size_t(n),false);
        for(int t=0;t<150;++t){
            const ll v=ll(rng()%size_t(n));
            if(t>0&&rng()%2){active[size_t(v)]=true;for(auto [c,length]:cd.ancestors(v))best[size_t(c)]=min(best[size_t(c)],length);}
            else{
                ll actual=unreached,expected=unreached;
                for(auto [c,length]:cd.ancestors(v))if(best[size_t(c)]!=unreached)actual=min(actual,best[size_t(c)]+length);
                for(ll u=0;u<n;++u)if(active[size_t(u)])expected=min(expected,d[size_t(v)][size_t(u)]);
                require(actual==expected);++nearest_queries;
            }
        }
    }
}
int main(){
    mt19937_64 rng(4869);CentroidDecomposition empty;require(empty.size()==0);check({},false,rng);
    for(int n=1;n<=7;++n){
        size_t count=1;for(int i=0;i<n-2;++i)count*=size_t(n);
        for(size_t code=0;code<count;++code){check(prufer(n,code),false,rng);++exhaustive_trees;}
    }
    for(int t=0;t<500;++t){
        const int n=1+int(rng()%80);graph g(static_cast<size_t>(n));
        for(int v=1;v<n;++v){const ll p=ll(rng()%size_t(v));g[size_t(v)].push_back(p);g[size_t(p)].push_back(v);}
        for(auto&row:g)shuffle(row.begin(),row.end(),rng);
        check(g,true,rng);++random_trees;
    }
    graph g={{1},{0,2,3},{1},{1,4},{3}};CentroidDecomposition cd(g);auto saved=cd.ancestors(4);g.clear();
    require(cd.size()==5&&saved.back()==pll(4,0));
    cd.build(graph{{}});require(cd.size()==1&&cd.parent(0)==-1&&cd.ancestors(0)==vector<pll>({{0,0}}));
    cd.build({});require(cd.size()==0&&saved.back()==pll(4,0));
    const vector<graph> invalid={{{0}},{{1},{}},{{1},{0,2},{1,0}},{{1,2},{2},{1}},{{1,2},{0,2},{0,1},{}},{{1,1},{0,0},{3},{2}},{{-1}},{{2},{0}}};
    for(const auto&bad:invalid)require(stops_silently([&]{CentroidDecomposition rejected(bad);}));
    for(int api=0;api<3;++api)for(ll v:{-1LL,1LL})require(stops_silently([&]{
        CentroidDecomposition one(graph{{}});if(api==0)(void)one.parent(v);if(api==1)(void)one.depth(v);if(api==2)(void)one.ancestors(v);
    }));
    require(stops_silently([&]{(void)empty.ancestors(0);}));
    const ll n=200000;
    for(int shape=0;shape<2;++shape){
        graph large(static_cast<size_t>(n));
        for(ll v=1;v<n;++v){ll p=shape==0?v-1:0;large[size_t(v)].push_back(p);large[size_t(p)].push_back(v);}
        CentroidDecomposition decomposition(large);require(decomposition.size()==n);
        for(ll v=0;v<n;++v){
            const auto&path=decomposition.ancestors(v);require(path.size()==size_t(decomposition.depth(v)+1));require(path.size()<=size_t(bit_width(size_t(n))));
            ll above=-1;
            for(auto [c,length]:path){require(decomposition.parent(c)==above);require(length==(shape==0?abs(c-v):(c==v?0:(c==0||v==0?1:2))));above=c;}
            require((path.back()==pll{v,0}));
        }
        if(shape==1)for(ll v=1;v<n;++v)require(decomposition.parent(v)==0&&decomposition.depth(v)==1);
    }
    cout<<"Passed "<<exhaustive_trees<<" exhaustive labelled trees, "<<random_trees<<" random trees, "<<nearest_queries
        <<" nearest-marked comparisons; 200000-node path/star, rebuild, 15 silent-stop cases\n";
}
