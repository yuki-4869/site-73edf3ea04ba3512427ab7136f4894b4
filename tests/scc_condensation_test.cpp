#include <bits/stdc++.h>
using namespace std;
using ll=long long;
using vl=vector<ll>;
using vb=vector<bool>;
using graph=vector<vl>;
using Graph=vector<vector<pair<ll,ll>>>;
#define rep(i,n) for(ll i=0;i<(ll)(n);++i)
#define pb push_back
#include "Graph/scc_condensation.cpp"

void require(bool ok){if(!ok)abort();}
size_t cases=0;

vb cycle_vertices(const Graph& g){
    ll n=g.size();
    vb result(n,false),used(n,false);
    vl path;
    for(ll root=0;root<n;++root){
        auto dfs=[&](auto&& self,ll v,__int128 sum)->void{
            used[v]=true;path.pb(v);
            for(auto [to,w]:g[v]){
                if(to==root){
                    if(sum+(__int128)w!=0)for(ll u:path)result[u]=true;
                }
                else if(to>root&&!used[to])self(self,to,sum+(__int128)w);
            }
            path.pop_back();used[v]=false;
        };
        dfs(dfs,root,0);
    }
    return result;
}

void check(const Graph& g){
    auto original=g;
    auto s=SCCDecomposition(g);
    ll n=g.size();
    graph plain(n);
    vector<vb> reach(n,vb(n,false));
    rep(v,n)for(auto [to,w]:g[v]){plain[v].pb(to);reach[v][to]=true;}
    auto old=SCCDecomposition(plain);
    require(s.K==old.K&&s.cmp==old.cmp&&s.groups==old.groups);
    require(s.dag==old.dag&&s.rdag==old.rdag&&s.indeg==old.indeg&&s.outdeg==old.outdeg);
    require(s.cyclic==old.cyclic&&s.sources()==old.sources()&&s.sinks()==old.sinks());
    rep(k,n)rep(i,n)rep(j,n)if(reach[i][k]&&reach[k][j])reach[i][j]=true;
    rep(i,n){
        require(s.in_cycle(i)==reach[i][i]);
        rep(j,n)require(s.same(i,j)==(i==j||(reach[i][j]&&reach[j][i])));
    }
    auto nonzero_vertices=cycle_vertices(g);
    vb expected(s.K,false);
    rep(v,n)if(nonzero_vertices[v])expected[s.cmp[v]]=true;
    require(s.nonzero_cycle==expected);
    const auto& constant=s;
    require(constant.has_nonzero_cycle()==(find(expected.begin(),expected.end(),true)!=expected.end()));
    require(g==original);
    ++cases;
}

int main(){
    static_assert(is_same_v<decltype(SCCDecomposition(declval<const graph&>())),SCCResult>);
    static_assert(is_same_v<decltype(SCCDecomposition(declval<const Graph&>())),WeightedSCCResult>);
    static_assert(is_base_of_v<SCCResult,WeightedSCCResult>);
    check(Graph{});
    for(ll n=1;n<=3;++n){
        ll graphs=1LL<<(2*n*n);
        for(ll mask=0;mask<graphs;++mask){
            ll code=mask;
            Graph g(n);
            rep(u,n)rep(v,n){
                ll choice=code%4;code/=4;
                if(choice)g[u].pb({v,choice-2});
            }
            check(g);
        }
    }
    mt19937_64 rng(4869);
    for(int test=0;test<4000;++test){
        ll n=1+rng()%7;
        Graph g(n);
        rep(u,n)rep(v,n)if(rng()%3==0){
            g[u].pb({v,ll(rng()%9)-4});
            if(rng()%5==0)g[u].pb({v,ll(rng()%9)-4});
        }
        for(auto& edges:g)shuffle(edges.begin(),edges.end(),rng);
        check(g);
    }
    const ll high=LLONG_MAX,low=LLONG_MIN;
    for(ll weight:{low,0LL,high})check(Graph{{{0,weight}}});
    Graph huge(4);
    huge[0]={{1,high}};huge[1]={{2,high}};
    huge[2]={{3,-high}};huge[3]={{0,-high}};
    check(huge);
    require(!SCCDecomposition(huge).has_nonzero_cycle());
    ++huge[3][0].second;
    check(huge);
    require(SCCDecomposition(huge).has_nonzero_cycle());
    check(Graph{{{1,low}},{{0,high}}});
    check(Graph{{{1,0},{2,10}},{{2,0}},{{0,1}}});
    check(Graph{{{1,0},{2,10}},{{2,0}}, {}});
    check(Graph{{{1,1},{1,2}},{{0,-1}}, {}});

    const ll n=200000;
    {
        Graph chain(n);
        for(ll v=1;v<n;++v)chain[v-1].pb({v,v%2?low:high});
        auto s=SCCDecomposition(chain);
        require(s.K==n&&!s.has_cycle()&&!s.has_nonzero_cycle());
        for(ll v=1;v<n;++v)require(s.cmp[v-1]<s.cmp[v]);
    }
    {
        Graph ring(n);
        rep(v,n)ring[v].pb({(v+1)%n,v<n/2?high:-high});
        auto s=SCCDecomposition(ring);
        require(s.K==1&&s.has_cycle()&&!s.has_nonzero_cycle());
        ++ring[n-1][0].second;
        s=SCCDecomposition(ring);
        require(s.K==1&&s.has_nonzero_cycle()&&s.nonzero_cycle[0]);
    }
    cout<<"Passed "<<cases<<" independent simple-cycle oracle cases; legacy SCC parity, extreme weights, and 200000-vertex graphs\n";
}
