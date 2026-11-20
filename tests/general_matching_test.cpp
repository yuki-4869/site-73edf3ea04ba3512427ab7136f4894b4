#include <bits/stdc++.h>
#include "silent-check.hpp"
using namespace std;
using ll=long long;
using vl=vector<ll>;
using vp=vector<pair<ll,ll>>;
#include "Graph/general_matching.cpp"
#include "Graph/hopcroft_karp.cpp"

size_t checked=0,exhaustive_count=0;
void require(bool ok){if(!ok)abort();}
int brute(int n,const vp& edges){
    vector<unsigned> adjacent((size_t)n);
    for(auto [u,v]:edges)if(u!=v){adjacent[(size_t)u]|=1U<<v;adjacent[(size_t)v]|=1U<<u;}
    vector<int> memo(size_t(1)<<n,-1);
    memo[0]=0;
    auto solve=[&](auto&& self,unsigned mask)->int{
        int& answer=memo[mask];
        if(answer!=-1)return answer;
        int u=countr_zero(mask);unsigned rest=mask^(1U<<u);
        answer=self(self,rest);
        for(unsigned next=rest&adjacent[(size_t)u];next;next&=next-1){
            unsigned bit=next&(~next+1);
            answer=max(answer,1+self(self,rest^bit));
        }
        return answer;
    };
    return solve(solve,(1U<<n)-1);
}
GeneralMatchingResult check(int n,const vp& edges,int expected){
    auto original=edges;
    auto result=general_matching(n,edges);
    require(edges==original && result.cardinality==expected);
    require(result.mate.size()==(size_t)n && result.edge_ids.size()==(size_t)expected);
    vector<char> used((size_t)n);
    map<pair<ll,ll>,ll> first;
    for(size_t id=0;id<edges.size();++id){auto [u,v]=edges[id];if(u!=v)first.emplace(minmax(u,v),(ll)id);}
    ll previous=-1;
    for(ll id:result.edge_ids){
        require(id>previous && id>=0 && (size_t)id<edges.size());previous=id;
        auto [u,v]=edges[(size_t)id];
        require(u!=v && !used[(size_t)u] && !used[(size_t)v]);
        used[(size_t)u]=used[(size_t)v]=true;
        require(result.mate[(size_t)u]==v && result.mate[(size_t)v]==u);
        require(first.at(minmax(u,v))==id);
    }
    for(int v=0;v<n;++v){
        ll u=result.mate[(size_t)v];
        if(u==-1)require(!used[(size_t)v]);
        else require(u>=0 && u<n && u!=v && result.mate[(size_t)u]==v && used[(size_t)v]);
    }
    ++checked;
    return result;
}
void exhaustive(){
    for(int n=0;n<=6;++n){
        vp possible;
        for(int u=0;u<n;++u)for(int v=u+1;v<n;++v)possible.push_back({u,v});
        for(unsigned mask=0;mask<(1U<<possible.size());++mask){
            vp edges;for(size_t i=0;i<possible.size();++i)if(mask>>i&1)edges.push_back(possible[i]);
            int expected=brute(n,edges);check(n,edges,expected);++exhaustive_count;
            if(mask%17==0){
                vp duplicate;for(auto [u,v]:edges){duplicate.push_back({v,u});duplicate.push_back({u,v});duplicate.push_back({u,v});}
                for(int v=0;v<n;++v)duplicate.push_back({v,v});
                reverse(duplicate.begin(),duplicate.end());check(n,duplicate,expected);
            }
        }
    }
}
constexpr ll prime=1000000007;
ll inverse(ll a){ll result=1;for(ll e=prime-2;e;e>>=1){if(e&1)result=result*a%prime;a=a*a%prime;}return result;}
int tutte_rank(int n,const vp& edges,mt19937_64& gen){
    vector<vector<ll>> a((size_t)n,vector<ll>((size_t)n));
    for(auto [u,v]:edges)if(u!=v && a[(size_t)u][(size_t)v]==0){
        ll x=1+(ll)(gen()%(prime-1));a[(size_t)u][(size_t)v]=x;a[(size_t)v][(size_t)u]=prime-x;
    }
    int rank=0;
    for(int col=0;col<n && rank<n;++col){
        int pivot=rank;while(pivot<n && a[(size_t)pivot][(size_t)col]==0)++pivot;
        if(pivot==n)continue;
        swap(a[(size_t)pivot],a[(size_t)rank]);
        ll inv=inverse(a[(size_t)rank][(size_t)col]);
        for(int row=rank+1;row<n;++row){
            ll factor=a[(size_t)row][(size_t)col]*inv%prime;
            if(factor==0)continue;
            for(int j=col;j<n;++j)a[(size_t)row][(size_t)j]=(a[(size_t)row][(size_t)j]-factor*a[(size_t)rank][(size_t)j]%prime+prime)%prime;
        }
        ++rank;
    }
    return rank;
}
void random_cases(){
    mt19937_64 gen(4869);
    for(int run=0;run<10000;++run){
        int n=(int)(gen()%17),density=(int)(gen()%101);vp edges;
        for(int u=0;u<n;++u)for(int v=u+1;v<n;++v)if((int)(gen()%100)<density){
            edges.push_back(gen()%2?pair<ll,ll>{u,v}:pair<ll,ll>{v,u});
            if(gen()%7==0)edges.push_back({u,v});
        }
        if(n && gen()%2)edges.push_back({(ll)(gen()%(unsigned)n),(ll)(gen()%(unsigned)n)});
        shuffle(edges.begin(),edges.end(),gen);check(n,edges,brute(n,edges));
    }
    for(int run=0;run<300;++run){
        int l=(int)(gen()%41),r=(int)(gen()%41);vp edges;HopcroftKarp hk(l,r);
        for(int u=0;u<l;++u)for(int v=0;v<r;++v)if(gen()%4==0){edges.push_back({u,l+v});hk.add_edge(u,v);}
        shuffle(edges.begin(),edges.end(),gen);check(l+r,edges,hk.max_matching());
    }
    for(int run=0;run<150;++run){
        int n=16+(int)(gen()%65),density=(int)(gen()%101);vp edges;
        for(int u=0;u<n;++u)for(int v=u+1;v<n;++v)if((int)(gen()%100)<density)edges.push_back({u,v});
        shuffle(edges.begin(),edges.end(),gen);
        int rank=0;for(int t=0;t<3;++t)rank=max(rank,tutte_rank(n,edges,gen));
        require(rank%2==0);check(n,edges,rank/2);
    }
}
void known_cases(){
    vp petersen={{0,1},{1,2},{2,3},{3,4},{4,0},{0,5},{1,6},{2,7},{3,8},{4,9},{5,7},{7,9},{9,6},{6,8},{8,5}};
    mt19937_64 gen(1234567);
    for(int t=0;t<500;++t){
        vector<int> permutation(10);iota(permutation.begin(),permutation.end(),0);shuffle(permutation.begin(),permutation.end(),gen);
        vp edges;for(auto [u,v]:petersen)edges.push_back({permutation[(size_t)u],permutation[(size_t)v]});
        shuffle(edges.begin(),edges.end(),gen);check(10,edges,5);check(11,edges,5);
    }
    vp dense;for(int u=0;u<601;++u)for(int v=u+1;v<601;++v)dense.push_back({u,v});
    check(601,dense,300);
    vp long_path;
    for(int i=0;i<999;++i)long_path.push_back({2*i,2*i+1});
    for(int i=0;i<998;++i)long_path.push_back({2*i+1,2*i+2});
    long_path.push_back({1998,0});long_path.push_back({1997,1999});check(2000,long_path,1000);
    vp flowers;
    for(int i=0;i<500;++i){ll b=4*i;for(auto [u,v]:vp{{0,1},{1,2},{2,0},{2,3}})flowers.push_back({b+u,b+v});}
    check(2000,flowers,1000);
    vp chain;
    for(int i=0;i<200;++i){chain.push_back({2*i,2*i+1});chain.push_back({2*i+1,2*i+2});chain.push_back({2*i+2,2*i});}
    for(int t=0;t<20;++t){shuffle(chain.begin(),chain.end(),gen);check(401,chain,200);}
    chain.push_back({0,401});check(402,chain,201);
    vp parallel;for(int i=0;i<200000;++i)parallel.push_back({i%64,(i+1)%64});check(64,parallel,32);
    check(200000,vp{},0);
    auto saved=check(4,vp{{0,1},{1,2},{2,0},{2,3}},2);auto copy=saved;copy.mate[0]=-1;require(saved.mate[0]==1);
    require(stops_silently([]{(void)general_matching(-1,vp{});}));
    require(stops_silently([]{(void)general_matching((ll)INT_MAX+1,vp{});}));
    require(stops_silently([]{(void)general_matching(0,vp{{0,0}});}));
    require(stops_silently([]{(void)general_matching(2,vp{{-1,1}});}));
    require(stops_silently([]{(void)general_matching(2,vp{{0,2}});}));
}
int main(){
    exhaustive();random_cases();known_cases();
    cout<<"Passed "<<checked<<" graph checks, including "<<exhaustive_count<<" exhaustive simple graphs, subset-DP, Hopcroft-Karp and 3-trial Tutte-rank comparisons; Petersen relabelings, odd-cycle chains, 601-vertex clique, 2000-vertex augmenting path/flowers, duplicate edges, 200000 isolated vertices and silent-stop checks\n";
}
