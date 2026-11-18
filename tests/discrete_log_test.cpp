#include <bits/stdc++.h>
#include "silent-check.hpp"
using namespace std;
using ll=long long;
using LL=__int128;
using pll=pair<ll,ll>;
#include "MathNumberTheory/discrete_log.cpp"

size_t checked=0;
void require(bool ok){if(!ok)abort();}
ll normalize(ll x,ll m){x%=m;if(x<0)x+=m;return x;}
vector<ll> brute_all(ll a,ll m){
    vector<ll> first((size_t)m,-1);
    ll value=1%m,exponent=0;
    a=normalize(a,m);
    while(first[(size_t)value]<0){
        first[(size_t)value]=exponent++;
        value=(ll)((__uint128_t)(unsigned long long)value*(unsigned long long)a%(unsigned long long)m);
    }
    return first;
}
void check(ll a,ll b,ll m,ll expected){
    ll actual=discrete_log(a,b,m);
    if(actual!=expected){cerr<<"a="<<a<<" b="<<b<<" m="<<m<<" expected="<<expected<<" actual="<<actual<<'\n';abort();}
    ++checked;
}
ll slow_power(ll a,ll e,ll m){
    unsigned long long x=(unsigned long long)normalize(a,m),result=1%(unsigned long long)m;
    while(e--){result=(unsigned long long)((__uint128_t)result*x%(unsigned long long)m);}
    return (ll)result;
}
void exhaustive(){
    for(ll m=1;m<=180;++m)for(ll a=0;a<m;++a){
        auto expected=brute_all(a,m);
        for(ll b=0;b<m;++b){
            check(a,b,m,expected[(size_t)b]);
            if((a+3*b)%41==0)check(a-m,b-m,m,expected[(size_t)b]);
        }
    }
}
void random_cases(){
    mt19937_64 gen(4869);
    for(int t=0;t<30000;++t){
        ll m=1+(ll)(gen()%2500);
        ll a=(ll)(gen()%20001)-10000;
        auto expected=brute_all(a,m);
        for(int j=0;j<3;++j){ll b=(ll)(gen()%20001)-10000;check(a,b,m,expected[(size_t)normalize(b,m)]);}
    }
    for(ll m:{2,3,7,64,97,128,1000,1024})for(ll a:{LLONG_MIN,LLONG_MIN+1,-1LL,0LL,1LL,LLONG_MAX-1,LLONG_MAX}){
        auto expected=brute_all(a,m);
        for(ll b:{LLONG_MIN,LLONG_MIN+1,-1LL,0LL,1LL,LLONG_MAX-1,LLONG_MAX})check(a,b,m,expected[(size_t)normalize(b,m)]);
    }
}
void large(){
    for(ll m:{1LL,LLONG_MAX,(1LL<<62),4052555153018976267LL}){
        check(1,1,m,0);check(1,0,m,m==1?0:-1);
        check(0,0,m,m==1?0:1);check(0,1,m,0);
        check(LLONG_MIN,LLONG_MAX,1,0);
    }
    for(ll bits=1;bits<=62;++bits){
        ll m=1LL<<bits;
        check(2,0,m,bits);
        for(ll exponent=0;exponent<min(bits,10LL);++exponent)check(2,1LL<<exponent,m,exponent);
        if(bits>1)check(2,3,m,-1);
    }
    ll pow3=1;
    for(ll e=1;e<=39;++e){pow3*=3;check(3,0,pow3,e);check(-3,0,pow3,e);}
    for(ll m:{999999937LL,10000000019LL,1000000000039LL}){
        check(m-1,m-1,m,1);
        check(m-1,2,m,-1);
    }
    const ll m=10000000019LL;
    for(ll a:{1234567890LL,-12345678901234567LL,LLONG_MAX,LLONG_MIN}){
        map<ll,ll> first;
        for(ll e=0;e<=80;++e)first.emplace(slow_power(a,e,m),e);
        for(ll e:{1,2,19,80}){ll b=slow_power(a,e,m);check(a,b,m,first.at(b));}
    }
    require(stops_silently([]{(void)discrete_log(2,1,0);}));
    require(stops_silently([]{(void)discrete_log(2,1,-1);}));
    require(stops_silently([]{(void)discrete_log(LLONG_MIN,LLONG_MIN,LLONG_MIN);}));
}
int main(){
    exhaustive();random_cases();large();
    cout<<"Passed "<<checked<<" minimum-exponent comparisons; all a,b for moduli 1..180, full orbit enumeration, negative and LLONG_MIN/MAX inputs, noncoprime/zero targets, repeated residues, 10^12-scale BSGS and silent-stop checks\n";
}
