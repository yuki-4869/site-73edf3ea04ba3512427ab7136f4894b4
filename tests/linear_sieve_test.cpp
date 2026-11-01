#include <bits/stdc++.h>
#include <atcoder/modint>
using namespace std;
using ll=long long;
using vl=vector<ll>;
using vp=vector<pair<ll,ll>>;
using mint=atcoder::modint998244353;
using vmi=vector<mint>;
#define rep(i,n) for(ll i=0;i<(ll)(n);++i)
#define pb push_back
#define all(a) (a).begin(),(a).end()
namespace standalone {
#include "MathNumberTheory/linear_sieve.cpp"
}
namespace bundled {
#include "MathNumberTheory/number_theory_bundle.cpp"
}

void require(bool ok){if(!ok)abort();}
size_t cases=0;

vp full_factor(ll x){
    vp result;
    for(ll p=2;p<=x/p;++p){
        ll e=0;
        while(x%p==0){x/=p;++e;}
        if(e)result.pb({p,e});
    }
    if(x>1)result.pb({x,1});
    return result;
}

vp expected_factor(const vp& exact,ll n){
    vp result;
    __int128 remaining=1;
    for(auto [p,e]:exact){
        if(p<=n)result.pb({p,e});
        else for(ll i=0;i<e;++i)remaining*=p;
    }
    require(remaining<=LLONG_MAX);
    if(remaining>1)result.pb({(ll)remaining,1});
    return result;
}

void check(standalone::LinearSieve& a,const bundled::LinearSieve& b,ll x,const vp& exact){
    auto expected=expected_factor(exact,a.n);
    auto actual=a.factorize(x);
    require(actual==expected&&b.factor(x)==expected);
    __int128 product=1;
    ll previous=1;
    for(auto [p,e]:actual){
        require(p>previous&&e>0&&e<=63);previous=p;
        for(ll i=0;i<e;++i){product*=p;require(product<=x);}
    }
    require(product==x);
    if((__int128)x<=(__int128)a.n*a.n)require(actual==exact);
    ++cases;
}

vl full_divisors(ll x){
    vl result;
    for(ll d=1;d<=x/d;++d)if(x%d==0){
        result.pb(d);
        if(d!=x/d)result.pb(x/d);
    }
    sort(all(result));return result;
}

int main(){
    for(ll n=1;n<=64;++n){
        standalone::LinearSieve a(n);
        const bundled::LinearSieve b(n);
        auto spf=a.spf,phi=a.phi,mu=a.mu,mn=b.mn,pi=b.pi;
        for(ll x=1;x<=4*n*n+3;++x){
            check(a,b,x,full_factor(x));
            if(n<=24&&x<=n*n){
                auto d=a.divisors(x);sort(all(d));
                require(d==full_divisors(x)&&b.divisors(x)==d);
            }
        }
        require(a.spf==spf&&a.phi==phi&&a.mu==mu&&b.mn==mn&&b.pi==pi);
    }

    const vp max_factors={{7,2},{73,1},{127,1},{337,1},{92737,1},{649657,1}};
    __int128 max_product=1;
    for(auto [p,e]:max_factors){
        require(full_factor(p)==vp({{p,1}}));
        for(ll i=0;i<e;++i)max_product*=p;
    }
    require(max_product==LLONG_MAX);
    ll pow3=1;for(int i=0;i<39;++i)pow3*=3;
    mt19937_64 rng(4869);
    for(ll n:{1LL,2LL,10LL,31LL,1000LL,1000000LL}){
        standalone::LinearSieve a(n);
        const bundled::LinearSieve b(n);
        check(a,b,LLONG_MAX,max_factors);
        check(a,b,1LL<<62,{{2,62}});
        check(a,b,pow3,{{3,39}});
        check(a,b,2305843009213693951LL,{{2305843009213693951LL,1}});
        check(a,b,999983LL*999983,{{999983,2}});
        check(a,b,999983LL*1000003,{{999983,1},{1000003,1}});
        check(a,b,1000003LL*1000003,{{1000003,2}});
        check(a,b,2LL*1000003*1000003,{{2,1},{1000003,2}});
        check(a,b,n*n,full_factor(n*n));
        check(a,b,n*n+1,full_factor(n*n+1));
        for(int test=0;test<2000;++test){
            ll x=1+rng()%10000000;
            check(a,b,x,full_factor(x));
        }
        if(n==1000000){
            require(a.factorize(2LL*999983)==vp({{2,1},{999983,1}}));
            require(b.factor(72)==vp({{2,3},{3,2}}));
        }
    }

    standalone::LinearSieve a(10);
    const bundled::LinearSieve b(10);
    for(ll x:{10,11,14,32,72,97,98,100,101,121,143,242,286,605})check(a,b,x,full_factor(x));
    require(a.factorize(72)==vp({{2,3},{3,2}}));
    require(a.factorize(121)==vp({{121,1}}));
    require(a.factorize(242)==vp({{2,1},{121,1}}));
    require(a.divisors(143)==vl({1,143})&&b.divisors(143)==vl({1,143}));
    require(a.isPrime(7)&&!a.isPrime(1)&&b.is_prime(7)&&!b.is_prime(1));
    require(b.factor_list(8)==vl({2,2,2})&&b.factor_unique(8)==vl({2}));
    require(a.phi[8]==4&&a.mu[8]==0&&b.phi(8)==4&&b.mobius(8)==0);
    vmi counts(11,1);counts[0]=0;
    auto divisor_sums=bundled::divisor_zeta(counts,b);
    require(divisor_sums[6].val()==4&&bundled::divisor_mobius(divisor_sums,b)==counts);
    auto multiple_sums=bundled::multiple_zeta(counts,b);
    require(multiple_sums[2].val()==5&&bundled::multiple_mobius(multiple_sums,b)==counts);
    cout<<"Passed "<<cases<<" independent factorization cases for both sieves; square boundaries, SPF crossover, partial cofactors, LLONG_MAX, and divisor regressions\n";
}
