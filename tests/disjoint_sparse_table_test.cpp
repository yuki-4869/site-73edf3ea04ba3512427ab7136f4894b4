#include <bits/stdc++.h>
#include "silent-check.hpp"
#include "DataStructureSegment/disjoint_sparse_table.cpp"

using ll=long long;
constexpr ll MOD=998244353;
std::uint64_t checks=0;
void require(bool ok){++checks;if(!ok)std::abort();}
ll sum_op(ll a,ll b){return a+b;}
ll sum_e(){return 0;}
ll min_op(ll a,ll b){return std::min(a,b);}
ll min_e(){return std::numeric_limits<ll>::max();}
using Sum=DisjointSparseTable<ll,sum_op,sum_e>;
using Minimum=DisjointSparseTable<ll,min_op,min_e>;

struct Affine{
    ll a,b;
    bool operator==(const Affine&)const=default;
};
Affine compose(Affine f,Affine g){return {g.a*f.a%MOD,(g.a*f.b+g.b)%MOD};}
Affine identity(){return {1,0};}
using Composition=DisjointSparseTable<Affine,compose,identity>;

struct Matrix{
    std::array<ll,4> a;
    bool operator==(const Matrix&)const=default;
};
Matrix multiply(Matrix x,Matrix y){
    Matrix z{{0,0,0,0}};
    for(int i=0;i<2;++i)for(int j=0;j<2;++j)for(int k=0;k<2;++k)
        z.a[2*i+j]=(z.a[2*i+j]+x.a[2*i+k]*y.a[2*k+j])%MOD;
    return z;
}
Matrix unit_matrix(){return {{1,0,0,1}};}
using Product=DisjointSparseTable<Matrix,multiply,unit_matrix>;
std::string concatenate(std::string a,std::string b){return a+b;}
std::string empty_string(){return {};}

struct NoDefault{
    ll value;
    NoDefault()=delete;
    explicit NoDefault(ll x):value(x){}
};
NoDefault wrapped_sum(NoDefault a,NoDefault b){return NoDefault(a.value+b.value);}
NoDefault wrapped_zero(){return NoDefault(0);}
using Wrapped=DisjointSparseTable<NoDefault,wrapped_sum,wrapped_zero>;
int calls=0;
ll counted_sum(ll a,ll b){++calls;return a+b;}

void all_intervals(const std::vector<ll>& a,const std::vector<Affine>& f,const std::vector<Matrix>& m){
    const int n=static_cast<int>(a.size());
    const Sum sum(a);
    const Minimum minimum(a);
    const Composition functions(f);
    const Product matrices(m);
    require(sum.size()==n);
    ll total=0;
    for(int i=0;i<n;++i){require(sum.get(i)==a[i]);total+=a[i];}
    require(sum.all_prod()==total);
    for(int l=0;l<=n;++l){
        ll s=0,mn=min_e();
        Affine composed=identity();
        Matrix product=unit_matrix();
        for(int r=l;r<=n;++r){
            require(sum.prod(l,r)==s);
            require(minimum.prod(l,r)==mn);
            require(functions.prod(l,r)==composed);
            require(matrices.prod(l,r)==product);
            if(r<n){
                s+=a[r];mn=std::min(mn,a[r]);
                composed=compose(composed,f[r]);
                product=multiply(product,m[r]);
            }
        }
    }
}

int main(){
    Sum empty;
    require(empty.size()==0&&empty.prod(0,0)==0&&empty.all_prod()==0);
    Minimum none(std::vector<ll>{});
    require(none.all_prod()==min_e());
    Wrapped no_default;
    require(no_default.all_prod().value==0);
    no_default.build(std::vector<NoDefault>{NoDefault(3),NoDefault(7),NoDefault(-2)});
    require(no_default.prod(0,3).value==8);
    require(no_default.get(1).value==7);

    Composition fixed(std::vector<Affine>{{2,1},{3,4},{5,6}});
    require(fixed.prod(0,3)==Affine{30,41});
    require(fixed.prod(0,3).a*7+fixed.prod(0,3).b==251);
    require(fixed.prod(1,3)==Affine{15,26});
    DisjointSparseTable<std::string,concatenate,empty_string> text(std::vector<std::string>{"ab","C","de","F","g"});
    for(int l=0;l<=5;++l)for(int r=l;r<=5;++r){
        std::string expected;
        const std::vector<std::string> a{"ab","C","de","F","g"};
        for(int i=l;i<r;++i)expected+=a[i];
        require(text.prod(l,r)==expected);
    }

    std::mt19937_64 rng(20261021);
    for(int trial=0;trial<600;++trial){
        const int n=trial<130?trial:static_cast<int>(rng()%100);
        std::vector<ll> a(n);
        std::vector<Affine> f(n);
        std::vector<Matrix> m(n);
        for(int i=0;i<n;++i){
            a[i]=static_cast<ll>(rng()%2001)-1000;
            f[i]={static_cast<ll>(rng()%MOD),static_cast<ll>(rng()%MOD)};
            for(auto& x:m[i].a)x=static_cast<ll>(rng()%MOD);
        }
        all_intervals(a,f,m);
    }

    Sum reused;
    for(int n:{0,1,17,128,3,255,256,257,2,0,1}){
        std::vector<ll> a(n);
        for(auto& x:a)x=static_cast<ll>(rng()%101)-50;
        reused.build(a);
        require(reused.size()==n);
        Sum copied=reused;
        Sum assigned;
        assigned=reused;
        if(n){const ll old=a[0];a[0]+=100;require(reused.get(0)==old);a[0]=old;}
        for(int l=0;l<=n;++l){
            ll expected=0;
            for(int r=l;r<=n;++r){
                require(reused.prod(l,r)==expected);
                require(copied.prod(l,r)==expected);
                require(assigned.prod(l,r)==expected);
                if(r<n)expected+=a[r];
            }
        }
    }

    for(int k=1;k<=18;++k)for(int delta:{-1,0,1}){
        const int n=(1<<k)+delta;
        std::vector<ll> a(n);
        for(int i=0;i<n;++i)a[i]=(i%11)-5;
        Sum table(a);
        std::vector<ll> prefix(n+1);
        std::partial_sum(a.begin(),a.end(),prefix.begin()+1);
        for(int t=0;t<200;++t){
            int l=static_cast<int>(rng()%static_cast<unsigned>(n+1));
            int r=static_cast<int>(rng()%static_cast<unsigned>(n+1));
            if(l>r)std::swap(l,r);
            require(table.prod(l,r)==prefix[r]-prefix[l]);
        }
        require(table.all_prod()==prefix[n]);
    }

    DisjointSparseTable<ll,counted_sum,sum_e> counted(std::vector<ll>(513,1));
    for(int l=0;l<=513;++l)for(int r=l;r<=513;++r){
        calls=0;
        require(counted.prod(l,r)==r-l);
        require(calls==(r-l>=2?1:0));
    }

    Sum bounds(std::vector<ll>{1,2,3});
    require(stops_silently([&]{empty.get(0);}));
    require(stops_silently([&]{bounds.get(-1);}));
    require(stops_silently([&]{bounds.get(3);}));
    require(stops_silently([&]{bounds.prod(-1,1);}));
    require(stops_silently([&]{bounds.prod(0,-1);}));
    require(stops_silently([&]{bounds.prod(2,1);}));
    require(stops_silently([&]{bounds.prod(0,4);}));
    require(stops_silently([&]{bounds.prod(4,4);}));
    std::cout<<"Disjoint Sparse Table: "<<checks<<" checks passed.\n";
}
