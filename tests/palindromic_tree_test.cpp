#include <bits/stdc++.h>
#include "silent-check.hpp"
using namespace std;
using ll=long long;
using vl=vector<ll>;
using pll=pair<ll,ll>;
#include "String/palindromic_tree.cpp"
size_t prefix_checks=0,string_checks=0;
void require(bool ok){if(!ok)abort();}
bool palindrome(const string&s){return equal(s.begin(),s.end(),s.rbegin());}
void verify(const string&s,const PalindromicTree&tree){
    map<string,ll> ids;vector<string> words;vector<pll> first;vl counts;
    for(size_t r=1;r<=s.size();++r)for(size_t l=0;l<r;++l){
        auto word=s.substr(l,r-l);if(!palindrome(word))continue;
        auto [it,added]=ids.emplace(word,ll(words.size()));
        if(added){words.push_back(word);first.emplace_back(ll(l),ll(r));counts.push_back(0);}
        ++counts[size_t(it->second)];
    }
    require(tree.size()==ll(s.size())&&tree.distinct_count()==ll(words.size()));
    require(tree.occurrence_counts()==counts&&tree.occurrence_counts()==counts);
    ll longest=-1;
    for(size_t i=0;i<words.size();++i){
        const ll id=ll(i);require(tree.length(id)==ll(words[i].size()));require(tree.first_occurrence(id)==first[i]);
        ll suffix=-1;
        for(size_t begin=1;begin<words[i].size();++begin){
            string tail=words[i].substr(begin);
            if(palindrome(tail)){suffix=ids.at(tail);break;}
        }
        require(tree.suffix_link(id)==suffix);require(suffix<id);
        if(s.size()>=words[i].size()&&s.compare(s.size()-words[i].size(),words[i].size(),words[i])==0)
            if(longest<0||tree.length(longest)<tree.length(id))longest=id;
    }
    require(tree.longest_suffix()==longest);++prefix_checks;
}
void check(const string&s){
    PalindromicTree tree;string prefix;verify(prefix,tree);
    for(char c:s){
        const ll old=tree.distinct_count();prefix+=c;
        const ll id=tree.push(c);require(id==tree.longest_suffix());
        require(tree.distinct_count()==old||tree.distinct_count()==old+1);verify(prefix,tree);
    }
    string copy=s;PalindromicTree built(copy);copy.clear();verify(s,built);
    auto snapshot=built.occurrence_counts();const auto original=snapshot;
    built.clear();require(built.size()==0&&built.longest_suffix()==-1&&built.distinct_count()==0);require(snapshot==original);
    require(built.push('z')==0&&built.length(0)==1&&built.suffix_link(0)==-1);
    ++string_checks;
}
int main(){
    size_t count=1;
    for(int n=0;n<=8;++n){
        for(size_t code=0;code<count;++code){
            size_t x=code;string s(size_t(n),'a');for(char&c:s){c=char('a'+x%3);x/=3;}check(s);
        }
        count*=3;
    }
    mt19937_64 rng(4869);
    for(int t=0;t<300;++t){string s(rng()%61,'a');for(char&c:s)c=char('a'+rng()%(t%2?26:2));check(s);}
    for(const string&s:{string("abcdefghijklmnopqrstuvwxyz"),string("abba"),string("abacabadabacaba"),string("abca")})check(s);
    const array<unsigned char,8> invalid={'A','0',' ',0,255,128,'`','{'};
    for(auto c:invalid){
        require(stops_silently([&]{PalindromicTree t;t.push(char(c));}));
        require(stops_silently([&]{PalindromicTree t(string(1,char(c)));}));
    }
    for(int api=0;api<3;++api)for(ll id:{-1LL,1LL})require(stops_silently([&]{
        PalindromicTree t("a");if(api==0)(void)t.length(id);if(api==1)(void)t.first_occurrence(id);if(api==2)(void)t.suffix_link(id);
    }));
    require(stops_silently([]{PalindromicTree t;(void)t.length(0);}));
    const ll n=200000;
    for(int pattern=0;pattern<2;++pattern){
        string s(size_t(n),'a');if(pattern==1)for(ll i=1;i<n;i+=2)s[size_t(i)]='b';
        PalindromicTree tree(s);require(tree.size()==n&&tree.distinct_count()==n);
        const auto counts=tree.occurrence_counts();
        for(ll id=0;id<n;++id){
            auto [l,r]=tree.first_occurrence(id);const ll length=tree.length(id);
            require(r-l==length&&l>=0&&r<=n);
            if(pattern==0){require(l==0&&length==id+1&&counts[size_t(id)]==n-length+1);require(tree.suffix_link(id)==id-1);}
            else{require((length&1)&&l<=1);require(counts[size_t(id)]==(n-r)/2+1);}
        }
        require(tree.length(tree.longest_suffix())==(pattern==0?n:n-1));
        auto again=tree.occurrence_counts();require(again==counts);
        const ll id=tree.push('z');require(tree.length(id)==1&&tree.distinct_count()==n+1);
        again=tree.occurrence_counts();require(again.back()==1);again.pop_back();require(again==counts);
    }
    cout<<"Passed "<<string_checks<<" strings and "<<prefix_checks
        <<" independent substring-enumeration snapshots; 200000-character unary/alternating strings, append after counting, 23 silent-stop cases\n";
}
