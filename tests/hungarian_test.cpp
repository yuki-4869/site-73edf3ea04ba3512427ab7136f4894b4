#include <bits/stdc++.h>
#include <atcoder/mincostflow>
#include "silent-check.hpp"
using namespace std;
using ll=long long;
using LL=__int128;
using vl=vector<ll>;
using vvl=vector<vl>;
#include "Graph/hungarian.cpp"
size_t brute_cases=0,flow_cases=0;
void require(bool ok){if(!ok)abort();}
LL brute(const vvl&a){
    if(a.empty())return 0;
    const size_t n=a.size(),m=a[0].size();
    vector<char> used(m);LL best=0;bool found=false;
    auto dfs=[&](auto&&self,size_t row,LL sum)->void{
        if(row==n){if(!found||sum<best){best=sum;found=true;}return;}
        for(size_t j=0;j<m;++j)if(!used[j]){used[j]=true;self(self,row+1,sum+LL(a[row][j]));used[j]=false;}
    };
    dfs(dfs,0,0);require(found);return best;
}
LL mincost_reference(const vvl&a){
    if(a.empty())return 0;
    const int n=int(a.size()),m=int(a[0].size()),source=n+m,sink=source+1;
    atcoder::mcf_graph<int,LL> flow(sink+1);
    LL bias=0;
    for(int i=0;i<n;++i){
        flow.add_edge(source,i,1,0);
        ll low=*min_element(a[i].begin(),a[i].end());bias+=LL(low);
        for(int j=0;j<m;++j)flow.add_edge(i,n+j,1,LL(a[i][j])-LL(low));
    }
    for(int j=0;j<m;++j)flow.add_edge(n+j,sink,1,0);
    auto [amount,cost]=flow.flow(source,sink,n);require(amount==n);return cost+bias;
}
void validate(const vvl&a,const HungarianResult&r){
    require(r.column.size()==a.size());LL actual=0;
    if(a.empty()){require(r.cost==0);return;}
    vector<char> used(a[0].size());
    for(size_t i=0;i<a.size();++i){
        ll j=r.column[i];require(j>=0&&size_t(j)<used.size());require(!used[size_t(j)]);used[size_t(j)]=true;
        actual+=LL(a[i][size_t(j)]);
    }
    require(actual==r.cost);
}
void check(const vvl&a,bool use_flow=false){
    auto original=a;const auto result=hungarian(a);validate(a,result);require(a==original);
    if(use_flow){require(result.cost==mincost_reference(a));++flow_cases;}
    else{require(result.cost==brute(a));++brute_cases;}
}
int main(){
    check({});
    const array<ll,3> vals={-1,0,2};
    for(int n=1;n<=3;++n)for(int m=n;m<=3;++m){
        size_t count=1;for(int i=0;i<n*m;++i)count*=3;
        for(size_t code=0;code<count;++code){
            size_t v=code;vvl a(n,vl(m));for(auto&row:a)for(auto&x:row){x=vals[v%3];v/=3;}
            check(a);
        }
    }
    mt19937_64 rng(4869);
    const array<ll,7> extreme={LLONG_MIN,LLONG_MIN+1,-1,0,1,LLONG_MAX-1,LLONG_MAX};
    for(int t=0;t<1500;++t){
        int n=1+int(rng()%5),m=n+int(rng()%(8-n));
        vvl a(n,vl(m));for(auto&row:a)for(auto&x:row)x=t%2?extreme[rng()%extreme.size()]:ll(rng()%201)-100;
        check(a);
    }
    for(int t=0;t<200;++t){
        int n=1+int(rng()%25),m=n+int(rng()%11);
        vvl a(n,vl(m));for(auto&row:a)for(auto&x:row)x=t%2?extreme[rng()%extreme.size()]:ll(rng()%2001)-1000;
        check(a,true);
    }
    check({{LLONG_MIN,LLONG_MAX},{LLONG_MAX,LLONG_MIN}});
    check({{LLONG_MAX,LLONG_MAX},{LLONG_MAX,LLONG_MAX}});
    check({{LLONG_MIN,LLONG_MIN},{LLONG_MIN,LLONG_MIN}});
    require(stops_silently([]{hungarian(vvl{{}});}));
    require(stops_silently([]{hungarian(vvl{{1},{2}});}));
    require(stops_silently([]{hungarian(vvl{{1,2},{3}});}));
    require(stops_silently([]{hungarian(vvl{{1,2},{3,4,5}});}));
    vvl tied(400,vl(400,7));auto r=hungarian(tied);validate(tied,r);require(r.cost==LL(2800));
    vvl rectangular(100,vl(5000,10));for(size_t i=0;i<rectangular.size();++i)rectangular[i][3*i]=-1;
    auto s=hungarian(rectangular);validate(rectangular,s);require(s.cost==LL(-100));
    cout<<"Passed "<<brute_cases<<" exhaustive-assignment comparisons and "<<flow_cases<<" ACL min-cost-flow comparisons; full ll extremes, 400x400/100x5000 matrices, 4 silent-stop cases\n";
}
