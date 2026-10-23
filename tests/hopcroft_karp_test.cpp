#include <bits/stdc++.h>
#include <atcoder/maxflow>
#include "silent-check.hpp"
using ll=long long;
#include "Graph/hopcroft_karp.cpp"

std::uint64_t checks=0,graphs=0;
void require(bool ok,std::source_location at=std::source_location::current()){
    ++checks;
    if(!ok){std::cerr<<"check failed at line "<<at.line()<<'\n';std::abort();}
}
using Matrix=std::vector<std::vector<char>>;

int brute_matching(const Matrix&g,int nr){
    std::vector<int>dp(std::size_t(1)<<nr,-1000),next;
    dp[0]=0;
    for(const auto&row:g){
        next=dp;
        for(int mask=0;mask<(1<<nr);++mask)if(dp[mask]>=0)
            for(int v=0;v<nr;++v)if(row[v]&&!(mask>>v&1))
                next[mask|(1<<v)]=std::max(next[mask|(1<<v)],dp[mask]+1);
        dp.swap(next);
    }
    return *std::max_element(dp.begin(),dp.end());
}

int flow_matching(const Matrix&g,int nr){
    const int nl=static_cast<int>(g.size()),source=nl+nr,sink=source+1;
    atcoder::mf_graph<int>flow(sink+1);
    for(int u=0;u<nl;++u){
        flow.add_edge(source,u,1);
        for(int v=0;v<nr;++v)if(g[u][v])flow.add_edge(u,nl+v,1);
    }
    for(int v=0;v<nr;++v)flow.add_edge(nl+v,sink,1);
    return flow.flow(source,sink);
}

void inspect(HopcroftKarp&hk,const Matrix&g,int nr,int optimum){
    ++graphs;
    const int nl=static_cast<int>(g.size());
    require(hk.left_size()==nl&&hk.right_size()==nr);
    auto pairs=hk.matching();
    require(static_cast<int>(pairs.size())==optimum);
    require(hk.max_matching()==optimum&&hk.max_matching()==optimum);
    require(std::is_sorted(pairs.begin(),pairs.end()));
    std::vector<int>left(nl,-1),right(nr,-1);
    for(auto[u,v]:pairs){
        require(0<=u&&u<nl&&0<=v&&v<nr&&g[u][v]);
        require(left[u]==-1&&right[v]==-1);
        left[u]=v;right[v]=u;
    }
    for(int u=0;u<nl;++u)require(hk.mate_left(u)==left[u]);
    for(int v=0;v<nr;++v)require(hk.mate_right(v)==right[v]);
    auto [cover_left,cover_right]=hk.min_vertex_cover();
    require(static_cast<int>(cover_left.size()+cover_right.size())==optimum);
    require(std::is_sorted(cover_left.begin(),cover_left.end()));
    require(std::is_sorted(cover_right.begin(),cover_right.end()));
    std::vector<char>cl(nl),cr(nr);
    for(int u:cover_left){require(0<=u&&u<nl&&!cl[u]);cl[u]=true;}
    for(int v:cover_right){require(0<=v&&v<nr&&!cr[v]);cr[v]=true;}
    for(int u=0;u<nl;++u)for(int v=0;v<nr;++v)if(g[u][v])require(cl[u]||cr[v]);
    require(pairs==hk.matching());
    pairs.clear();
    require(hk.max_matching()==optimum);
}

void exhaustive(){
    for(int nl=0;nl<=4;++nl)for(int nr=0;nr<=4;++nr){
        const unsigned count=1U<<(nl*nr);
        for(unsigned mask=0;mask<count;++mask){
            Matrix g(nl,std::vector<char>(nr));
            HopcroftKarp hk(nl,nr);
            for(int u=0;u<nl;++u)for(int v=0;v<nr;++v)if(mask>>(u*nr+v)&1U){
                g[u][v]=true;
                hk.add_edge(u,v);
            }
            const int optimum=brute_matching(g,nr);
            if(mask%3==0)hk.min_vertex_cover();
            else if(mask%3==1&&nl)hk.mate_left(0);
            else if(nr)hk.mate_right(nr-1);
            inspect(hk,g,nr,optimum);
        }
    }
}

void incremental(){
    std::mt19937_64 rng(20261023);
    for(int trial=0;trial<300;++trial){
        const int nl=1+static_cast<int>(rng()%30),nr=1+static_cast<int>(rng()%30);
        Matrix g(nl,std::vector<char>(nr));
        HopcroftKarp hk(nl,nr);
        int previous=0;
        for(int step=0;step<120;++step){
            const int u=static_cast<int>(rng()%static_cast<unsigned>(nl));
            const int v=static_cast<int>(rng()%static_cast<unsigned>(nr));
            hk.add_edge(u,v);
            if(step%5==0)hk.add_edge(u,v);
            g[u][v]=true;
            if(step%4==3){
                const int optimum=flow_matching(g,nr);
                require(optimum>=previous);
                inspect(hk,g,nr,optimum);
                previous=optimum;
            }
        }
        HopcroftKarp copy=hk;
        inspect(copy,g,nr,previous);
        copy=copy;
        inspect(copy,g,nr,previous);
        HopcroftKarp assigned;
        assigned=hk;
        inspect(assigned,g,nr,previous);
        HopcroftKarp moved(std::move(assigned));
        inspect(moved,g,nr,previous);
        inspect(assigned,Matrix{},0,0);
        assigned=std::move(moved);
        inspect(assigned,g,nr,previous);
        inspect(moved,Matrix{},0,0);
        HopcroftKarp*alias=&assigned;
        assigned=std::move(*alias);
        inspect(assigned,g,nr,previous);
    }
    for(int trial=0;trial<1200;++trial){
        const int nl=static_cast<int>(rng()%81),nr=static_cast<int>(rng()%81);
        Matrix g(nl,std::vector<char>(nr));
        HopcroftKarp hk(nl,nr);
        if(nl&&nr)for(int i=0,m=static_cast<int>(rng()%500);i<m;++i){
            const int u=static_cast<int>(rng()%static_cast<unsigned>(nl)),v=static_cast<int>(rng()%static_cast<unsigned>(nr));
            hk.add_edge(u,v);g[u][v]=true;
        }
        inspect(hk,g,nr,flow_matching(g,nr));
    }
}

void boundaries(){
    HopcroftKarp empty;
    inspect(empty,Matrix{},0,0);
    HopcroftKarp no_left(0,5),no_right(5,0);
    inspect(no_left,Matrix{},5,0);
    inspect(no_right,Matrix(5),0,0);
    HopcroftKarp small(3,3);
    small.add_edge(0,0);small.add_edge(0,1);small.add_edge(1,0);
    require(small.max_matching()==2);
    auto copied=small;
    copied.add_edge(2,2);
    require(copied.max_matching()==3&&small.max_matching()==2);
    auto cover=copied.min_vertex_cover();
    require(cover.first==std::vector<int>({0,1,2})&&cover.second.empty());
    require(copied.matching()==std::vector<std::pair<int,int>>({{0,1},{1,0},{2,2}}));
    HopcroftKarp survivor;
    {
        HopcroftKarp temporary(1,1);
        temporary.add_edge(0,0);
        survivor=temporary;
    }
    require(survivor.mate_left(0)==0);
    require(stops_silently([]{HopcroftKarp invalid(-1,0);}));
    require(stops_silently([]{HopcroftKarp invalid(0,-1);}));
    require(stops_silently([&]{empty.add_edge(0,0);}));
    require(stops_silently([&]{small.add_edge(-1,0);}));
    require(stops_silently([&]{small.add_edge(0,-1);}));
    require(stops_silently([&]{small.add_edge(3,0);}));
    require(stops_silently([&]{small.add_edge(0,3);}));
    require(stops_silently([&]{small.mate_left(-1);}));
    require(stops_silently([&]{small.mate_left(3);}));
    require(stops_silently([&]{small.mate_right(-1);}));
    require(stops_silently([&]{small.mate_right(3);}));
    require(stops_silently([&]{empty.mate_left(0);}));
    require(stops_silently([&]{empty.mate_right(0);}));
}

void long_path(){
    const int n=200000;
    HopcroftKarp hk(n,n);
    for(int u=0;u<n-1;++u){hk.add_edge(u,u);hk.add_edge(u,u+1);}
    hk.add_edge(n-1,0);
    require(hk.max_matching()==n);
    for(int u=0;u<n-1;++u)require(hk.mate_left(u)==u+1);
    require(hk.mate_left(n-1)==0&&hk.mate_right(0)==n-1);
    auto [left,right]=hk.min_vertex_cover();
    require(left.size()==static_cast<std::size_t>(n)&&right.empty());
    HopcroftKarp star(n,1);
    for(int u=0;u<n;++u)star.add_edge(u,0);
    require(star.max_matching()==1);
    auto cover=star.min_vertex_cover();
    require(cover.first.empty()&&cover.second==std::vector<int>{0});
}

int main(){
    exhaustive();incremental();boundaries();long_path();
    std::cout<<"Hopcroft-Karp: "<<checks<<" checks, "<<graphs<<" graph states; exhaustive up to 4x4, 36000 incremental additions, ACL max-flow comparison, 200000-per-side alternating chain passed.\n";
}
