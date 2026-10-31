#include <bits/stdc++.h>
using namespace std;
using ll=long long;
using vl=vector<ll>;
using graph=vector<vl>;
using Graph=vector<vector<pair<ll,ll>>>;
#define rep(i,n) for(ll i=0;i<(ll)(n);++i)
#define pb push_back
#define all(a) (a).begin(),(a).end()
#include "Tree/tree_diameter.cpp"

void require(bool ok){if(!ok)abort();}
size_t cases=0;

vl oracle(const Graph& g){
    const ll inf=LLONG_MAX;
    ll n=g.size();
    vector<vl> d(n,vl(n,inf));
    rep(i,n){
        d[i][i]=0;
        for(auto [j,w]:g[i])d[i][j]=w;
    }
    rep(k,n)rep(i,n)rep(j,n){
        if(d[i][k]!=inf && d[k][j]!=inf && d[i][k]<=inf-d[k][j])
            d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
    }
    vl eccentricity(n);
    rep(i,n)eccentricity[i]=*max_element(all(d[i]));
    ll radius=*min_element(all(eccentricity));
    vl result;
    rep(i,n)if(eccentricity[i]==radius)result.pb(i);
    return result;
}

void check(const Graph& g){
    WeightedTreeDiameter weighted(g);
    const auto& const_weighted=weighted;
    vl expected=oracle(g);
    require(const_weighted.get_centers()==expected);
    auto copy=const_weighted.get_centers();
    copy.clear();
    require(const_weighted.get_centers()==expected);
    weighted.build(Graph(1));
    require(weighted.get_centers()==vl{0});
    weighted.build(g);
    require(weighted.get_centers()==expected);

    graph unweighted(g.size());
    Graph unit(g.size());
    rep(i,(ll)g.size())for(auto [j,w]:g[i]){
        unweighted[i].pb(j);
        unit[i].pb({j,1});
    }
    TreeDiameter tree(unweighted);
    const auto& const_tree=tree;
    expected=oracle(unit);
    require(const_tree.get_centers()==expected);
    copy=const_tree.get_centers();
    copy.pb(-1);
    require(const_tree.get_centers()==expected);
    tree.build(graph(1));
    require(tree.get_centers()==vl{0});
    tree.build(unweighted);
    require(tree.get_centers()==expected);
    ++cases;
}

void add_edge(Graph& g,ll u,ll v,ll w){
    g[u].pb({v,w});g[v].pb({u,w});
}

int main(){
    static_assert(is_same_v<decltype(declval<const TreeDiameter&>().get_centers()),vl>);
    static_assert(is_same_v<decltype(declval<const WeightedTreeDiameter&>().get_centers()),vl>);
    require(TreeDiameter().get_centers().empty());
    require(WeightedTreeDiameter().get_centers().empty());
    check(Graph(1));
    for(ll n=2;n<=6;++n){
        ll sequences=1;
        for(ll i=0;i<n-2;++i)sequences*=n;
        for(ll mask=0;mask<sequences;++mask){
            vl prufer(n-2),degree(n,1);
            ll code=mask;
            for(ll& v:prufer){v=code%n;code/=n;++degree[v];}
            Graph g(n);
            for(ll v:prufer){
                ll leaf=find(all(degree),1)-degree.begin();
                add_edge(g,leaf,v,(mask+leaf+v)%5);
                --degree[leaf];--degree[v];
            }
            ll u=find(all(degree),1)-degree.begin();
            degree[u]=0;
            ll v=find(all(degree),1)-degree.begin();
            add_edge(g,u,v,mask%5);
            check(g);
        }
    }
    mt19937_64 rng(4869);
    for(int test=0;test<3000;++test){
        ll n=1+rng()%40;
        Graph g(n);
        for(ll v=1;v<n;++v)add_edge(g,v,rng()%v,test%3==0?0:rng()%101);
        for(auto& edges:g)shuffle(all(edges),rng);
        check(g);
    }
    Graph large_weights(4);
    for(ll v=1;v<4;++v)add_edge(large_weights,v-1,v,3000000000000000000LL);
    check(large_weights);
    require(WeightedTreeDiameter(large_weights).get_centers()==vl({1,2}));

    const ll n=200000;
    graph path(n);
    for(ll v=1;v<n;++v){path[v-1].pb(v);path[v].pb(v-1);}
    TreeDiameter tree(path);
    require(tree.get_centers()==vl({n/2-1,n/2}));
    Graph zero_star(n);
    for(ll v=1;v<n;++v)add_edge(zero_star,0,v,0);
    WeightedTreeDiameter weighted(zero_star);
    vl all_vertices(n);iota(all(all_vertices),0);
    require(weighted.get_centers()==all_vertices);
    weighted.build(Graph(1));
    require(weighted.get_centers()==vl{0});
    cout<<"Passed "<<cases<<" all-pairs center oracle cases; copies, rebuilds, zero weights, large weights, and 200000-vertex trees\n";
}
