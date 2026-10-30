#include <bits/stdc++.h>
using namespace std;
using ll=long long;
using pll=pair<ll,ll>;
#include "String/lyndon_factorization.cpp"
size_t strings_checked=0,partitions_checked=0;
void require(bool ok){if(!ok)abort();}
bool less_bytes(string_view a,string_view b){
    return lexicographical_compare(a.begin(),a.end(),b.begin(),b.end(),[](char x,char y){return static_cast<unsigned char>(x)<static_cast<unsigned char>(y);});
}
bool is_lyndon(string_view word){
    if(word.empty())return false;
    for(size_t k=1;k<word.size();++k)if(!less_bytes(word,word.substr(k)))return false;
    return true;
}
vector<pll> brute_factors(const string&s){
    if(s.empty())return {};
    const size_t n=s.size();
    vector<vector<bool>> good(n,vector<bool>(n+1));
    for(size_t l=0;l<n;++l)for(size_t r=l+1;r<=n;++r)good[l][r]=is_lyndon(string_view(s).substr(l,r-l));
    vector<pll> result;
    size_t valid=0;
    for(size_t mask=0;mask<(size_t(1)<<(n-1));++mask){
        vector<pll> pieces;
        size_t l=0;bool ok=true;
        for(size_t r=1;r<=n;++r)if(r==n||(mask>>(r-1)&1)){
            if(!good[l][r]){ok=false;break;}
            if(!pieces.empty()){
                auto [pl,pr]=pieces.back();
                if(less_bytes(string_view(s).substr(size_t(pl),size_t(pr-pl)),string_view(s).substr(l,r-l))){ok=false;break;}
            }
            pieces.emplace_back(ll(l),ll(r));l=r;
        }
        if(ok){++valid;result=std::move(pieces);}
    }
    require(valid==1);return result;
}
ll brute_rotation(const string&s){
    if(s.empty())return 0;
    ll answer=0;string best=s;
    for(size_t i=1;i<s.size();++i){
        string rotated=s.substr(i)+s.substr(0,i);
        if(less_bytes(rotated,best)){best=std::move(rotated);answer=ll(i);}
    }
    return answer;
}
void check(const string&s,bool full_partition){
    auto original=s;
    const auto factors=lyndon_factorization(s);
    ll end=0;string_view prev;
    for(auto [l,r]:factors){
        require(l==end&&l<r&&r<=ll(s.size()));
        auto word=string_view(s).substr(size_t(l),size_t(r-l));
        require(is_lyndon(word));
        if(l>0)require(!less_bytes(prev,word));
        prev=word;end=r;
    }
    require(end==ll(s.size()));
    if(full_partition){require(factors==brute_factors(s));++partitions_checked;}
    require(minimum_rotation(s)==brute_rotation(s));require(s==original);++strings_checked;
}
int main(){
    const array<unsigned char,3> alphabet={0,128,255};
    size_t total=1;
    for(size_t n=0;n<=10;++n){
        if(n)total*=3;
        for(size_t code=0;code<total;++code){
            size_t v=code;string s(n,'\0');
            for(auto&c:s){c=static_cast<char>(alphabet[v%3]);v/=3;}
            check(s,n<=8);
        }
    }
    for(string s:{"banana","baba","abab","aaaa","alfa","aaba","aaaba","0010","abacaba","z"})check(s,s.size()<=8);
    mt19937_64 rng(4869);
    for(int t=0;t<1000;++t){
        string s(size_t(rng()%81),'\0');for(auto&c:s)c=static_cast<char>(rng()%256);
        check(s,s.size()<=8);
    }
    const ll n=1000000;
    string same(size_t(n),'a');auto f=lyndon_factorization(same);
    require(f.size()==size_t(n));for(ll i=0;i<n;++i)require((f[size_t(i)]==pll{i,i+1}));require(minimum_rotation(same)==0);
    string periodic;periodic.reserve(size_t(n));for(ll i=0;i<n/2;++i)periodic+="ba";
    require(minimum_rotation(periodic)==1);
    string last(size_t(n),'a');last.back()='b';require((lyndon_factorization(last)==vector<pll>{{0,n}}));require(minimum_rotation(last)==0);
    string first(size_t(n),'a');first.front()='b';require(minimum_rotation(first)==1);
    string tail(size_t(n),'b');tail.back()='a';require(minimum_rotation(tail)==n-1);
    cout<<"Passed "<<strings_checked<<" brute-rotation cases, "<<partitions_checked<<" exhaustive-partition cases; 1000000-byte periodic and boundary cases\n";
}
