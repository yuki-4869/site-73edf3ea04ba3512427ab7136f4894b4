#include <bits/stdc++.h>
#include "silent-check.hpp"
using ll=long long;
#include "DataStructureSegment/persistent_segment_tree.cpp"

std::uint64_t checks=0;
void require(bool ok){++checks;if(!ok)std::abort();}
struct Sum{ll operator()(ll a,ll b)const{return a+b;}};
using Tree=PersistentSegmentTree<ll,Sum,200000>;
static_assert(!std::is_copy_constructible_v<Tree>);
static_assert(!std::is_copy_assignable_v<Tree>);
static_assert(!std::is_move_constructible_v<Tree>);
static_assert(!std::is_move_assignable_v<Tree>);

ll scan(const std::vector<ll>&a,int l,int r){return std::accumulate(a.begin()+l,a.begin()+r,0LL);}
ll right_scan(const std::vector<ll>&a,int l,ll limit){
    ll sum=0;
    int r=l;
    while(r<static_cast<int>(a.size())&&sum+a[r]<=limit)sum+=a[r++];
    return r;
}

void exhaustive(){
    for(int n=0;n<=6;++n){
        Tree pst(n,Sum{},0);
        int count=1;
        for(int i=0;i<n;++i)count*=3;
        std::vector<Tree::Node*> roots;
        std::vector<std::vector<ll>> arrays;
        for(int mask=0;mask<count;++mask){
            std::vector<ll>a(n);
            int x=mask;
            for(auto&v:a){v=x%3-1;x/=3;}
            const int before=pst.nodes_used();
            auto root=pst.build(a);
            require(pst.nodes_used()-before==(n?2*n-1:0));
            roots.push_back(root);arrays.push_back(a);
        }
        for(std::size_t t=0;t<roots.size();++t){
            auto root=roots[t];
            for(int l=0;l<=n;++l)for(int r=l;r<=n;++r)require(pst.query(root,l,r)==scan(arrays[t],l,r));
            if(n){
                auto changed=pst.update(root,n-1,12);
                require(pst.query(changed,0,n)==scan(arrays[t],0,n)-arrays[t].back()+12);
                require(pst.query(root,0,n)==scan(arrays[t],0,n));
            }
        }
    }
}

void branches(){
    std::mt19937_64 rng(20261022);
    for(int trial=0;trial<400;++trial){
        const int n=1+static_cast<int>(rng()%40);
        Tree pst(n,Sum{},0);
        std::vector<ll> initial(n);
        for(auto&x:initial)x=static_cast<ll>(rng()%21);
        std::vector<Tree::Node*> roots{pst.new_tree(),pst.build(initial)};
        std::vector<std::vector<ll>> arrays{std::vector<ll>(n),initial};
        for(int step=0;step<160;++step){
            const std::size_t base=static_cast<std::size_t>(rng()%roots.size());
            const int i=static_cast<int>(rng()%static_cast<unsigned>(n));
            std::vector<ll> next=arrays[base];
            const ll value=static_cast<ll>(rng()%21);
            const int before=pst.nodes_used();
            Tree::Node* root;
            if(rng()%2){root=pst.update(roots[base],i,value);next[i]=value;}
            else{
                int calls=0;
                root=pst.update_apply(roots[base],i,[&](ll x){++calls;return x+value;});
                require(calls==1);next[i]+=value;
            }
            const int added=pst.nodes_used()-before;
            require(added>=1&&added<=std::bit_width(static_cast<unsigned>(n-1))+1);
            roots.push_back(root);arrays.push_back(next);
            const Tree&reader=pst;
            for(int t=0;t<12;++t){
                const std::size_t version=static_cast<std::size_t>(rng()%roots.size());
                int l=static_cast<int>(rng()%static_cast<unsigned>(n+1));
                int r=static_cast<int>(rng()%static_cast<unsigned>(n+1));
                if(l>r)std::swap(l,r);
                require(reader.query(roots[version],l,r)==scan(arrays[version],l,r));
                const ll limit=static_cast<ll>(rng()%151);
                require(reader.max_right(roots[version],l,[&](ll x){return x<=limit;})==right_scan(arrays[version],l,limit));
            }
            require(reader.query(roots[base],0,n)==scan(arrays[base],0,n));
        }
        const int before=pst.nodes_used();
        for(std::size_t t=0;t<roots.size();++t)require(pst.query(roots[t],0,n)==scan(arrays[t],0,n));
        for(int i=0;i<100;++i)require(pst.new_tree()==roots[0]);
        require(pst.nodes_used()==before);
    }
}

void noncommutative(){
    auto concat=[](const std::string&a,const std::string&b){return a+b;};
    using Strings=PersistentSegmentTree<std::string,decltype(concat),20000>;
    Strings pst(9,concat,"");
    std::vector<Strings::Node*>roots{pst.new_tree()};
    std::vector<std::vector<std::string>>arrays{std::vector<std::string>(9)};
    std::mt19937_64 rng(42);
    for(int step=0;step<160;++step){
        const std::size_t base=static_cast<std::size_t>(rng()%roots.size());
        const int i=static_cast<int>(rng()%9);
        auto next=arrays[base];
        next[i]=std::string(1+static_cast<std::size_t>(rng()%2),static_cast<char>('a'+rng()%8));
        roots.push_back(pst.update(roots[base],i,next[i]));arrays.push_back(next);
    }
    for(std::size_t t=0;t<roots.size();++t)for(int l=0;l<=9;++l){
        std::string expected;
        for(int r=l;r<=9;++r){
            require(pst.query(roots[t],l,r)==expected);
            const std::string target=expected;
            auto pred=[&](const std::string&s){return target.starts_with(s);};
            int end=l;
            std::string current;
            while(end<9&&pred(current+arrays[t][end]))current+=arrays[t][end++];
            require(pst.max_right(roots[t],l,pred)==end);
            if(r<9)expected+=arrays[t][r];
        }
    }
}

struct NoDefault{
    ll value;
    NoDefault()=delete;
    explicit NoDefault(ll x):value(x){}
};
struct WrappedSum{NoDefault operator()(const NoDefault&a,const NoDefault&b)const{return NoDefault(a.value+b.value);}};
struct Counted{
    static inline int live=0;
    ll value;
    explicit Counted(ll x):value(x){++live;}
    Counted(const Counted&x):value(x.value){++live;}
    ~Counted(){--live;}
};
struct CountedSum{Counted operator()(const Counted&a,const Counted&b)const{return Counted(a.value+b.value);}};

void boundaries(){
    PersistentSegmentTree<ll,Sum,1> empty(0,Sum{},0);
    auto blank=empty.new_tree();
    require(empty.build({})==blank&&empty.size()==0&&empty.nodes_used()==1);
    require(empty.query(blank,0,0)==0);
    require(empty.max_right(blank,0,[](ll x){return x==0;})==0);
    PersistentSegmentTree<ll,Sum,2> one(1,Sum{},0);
    auto filled=one.update(one.new_tree(),0,5);
    require(one.query(filled,0,1)==5&&one.query(one.new_tree(),0,1)==0&&one.nodes_used()==2);
    require(one.max_right(filled,0,[](ll x){return x<5;})==0);
    require(one.max_right(filled,0,[](ll x){return x<=5;})==1);
    PersistentSegmentTree<ll,Sum,6> exact(3,Sum{},0);
    auto exact_root=exact.build({1,2,3});
    require(exact.nodes_used()==6&&exact.query(exact_root,0,3)==6);
    PersistentSegmentTree<ll,Sum> huge(LLONG_MAX,Sum{},0);
    require(huge.nodes_used()==1&&huge.size()==LLONG_MAX);
    auto huge_blank=huge.new_tree();
    auto last=huge.update(huge_blank,LLONG_MAX-1,7);
    auto middle=huge.update(last,LLONG_MAX/2,11);
    auto first=huge.update(middle,0,13);
    require(huge.query(last,LLONG_MAX-1,LLONG_MAX)==7);
    require(huge.query(middle,0,LLONG_MAX)==18&&huge.query(first,0,LLONG_MAX)==31);
    require(huge.query(huge_blank,0,LLONG_MAX)==0);
    require(huge.max_right(last,0,[](ll x){return x<7;})==LLONG_MAX-1);
    require(huge.max_right(middle,0,[](ll x){return x<=10;})==LLONG_MAX/2);
    require(huge.max_right(first,LLONG_MAX/2+1,[](ll x){return x<7;})==LLONG_MAX-1);
    require(huge.max_right(first,LLONG_MAX,[](ll){return true;})==LLONG_MAX);
    require(huge.nodes_used()<=193);
    PersistentSegmentTree<NoDefault,WrappedSum,100> wrapped(3,WrappedSum{},NoDefault(0));
    auto wr=wrapped.build({NoDefault(2),NoDefault(4),NoDefault(8)});
    auto wr2=wrapped.update_apply(wr,1,[](const NoDefault&v){return NoDefault(v.value+3);});
    require(wrapped.query(wr,0,3).value==14&&wrapped.query(wr2,0,3).value==17);
    require(Counted::live==0);
    {
        PersistentSegmentTree<Counted,CountedSum,10000000> counted(10,CountedSum{},Counted(0));
        auto cr=counted.update(counted.new_tree(),5,Counted(10));
        require(counted.query(cr,0,10).value==10);
        require(Counted::live==counted.nodes_used()+1);
    }
    require(Counted::live==0);
    auto min_op=[](ll a,ll b){return std::min(a,b);};
    PersistentSegmentTree<ll,decltype(min_op),100> minimum(3,min_op,LLONG_MAX);
    auto mr=minimum.update(minimum.new_tree(),1,2);
    require(minimum.query(mr,0,3)==2&&minimum.query(mr,2,3)==LLONG_MAX);

    Tree bounded(3,Sum{},0);
    auto root=bounded.build({1,2,3});
    require(stops_silently([]{Tree invalid(-1,Sum{},0);}));
    require(stops_silently([&]{empty.update(blank,0,1);}));
    require(stops_silently([&]{one.update(filled,0,6);}));
    require(stops_silently([&]{exact.build({1,2,3});}));
    require(stops_silently([&]{bounded.build({});}));
    require(stops_silently([&]{bounded.build({1,2,3,4});}));
    require(stops_silently([&]{bounded.update(root,-1,1);}));
    require(stops_silently([&]{bounded.update(root,3,1);}));
    require(stops_silently([&]{bounded.update_apply(root,LLONG_MAX,[](ll x){return x;});}));
    require(stops_silently([&]{bounded.query(root,-1,1);}));
    require(stops_silently([&]{bounded.query(root,0,-1);}));
    require(stops_silently([&]{bounded.query(root,2,1);}));
    require(stops_silently([&]{bounded.query(root,0,4);}));
    require(stops_silently([&]{bounded.max_right(root,-1,[](ll){return true;});}));
    require(stops_silently([&]{bounded.max_right(root,4,[](ll){return true;});}));
    require(stops_silently([&]{bounded.max_right(root,0,[](ll){return false;});}));
    require(stops_silently([&]{bounded.max_right(root,3,[](ll){return false;});}));
    require(stops_silently([&]{bounded.query(nullptr,0,0);}));
    require(stops_silently([&]{bounded.update(nullptr,0,1);}));
    require(stops_silently([&]{bounded.max_right(nullptr,3,[](ll){return true;});}));
}

int main(){exhaustive();branches();noncommutative();boundaries();std::cout<<"Persistent Segment Tree: "<<checks<<" checks passed; 64000 branching updates.\n";}
