#include <bits/stdc++.h>
using namespace std;
using ll=long long;
using vl=vector<ll>;
#include "Search/longest_increasing_subsequence.cpp"
size_t enumeration_cases=0,dp_cases=0;
void require(bool ok){if(!ok)abort();}
bool compatible(ll a,ll b,bool strict){return strict?a<b:a<=b;}
vl enumerate(const vl&a,bool strict){
    vl lengths(a.size());
    const size_t count=size_t(1)<<a.size();
    for(size_t mask=1;mask<count;++mask){
        ll previous=-1,length=0;bool ok=true;
        for(size_t i=0;i<a.size();++i)if((mask>>i)&1){
            if(previous>=0&&!compatible(a[size_t(previous)],a[i],strict)){ok=false;break;}
            previous=ll(i);++length;
        }
        if(ok)lengths[size_t(previous)]=max(lengths[size_t(previous)],length);
    }
    return lengths;
}
vl quadratic(const vl&a,bool strict){
    vl lengths(a.size(),1);
    for(size_t i=0;i<a.size();++i)for(size_t j=0;j<i;++j)
        if(compatible(a[j],a[i],strict))lengths[i]=max(lengths[i],lengths[j]+1);
    return lengths;
}
void validate(const vl&a,bool strict,const LISResult&r,const vl&expected){
    require(r.ending_length==expected);
    const ll best=expected.empty()?0:*max_element(expected.begin(),expected.end());
    require(r.length==best&&r.indices.size()==size_t(best));
    ll previous=-1;
    for(ll i:r.indices){
        require(previous<i&&i>=0&&size_t(i)<a.size());
        if(previous>=0)require(compatible(a[size_t(previous)],a[size_t(i)],strict));
        previous=i;
    }
}
void check(const vl&a,bool exhaustive){
    const vl original=a;
    for(bool strict:{false,true}){
        auto expected=exhaustive?enumerate(a,strict):quadratic(a,strict);
        auto result=longest_increasing_subsequence(a,strict);
        validate(a,strict,result,expected);require(a==original);
        if(strict){
            auto implicit=longest_increasing_subsequence(a);
            require(implicit.length==result.length&&implicit.indices==result.indices&&implicit.ending_length==result.ending_length);
        }
        if(exhaustive)++enumeration_cases;else ++dp_cases;
    }
}
int main(){
    LISResult blank;require(blank.length==0&&blank.indices.empty()&&blank.ending_length.empty());
    const array<ll,4> alphabet={-2,0,1,3};
    size_t count=1;
    for(int n=0;n<=8;++n){
        for(size_t code=0;code<count;++code){
            size_t x=code;vl a(size_t(n),0);
            for(auto&value:a){value=alphabet[x%alphabet.size()];x/=alphabet.size();}
            check(a,true);
        }
        count*=alphabet.size();
    }
    mt19937_64 rng(4869);
    const array<ll,7> extremes={LLONG_MIN,LLONG_MIN+1,-1,0,1,LLONG_MAX-1,LLONG_MAX};
    for(int t=0;t<2500;++t){
        vl a(rng()%101);
        for(auto&x:a){
            if(t%3==0)x=extremes[rng()%extremes.size()];
            else if(t%3==1)x=ll(rng()%11)-5;
            else x=bit_cast<ll>(rng());
        }
        check(a,false);
    }
    check({LLONG_MIN,LLONG_MAX,LLONG_MIN,LLONG_MAX},true);
    check({2,3,1},true);
    check({1,2,3,0},true);
    vl a={3,1,2,2,4};auto result=longest_increasing_subsequence(a);a.clear();
    require(result.length==3&&result.ending_length==vl({1,1,2,2,3}));
    const size_t n=1000000;
    for(int pattern=0;pattern<4;++pattern){
        vl large(n);
        for(size_t i=0;i<n;++i){
            if(pattern==0)large[i]=ll(i);
            if(pattern==1)large[i]=ll(n-i);
            if(pattern==2)large[i]=LLONG_MAX;
            if(pattern==3)large[i]=i%2?LLONG_MAX:LLONG_MIN;
        }
        for(bool strict:{false,true}){
            vl expected(n);
            for(size_t i=0;i<n;++i){
                if(pattern==0)expected[i]=ll(i+1);
                if(pattern==1)expected[i]=1;
                if(pattern==2)expected[i]=strict?1:ll(i+1);
                if(pattern==3)expected[i]=strict?ll(1+i%2):ll(i/2+1+i%2);
            }
            validate(large,strict,longest_increasing_subsequence(large,strict),expected);
        }
    }
    cout<<"Passed "<<enumeration_cases<<" complete-subsequence comparisons and "<<dp_cases
        <<" quadratic-DP comparisons; restoration, ending lengths, full ll extremes, 8 million-element cases\n";
}
