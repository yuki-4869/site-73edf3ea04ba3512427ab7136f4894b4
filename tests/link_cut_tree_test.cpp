#include <bits/stdc++.h>
#include "silent-check.hpp"
using namespace std;
using ll=long long;
#include "Tree/link_cut_tree_basic.cpp"
#include "Tree/link_cut_tree.cpp"
#include "Tree/lazy_link_cut_tree.cpp"
size_t checks=0;
void require(bool ok,source_location at=source_location::current()){
    ++checks;
    if(!ok){cerr<<"Failed line "<<at.line()<<'\n';abort();}
}
struct Forest{
    vector<vector<int>> g;
    explicit Forest(int n):g(n){}
    vector<int> path(int u,int v)const{
        vector<int> p(g.size(),-1),q{u};p[u]=u;
        for(size_t i=0;i<q.size();++i)for(int w:g[q[i]])if(p[w]<0){p[w]=q[i];q.push_back(w);}
        if(p[v]<0)return {};
        vector<int> out;
        for(int x=v;;x=p[x]){out.push_back(x);if(x==u)break;}
        reverse(out.begin(),out.end());return out;
    }
    bool link(int u,int v){if(!path(u,v).empty())return false;g[u].push_back(v);g[v].push_back(u);return true;}
    bool cut(int u,int v){
        auto it=find(g[u].begin(),g[u].end(),v);
        if(it==g[u].end())return false;
        g[u].erase(it);g[v].erase(find(g[v].begin(),g[v].end(),u));return true;
    }
    int lca(int r,int u,int v)const{
        auto a=path(r,u),b=path(r,v);int answer=-1;
        for(size_t i=0;i<min(a.size(),b.size())&&a[i]==b[i];++i)answer=a[i];
        return a.empty()||b.empty()?-1:answer;
    }
};
template<class T>
void structural(T&t,const Forest&f,int r,int u,int v){
    auto p=f.path(u,v),rv=f.path(r,v);const ll n=ll(p.size());
    require(t.same(u,v)==!p.empty());require(t.path_size(u,v)==n);require(t.dist_edges(u,v)==n-1);
    require(t.lca(r,u,v)==f.lca(r,u,v));
    require(t.parent(r,v)==(rv.size()<2?-1:rv[rv.size()-2]));
    for(ll k:vector<ll>{-1,0,n/2,n-1,n,LLONG_MAX}){
        require(t.kth(u,v,k)==(k<0||k>=n?-1:p[size_t(k)]));
        require(t.jump(u,v,k)==(k<0||k>=n?-1:p[size_t(k)]));
        require(t.kth_ancestor(r,v,k)==(k<0||k>=ll(rv.size())?-1:rv[rv.size()-1-size_t(k)]));
    }
}
void exhaustive(){
    for(int n=1;n<=5;++n){
        vector<pair<int,int>> edges;
        for(int u=0;u<n;++u)for(int v=u+1;v<n;++v)edges.emplace_back(u,v);
        for(unsigned mask=0;mask<(1U<<edges.size());++mask){
            Forest f(n);LinkCutTreeBasic basic(n);LCTSum sum(vector<ll>(n,1));LCTAddSum lazy(vector<ll>(n,1));
            bool valid=true;
            for(size_t i=0;i<edges.size();++i)if((mask>>i)&1){
                auto [u,v]=edges[i];if(!f.link(u,v)){valid=false;break;}
                require(basic.link(u,v));require(sum.link(u,v));require(lazy.link(u,v));
            }
            if(!valid)continue;
            for(int r=0;r<n;++r)for(int u=0;u<n;++u)for(int v=0;v<n;++v){
                structural(basic,f,r,u,v);structural(sum,f,r,u,v);structural(lazy,f,r,u,v);
                auto p=f.path(u,v);
                if(!p.empty()){
                    require(sum.prod(u,v)==ll(p.size()));require(lazy.prod(u,v)==ll(p.size()));
                    for(ll k=0;k<=ll(p.size());++k){
                        require(sum.max_right(u,v,[&](ll x){return x<=k;})==k);
                        require(lazy.max_right(u,v,[&](ll x){return x<=k;})==k);
                    }
                }
            }
            for(int u=0;u<n;++u)for(int v=0;v<n;++v){
                bool result=f.cut(u,v);
                require(basic.cut(u,v)==result);require(sum.cut(u,v)==result);require(lazy.cut(u,v)==result);
            }
        }
    }
}
constexpr ll MOD=998244353;
ll power(ll x,ll k){ll r=1;for(;k;k>>=1,x=x*x%MOD)if(k&1)r=r*x%MOD;return r;}
struct Mat{
    ll a,b,c,d;
    bool operator==(const Mat&)const=default;
};
Mat mul(Mat x,Mat y){return {(x.a*y.a+x.b*y.c)%MOD,(x.a*y.b+x.b*y.d)%MOD,(x.c*y.a+x.d*y.c)%MOD,(x.c*y.b+x.d*y.d)%MOD};}
Mat one(){return {1,0,0,1};}
struct Conjugation{Mat m,inv;};
Mat conjugate(Conjugation f,Mat x){return mul(mul(f.m,x),f.inv);}
Conjugation compose(Conjugation f,Conjugation g){return {mul(f.m,g.m),mul(g.inv,f.inv)};}
Conjugation conjugation_id(){return {one(),one()};}
Conjugation random_tag(mt19937_64&rng){
    for(;;){
        Mat a{ll(rng()%MOD),ll(rng()%MOD),ll(rng()%MOD),ll(rng()%MOD)};
        ll det=(a.a*a.d-a.b*a.c+MOD*MOD)%MOD;
        if(!det)continue;
        ll inv=power(det,MOD-2);
        return {a,{a.d*inv%MOD,(MOD-a.b)*inv%MOD,(MOD-a.c)*inv%MOD,a.a*inv%MOD}};
    }
}
struct Sum{ll sum,len;bool operator==(const Sum&)const=default;};
struct Affine{ll a,b;};
Sum add_sum(Sum a,Sum b){return {(a.sum+b.sum)%MOD,a.len+b.len};}
Sum empty_sum(){return {0,0};}
Sum map_sum(Affine f,Sum x){return {(f.a*x.sum+f.b*x.len)%MOD,x.len};}
Affine compose_affine(Affine f,Affine g){return {f.a*g.a%MOD,(f.a*g.b+f.b)%MOD};}
Affine id_affine(){return {1,0};}
void randomized(){
    mt19937_64 rng(4869);
    for(int trial=0;trial<100;++trial){
        int n=1+int(rng()%50);Forest f(n);
        vector<ll> values(n),mod_values(n);vector<Mat> matrices(n);vector<Sum> sums(n);
        for(int v=0;v<n;++v){
            values[v]=ll(rng()%100);mod_values[v]=ll(rng()%MOD);sums[v]={mod_values[v],1};
            matrices[v]=random_tag(rng).m;
        }
        LinkCutTreeBasic basic(n);LCTSum normal(values);LCTAddSum lazy(values);
        LCTMin minimum(values);LCTMax maximum(values);LCTAssignMin assign(values);
        LinkCutTree<Mat,mul,one> noncomm(matrices);
        LazyLinkCutTree<Mat,mul,one,Conjugation,conjugate,compose,conjugation_id> noncomm_lazy(matrices);
        LazyLinkCutTree<Sum,add_sum,empty_sum,Affine,map_sum,compose_affine,id_affine> affine(sums);
        auto each=[&](auto operation){operation(basic);operation(normal);operation(lazy);operation(minimum);operation(maximum);operation(assign);operation(noncomm);operation(noncomm_lazy);operation(affine);};
        vector<ll> assigned=values;
        if(trial%2==0)for(int v=1;v<n;++v){
            int p=trial%4==0?v-1:0;require(f.link(p,v));each([&](auto&t){require(t.link(p,v));});
        }
        for(int step=0;step<2000;++step){
            int u=int(rng()%n),v=int(rng()%n),r=int(rng()%n),type=int(rng()%10);
            if(type==2&&!f.g[u].empty())v=f.g[u][rng()%f.g[u].size()];
            auto p=f.path(u,v);
            if(type<=2){
                if(type==0){bool expected=f.link(u,v);each([&](auto&t){require(t.link(u,v)==expected);});}
                else{bool expected=f.cut(u,v);each([&](auto&t){require(t.cut(u,v)==expected);});}
            }else if(type==3){
                values[u]=ll(rng()%100);assigned[u]=ll(rng()%100)-50;
                normal.set(u,values[u]);lazy.set(u,values[u]);minimum.set(u,values[u]);maximum.set(u,values[u]);assign.set(u,assigned[u]);
                matrices[u]=random_tag(rng).m;noncomm.set(u,matrices[u]);noncomm_lazy.set(u,matrices[u]);
                mod_values[u]=ll(rng()%MOD);affine.set(u,{mod_values[u],1});
            }else if(type<=5&&!p.empty()){
                ll delta=ll(rng()%20);lazy.apply(u,v,delta);
                auto tag=random_tag(rng);noncomm_lazy.apply(u,v,tag);
                Affine ftag{ll(rng()%MOD),ll(rng()%MOD)};affine.apply(u,v,ftag);
                ll replacement=ll(rng()%200)-100;assign.apply(u,v,replacement);
                for(int x:p){
                    values[x]+=delta;normal.set(x,values[x]);minimum.set(x,values[x]);maximum.set(x,values[x]);assigned[x]=replacement;
                    matrices[x]=conjugate(tag,matrices[x]);noncomm.set(x,matrices[x]);mod_values[x]=(ftag.a*mod_values[x]+ftag.b)%MOD;
                }
            }else if(type==6){
                ll delta=ll(rng()%20);lazy.apply(u,delta);values[u]+=delta;
                normal.set(u,values[u]);minimum.set(u,values[u]);maximum.set(u,values[u]);
                assign.apply(u,ll(-77));assigned[u]=-77;
                auto tag=random_tag(rng);noncomm_lazy.apply(u,tag);matrices[u]=conjugate(tag,matrices[u]);noncomm.set(u,matrices[u]);
                Affine ftag{ll(rng()%MOD),ll(rng()%MOD)};affine.apply(u,ftag);mod_values[u]=(ftag.a*mod_values[u]+ftag.b)%MOD;
            }else{
                structural(basic,f,r,u,v);structural(normal,f,r,u,v);structural(lazy,f,r,u,v);
                require(normal.get(u)==values[u]);require(lazy.get(u)==values[u]);require(assign.get(u)==assigned[u]);
                require(noncomm.get(u)==matrices[u]);require(noncomm_lazy.get(u)==matrices[u]);require(affine.get(u)==Sum{mod_values[u],1});
                if(!p.empty()){
                    ll sum=0,mn=LLONG_MAX,mx=LLONG_MIN,am=LLONG_MAX,ms=0;Mat product=one();
                    for(int x:p){sum+=values[x];mn=min(mn,values[x]);mx=max(mx,values[x]);am=min(am,assigned[x]);product=mul(product,matrices[x]);ms=(ms+mod_values[x])%MOD;}
                    require(normal.prod(u,v)==sum);require(lazy.prod(u,v)==sum);require(minimum.prod(u,v)==mn);require(maximum.prod(u,v)==mx);require(assign.prod(u,v)==am);
                    require(noncomm.prod(u,v)==product);require(noncomm_lazy.prod(u,v)==product);require(affine.prod(u,v)==Sum{ms,ll(p.size())});
                    ll length_bound=ll(rng()%(p.size()+1));
                    require(affine.max_right(u,v,[&](Sum x){return x.len<=length_bound;})==length_bound);
                    ll bound=ll(rng()%1000),prefix=0,total=0;
                    for(int x:p){if(total+values[x]>bound)break;total+=values[x];++prefix;}
                    require(normal.max_right(u,v,[&](ll x){return x<=bound;})==prefix);
                    require(lazy.max_right(u,v,[&](ll x){return x<=bound;})==prefix);
                }
            }
            if(step%211==0){
                auto copied=lazy;auto old=values;copied.set(u,1234567);require(lazy.get(u)==old[u]);require(copied.get(u)==1234567);
                auto bc=basic;require(bc.path_size(u,v)==ll(f.path(u,v).size()));
                auto nc=noncomm_lazy;nc.set(u,one());require(noncomm_lazy.get(u)==matrices[u]);
            }
        }
    }
}
void edge_cases(){
    LinkCutTreeBasic empty;LCTSum es;LCTAddSum el;require(empty.size()==0&&es.size()==0&&el.size()==0);
    LCTAddSum zeros(3);require(zeros.link(0,1)&&zeros.link(1,2));zeros.apply(0,2,7);require(zeros.prod(0,2)==21);
    LCTMin mn(1);LCTMax mx(1);LCTAssignMin assign(1);
    require(mn.get(0)==LLONG_MAX&&mx.get(0)==LLONG_MIN);assign.apply(0,LLONG_MIN);require(assign.get(0)==LLONG_MIN);
    assign.apply(0,LLONG_MAX);require(assign.get(0)==LLONG_MAX);assign.apply(0,nullopt);require(assign.get(0)==LLONG_MAX);
    require(stops_silently([]{LinkCutTreeBasic bad(-1);}));require(stops_silently([]{LCTSum bad(ll(INT_MAX)+1);}));
    require(stops_silently([]{LCTAddSum bad(-1);}));require(stops_silently([]{LCTAddSum bad(ll(INT_MAX)+1);}));
    require(stops_silently([]{LinkCutTreeBasic t(1);t.same(0,1);}));require(stops_silently([]{LCTSum t(2);t.prod(0,1);}));
    require(stops_silently([]{LCTAddSum t(2);t.apply(0,1,5);}));require(stops_silently([]{LCTSum t(1);t.max_right(0,0,[](ll){return false;});}));
    require(stops_silently([]{LCTAddSum t(1);t.get(-1);}));
    require(stops_silently([]{LCTAddSum t(2);t.max_right(0,1,[](ll){return true;});}));
    LinkCutTree<string,[](string a,string b){return a+b;},[]{return string{};}> text(vector<string>{"a","b","c","d"});
    for(int v=1;v<4;++v)require(text.link(v-1,v));
    require(text.prod(0,3)=="abcd"&&text.prod(3,0)=="dcba");
    require(text.max_right(3,0,[](const string&s){return string("dcb").starts_with(s);})==3);
    auto copy=text;require(copy.cut(1,2));require(text.same(0,3)&&!copy.same(0,3));
}
void large(){
    constexpr int n=200000;LinkCutTreeBasic basic(n);LCTSum normal(vector<ll>(n,1));LCTAddSum lazy(n);
    for(int v=1;v<n;++v){require(basic.link(v-1,v));require(normal.link(v-1,v));require(lazy.link(v-1,v));}
    for(int step=0;step<3000;++step){
        int u=(step*367)%n,v=n-1-u;
        require(basic.path_size(u,v)==abs(u-v)+1);require(normal.prod(u,v)==abs(u-v)+1);
        require(basic.kth(u,v,abs(u-v))==v);require(basic.lca(0,u,v)==min(u,v));
        lazy.apply(0,n-1,1);require(lazy.prod(0,n-1)==ll(step+1)*n);
        require(lazy.max_right(0,n-1,[&](ll x){return x<=ll(step+1)*12345;})==12345);
    }
    for(int v=1;v<n;++v){require(basic.cut(v-1,v));require(normal.cut(v-1,v));require(lazy.cut(v-1,v));}
    for(int v=1;v<n;++v){require(basic.link(0,v));require(normal.link(0,v));require(lazy.link(0,v));}
    require(basic.path_size(1,n-1)==3);require(normal.prod(1,n-1)==3);require(lazy.prod(1,n-1)==9000);
}
int main(){
    edge_cases();exhaustive();randomized();large();
    cout<<"Passed "<<checks<<" LCT checks: exhaustive forests up to 5 vertices, 200000 mixed random operations, noncommutative matrix products/conjugation, affine tag order, copies, 200000-node chain/star, silent invalid-input checks\n";
}
