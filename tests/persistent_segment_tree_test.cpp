#include <bits/stdc++.h>
#include <atcoder/segtree>
#include "silent-check.hpp"
using ll=long long;
#include "DataStructureSegment/persistent_segment_tree.cpp"

std::uint64_t checks=0;
void require(bool ok,std::source_location at=std::source_location::current()){
    ++checks;
    if(!ok){std::cerr<<"check failed at line "<<at.line()<<'\n';std::abort();}
}
ll sum_op(ll a,ll b){return a+b;}
ll sum_e(){return 0;}
bool below_six(ll x){return x<6;}
using Tree=PersistentSegmentTree<ll,sum_op,sum_e,300000>;
using ACL=atcoder::segtree<ll,sum_op,sum_e>;
static_assert(std::is_copy_constructible_v<Tree>);
static_assert(std::is_copy_assignable_v<Tree>);
static_assert(std::is_nothrow_move_constructible_v<Tree>);
static_assert(std::is_nothrow_move_assignable_v<Tree>);

ll scan(const std::vector<ll>&a,int l,int r){return std::accumulate(a.begin()+l,a.begin()+r,0LL);}
ll right_scan(const std::vector<ll>&a,int l,ll limit){
    ll sum=0;
    int r=l;
    while(r<static_cast<int>(a.size())&&sum+a[r]<=limit)sum+=a[r++];
    return r;
}
ll left_scan(const std::vector<ll>&a,int r,ll limit){
    ll sum=0;
    int l=r;
    while(l>0&&sum+a[l-1]<=limit)sum+=a[--l];
    return l;
}

void exhaustive(){
    for(int n=0;n<=7;++n){
        int count=1;
        for(int i=0;i<n;++i)count*=3;
        for(int mask=0;mask<count;++mask){
            std::vector<ll>a(n);
            int bits=mask;
            for(auto&x:a){x=bits%3-1;bits/=3;}
            Tree tree(a);
            ACL acl(a);
            require(tree.nodes_used()==(n?2*n-1:0));
            for(int l=0;l<=n;++l)for(int r=l;r<=n;++r){
                require(tree.prod(l,r)==scan(a,l,r));
                require(tree.prod(l,r)==acl.prod(l,r));
            }
            for(int p=0;p<n;++p){
                auto next=tree;
                next.set(p,5);
                require(next.get(p)==5&&tree.get(p)==a[p]);
                require(next.all_prod()==tree.all_prod()-a[p]+5);
            }
        }
        for(int mask=0;mask<(1<<n);++mask){
            std::vector<ll>a(n);
            for(int i=0;i<n;++i)a[i]=(mask>>i)&1;
            const Tree tree(a);
            ACL acl(a);
            for(int limit=0;limit<=n+1;++limit)for(int p=0;p<=n;++p){
                auto pred=[&](ll x){return x<=limit;};
                require(tree.max_right(p,pred)==right_scan(a,p,limit));
                require(tree.min_left(p,pred)==left_scan(a,p,limit));
                require(tree.max_right(p,pred)==acl.max_right(p,pred));
                require(tree.min_left(p,pred)==acl.min_left(p,pred));
            }
        }
    }
}

void branches(){
    std::mt19937_64 rng(20261022);
    for(int trial=0;trial<240;++trial){
        const int n=1+static_cast<int>(rng()%48);
        std::vector<ll>a(n);
        for(auto&x:a)x=static_cast<ll>(rng()%16);
        std::vector<Tree>versions{Tree(n),Tree(a)};
        std::vector<ACL>acls{ACL(n),ACL(a)};
        std::vector<std::vector<ll>>arrays{std::vector<ll>(n),a};
        for(int step=0;step<200;++step){
            const std::size_t base=static_cast<std::size_t>(rng()%versions.size());
            const int p=static_cast<int>(rng()%static_cast<unsigned>(n));
            const ll value=static_cast<ll>(rng()%31);
            Tree next=versions[base];
            ACL next_acl=acls[base];
            auto next_array=arrays[base];
            const int before=next.nodes_used();
            next.set(p,value);next_acl.set(p,value);next_array[p]=value;
            const int added=next.nodes_used()-before;
            require(added>=1&&added<=static_cast<int>(std::bit_width(static_cast<unsigned>(n-1)))+1);
            versions.push_back(next);acls.push_back(next_acl);arrays.push_back(next_array);
            for(int sample=0;sample<8;++sample){
                const std::size_t v=static_cast<std::size_t>(rng()%versions.size());
                const Tree&reader=versions[v];
                const int used=reader.nodes_used();
                int l=static_cast<int>(rng()%static_cast<unsigned>(n+1));
                int r=static_cast<int>(rng()%static_cast<unsigned>(n+1));
                if(l>r)std::swap(l,r);
                const int i=static_cast<int>(rng()%static_cast<unsigned>(n));
                const ll limit=static_cast<ll>(rng()%201);
                auto pred=[&](ll x){return x<=limit;};
                require(reader.get(i)==arrays[v][i]&&reader.get(i)==acls[v].get(i));
                require(reader.prod(l,r)==scan(arrays[v],l,r)&&reader.prod(l,r)==acls[v].prod(l,r));
                require(reader.all_prod()==scan(arrays[v],0,n));
                require(reader.max_right(l,pred)==right_scan(arrays[v],l,limit));
                require(reader.min_left(r,pred)==left_scan(arrays[v],r,limit));
                require(reader.max_right(l,pred)==acls[v].max_right(l,pred));
                require(reader.min_left(r,pred)==acls[v].min_left(r,pred));
                require(reader.nodes_used()==used);
            }
        }
        for(std::size_t v=0;v<versions.size();++v)for(int p=0;p<n;++p)require(versions[v].get(p)==arrays[v][p]);
    }
}

std::string concat(std::string a,std::string b){return a+b;}
std::string empty_string(){return "";}
void noncommutative(){
    using Strings=PersistentSegmentTree<std::string,concat,empty_string,10000>;
    std::vector<Strings>versions{Strings(std::vector<std::string>{"a","","bc","d","","ef","g"})};
    std::vector<std::vector<std::string>>arrays{{"a","","bc","d","","ef","g"}};
    std::mt19937_64 rng(4869);
    for(int step=0;step<120;++step){
        const std::size_t base=static_cast<std::size_t>(rng()%versions.size());
        auto next=versions[base];auto a=arrays[base];
        const int p=static_cast<int>(rng()%7);
        a[p]=std::string(static_cast<std::size_t>(rng()%3),static_cast<char>('a'+rng()%7));
        next.set(p,a[p]);versions.push_back(next);arrays.push_back(a);
    }
    for(std::size_t v=0;v<versions.size();++v)for(int l=0;l<=7;++l)for(int r=l;r<=7;++r){
        std::string target;
        for(int i=l;i<r;++i)target+=arrays[v][i];
        require(versions[v].prod(l,r)==target);
        auto prefix=[&](std::string s){return target.starts_with(s);};
        auto suffix=[&](std::string s){return target.ends_with(s);};
        int right=l,left=r;
        std::string current;
        while(right<7&&prefix(current+arrays[v][right]))current+=arrays[v][right++];
        current.clear();
        while(left>0&&suffix(arrays[v][left-1]+current))current=arrays[v][--left]+current;
        require(versions[v].max_right(l,prefix)==right);
        require(versions[v].min_left(r,suffix)==left);
    }
}

struct Value{
    static inline int live=0;
    ll x;
    Value()=delete;
    explicit Value(ll v):x(v){++live;}
    Value(const Value&v):x(v.x){++live;}
    Value&operator=(const Value&)=default;
    ~Value(){--live;}
};
Value value_op(Value a,Value b){return Value(a.x+b.x);}
Value value_e(){return Value(0);}

void boundaries(){
    Tree empty;
    require(empty.size()==0&&empty.nodes_used()==0&&empty.all_prod()==0&&empty.prod(0,0)==0);
    require(empty.max_right<below_six>(0)==0&&empty.min_left<below_six>(0)==0);
    PersistentSegmentTree<ll,sum_op,sum_e,1> one(1);
    auto blank=one;
    one.set(0,5);
    require(one.nodes_used()==1&&blank.nodes_used()==1&&blank.get(0)==0);
    require(one.max_right<below_six>(0)==1&&one.min_left<below_six>(1)==0);
    PersistentSegmentTree<ll,sum_op,sum_e,5> exact(std::vector<ll>{1,2,3});
    require(exact.nodes_used()==5&&exact.all_prod()==6);
    require(exact.max_right<below_six>(0)==2&&exact.min_left<below_six>(3)==1);
    Tree huge(LLONG_MAX);
    auto old=huge;
    huge.set(LLONG_MAX-1,7);huge.set(LLONG_MAX/2,11);huge.set(0,13);
    require(huge.all_prod()==31&&old.all_prod()==0&&old.nodes_used()==huge.nodes_used());
    require(huge.get(LLONG_MAX-1)==7&&huge.get(LLONG_MAX/2-1)==0);
    require(huge.max_right(0,[](ll x){return x<13;})==0);
    require(huge.max_right(1,[](ll x){return x<11;})==LLONG_MAX/2);
    require(huge.min_left(LLONG_MAX,[](ll x){return x<7;})==LLONG_MAX);
    require(huge.min_left(LLONG_MAX-1,[](ll x){return x<11;})==LLONG_MAX/2+1);
    require(huge.nodes_used()<=192);
    require(Value::live==0);
    {
        using Wrapped=PersistentSegmentTree<Value,value_op,value_e,100>;
        Wrapped survivor;
        {
            Wrapped original(std::vector<Value>{Value(1),Value(2),Value(3)});
            survivor=original;
            original.set(1,Value(8));
            require(survivor.all_prod().x==6&&original.all_prod().x==12);
        }
        require(survivor.get(1).x==2);
        require(Value::live==survivor.nodes_used());
        Wrapped moved(std::move(survivor));
        require(survivor.size()==0&&survivor.all_prod().x==0&&moved.all_prod().x==6);
        survivor=std::move(moved);
        require(moved.size()==0&&moved.nodes_used()==0&&survivor.all_prod().x==6);
        Wrapped*alias=&survivor;
        survivor=std::move(*alias);
        require(survivor.all_prod().x==6);
    }
    require(Value::live==0);
    Tree bounded(std::vector<ll>{1,2,3});
    require(stops_silently([]{Tree invalid(-1);}));
    require(stops_silently([&]{empty.set(0,1);}));
    require(stops_silently([&]{one.set(0,6);}));
    require(stops_silently([&]{exact.set(0,1);}));
    require(stops_silently([&]{bounded.set(-1,1);}));
    require(stops_silently([&]{bounded.set(3,1);}));
    require(stops_silently([&]{bounded.get(LLONG_MAX);}));
    require(stops_silently([&]{bounded.prod(-1,1);}));
    require(stops_silently([&]{bounded.prod(2,1);}));
    require(stops_silently([&]{bounded.prod(0,4);}));
    require(stops_silently([&]{bounded.max_right(-1,[](ll){return true;});}));
    require(stops_silently([&]{bounded.min_left(4,[](ll){return true;});}));
    require(stops_silently([&]{bounded.max_right(3,[](ll){return false;});}));
    require(stops_silently([&]{bounded.min_left(0,[](ll){return false;});}));
}

int main(){exhaustive();branches();noncommutative();boundaries();std::cout<<"Persistent Segment Tree: "<<checks<<" checks passed; 48000 branching updates, ACL comparison.\n";}
