#include <bits/stdc++.h>
#include <atcoder/lazysegtree>
#include "silent-check.hpp"
using ll=long long;
#include "DataStructureSegment/persistent_lazy_segment_tree.cpp"

std::uint64_t checks=0;
void require(bool ok,std::source_location at=std::source_location::current()){
    ++checks;
    if(!ok){std::cerr<<"check failed at line "<<at.line()<<'\n';std::abort();}
}
constexpr ll MOD=998244353;
struct S{ll sum,len;bool operator==(const S&)const=default;};
struct F{ll a,b;};
S op(S x,S y){return {(x.sum+y.sum)%MOD,x.len+y.len};}
S e(){return {0,0};}
S mapping(F f,S x){return {(f.a*x.sum+f.b*x.len)%MOD,x.len};}
F composition(F f,F g){return {f.a*g.a%MOD,(f.a*g.b+f.b)%MOD};}
F id(){return {1,0};}
bool short_segment(S x){return x.len<=2;}
using Tree=PersistentLazySegmentTree<S,op,e,F,mapping,composition,id,400000>;
using ACL=atcoder::lazy_segtree<S,op,e,F,mapping,composition,id>;
static_assert(std::is_copy_constructible_v<Tree>);
static_assert(std::is_copy_assignable_v<Tree>);
static_assert(std::is_nothrow_move_constructible_v<Tree>);

S scan(const std::vector<S>&a,int l,int r){
    S sum=e();
    for(int i=l;i<r;++i)sum=op(sum,a[i]);
    return sum;
}

void exhaustive(){
    for(int n=0;n<=5;++n){
        int count=1;
        for(int i=0;i<n;++i)count*=3;
        for(int mask=0;mask<count;++mask){
            std::vector<S>a(n);
            int bits=mask;
            for(auto&x:a){x={bits%3,1};bits/=3;}
            Tree original(a);
            require(original.nodes_used()==(n?2*n-1:0));
            for(int l=0;l<=n;++l)for(int r=l;r<=n;++r)for(F tag:{F{0,2},F{1,3},F{2,1}}){
                Tree changed=original;
                ACL acl(a);
                auto expected=a;
                changed.apply(l,r,tag);acl.apply(l,r,tag);
                for(int i=l;i<r;++i)expected[i]=mapping(tag,expected[i]);
                for(int ql=0;ql<=n;++ql)for(int qr=ql;qr<=n;++qr){
                    require(changed.prod(ql,qr)==scan(expected,ql,qr));
                    require(changed.prod(ql,qr)==acl.prod(ql,qr));
                    require(original.prod(ql,qr)==scan(a,ql,qr));
                }
            }
        }
    }
}

void affine_branches(){
    std::mt19937_64 rng(2026102202);
    for(int trial=0;trial<240;++trial){
        const int n=1+static_cast<int>(rng()%48);
        std::vector<S>a(n);
        for(auto&x:a)x={static_cast<ll>(rng()%17),1};
        std::vector<Tree>versions{Tree(a)};
        std::vector<ACL>acls{ACL(a)};
        std::vector<std::vector<S>>arrays{a};
        for(int step=0;step<200;++step){
            const std::size_t base=static_cast<std::size_t>(rng()%versions.size());
            Tree next=versions[base];ACL acl=acls[base];auto expected=arrays[base];
            int l=static_cast<int>(rng()%static_cast<unsigned>(n+1));
            int r=static_cast<int>(rng()%static_cast<unsigned>(n+1));
            if(l>r)std::swap(l,r);
            const int p=static_cast<int>(rng()%static_cast<unsigned>(n));
            const F tag{static_cast<ll>(rng()%6),static_cast<ll>(rng()%9)};
            const int before=next.nodes_used();
            switch(rng()%4){
                case 0:{S x{static_cast<ll>(rng()%101),1};next.set(p,x);acl.set(p,x);expected[p]=x;break;}
                case 1:next.apply(p,tag);acl.apply(p,tag);expected[p]=mapping(tag,expected[p]);break;
                default:
                    next.apply(l,r,tag);acl.apply(l,r,tag);
                    for(int i=l;i<r;++i)expected[i]=mapping(tag,expected[i]);
            }
            require(next.nodes_used()-before<=12*static_cast<int>(std::bit_width(static_cast<unsigned>(n-1)))+1);
            require(next.all_prod()==scan(expected,0,n)&&versions[base].all_prod()==scan(arrays[base],0,n));
            versions.push_back(next);acls.push_back(acl);arrays.push_back(expected);
            for(int sample=0;sample<8;++sample){
                const std::size_t v=static_cast<std::size_t>(rng()%versions.size());
                const Tree&reader=versions[v];
                const int used=reader.nodes_used();
                int ql=static_cast<int>(rng()%static_cast<unsigned>(n+1));
                int qr=static_cast<int>(rng()%static_cast<unsigned>(n+1));
                if(ql>qr)std::swap(ql,qr);
                const int i=static_cast<int>(rng()%static_cast<unsigned>(n));
                require(reader.get(i)==arrays[v][i]&&reader.get(i)==acls[v].get(i));
                require(reader.prod(ql,qr)==scan(arrays[v],ql,qr)&&reader.prod(ql,qr)==acls[v].prod(ql,qr));
                const ll limit=static_cast<ll>(rng()%static_cast<unsigned>(n+1));
                auto pred=[&](S x){return x.len<=limit;};
                require(reader.max_right(ql,pred)==std::min<ll>(n,ql+limit));
                require(reader.min_left(qr,pred)==std::max<ll>(0,qr-limit));
                require(reader.max_right(ql,pred)==acls[v].max_right(ql,pred));
                require(reader.min_left(qr,pred)==acls[v].min_left(qr,pred));
                require(reader.nodes_used()==used);
            }
        }
        for(std::size_t v=0;v<versions.size();++v)for(int p=0;p<n;++p)require(versions[v].get(p)==arrays[v][p]);
    }
}

struct Paint{
    ll value;
    Paint()=delete;
    explicit Paint(ll x):value(x){}
};
ll maximum(ll a,ll b){return std::max(a,b);}
ll lowest(){return LLONG_MIN;}
ll paint(Paint f,ll x){return f.value==LLONG_MIN?x:f.value;}
Paint compose_paint(Paint f,Paint g){return f.value==LLONG_MIN?g:f;}
Paint no_paint(){return Paint(LLONG_MIN);}
using MaxTree=PersistentLazySegmentTree<ll,maximum,lowest,Paint,paint,compose_paint,no_paint,200000>;
using MaxACL=atcoder::lazy_segtree<ll,maximum,lowest,Paint,paint,compose_paint,no_paint>;

void nonidentity_mapping(){
    std::mt19937_64 rng(73);
    for(int trial=0;trial<100;++trial){
        const int n=1+static_cast<int>(rng()%40);
        std::vector<MaxTree>versions{MaxTree(n)};
        std::vector<MaxACL>acls{MaxACL(n)};
        std::vector<std::vector<ll>>arrays{std::vector<ll>(n,LLONG_MIN)};
        for(int step=0;step<120;++step){
            const std::size_t base=static_cast<std::size_t>(rng()%versions.size());
            auto next=versions[base];auto acl=acls[base];auto expected=arrays[base];
            int l=static_cast<int>(rng()%static_cast<unsigned>(n+1));
            int r=static_cast<int>(rng()%static_cast<unsigned>(n+1));
            if(l>r)std::swap(l,r);
            const ll value=static_cast<ll>(rng()%31)-15;
            if(step%3==0){l=0;r=n;}
            next.apply(l,r,Paint(value));acl.apply(l,r,Paint(value));
            for(int i=l;i<r;++i)expected[i]=value;
            if(step%5==0){
                const int p=static_cast<int>(rng()%static_cast<unsigned>(n));
                next.set(p,-20);acl.set(p,-20);expected[p]=-20;
            }
            versions.push_back(next);acls.push_back(acl);arrays.push_back(expected);
            for(int sample=0;sample<6;++sample){
                const std::size_t v=static_cast<std::size_t>(rng()%versions.size());
                const auto&reader=versions[v];
                const int used=reader.nodes_used();
                const int p=static_cast<int>(rng()%static_cast<unsigned>(n));
                int ql=static_cast<int>(rng()%static_cast<unsigned>(n+1));
                int qr=static_cast<int>(rng()%static_cast<unsigned>(n+1));
                if(ql>qr)std::swap(ql,qr);
                const ll limit=static_cast<ll>(rng()%41)-20;
                auto pred=[&](ll x){return x<=limit;};
                int right=ql,left=qr;
                while(right<n&&arrays[v][right]<=limit)++right;
                while(left>0&&arrays[v][left-1]<=limit)--left;
                const ll product=ql==qr?LLONG_MIN:*std::max_element(arrays[v].begin()+ql,arrays[v].begin()+qr);
                require(reader.prod(ql,qr)==product&&reader.prod(ql,qr)==acls[v].prod(ql,qr));
                require(reader.get(p)==arrays[v][p]&&reader.get(p)==acls[v].get(p));
                require(reader.max_right(ql,pred)==right&&reader.max_right(ql,pred)==acls[v].max_right(ql,pred));
                require(reader.min_left(qr,pred)==left&&reader.min_left(qr,pred)==acls[v].min_left(qr,pred));
                require(reader.nodes_used()==used);
            }
        }
    }
}

struct Text{std::string s;ll len;};
Text text_op(Text a,Text b){return {a.s+b.s,a.len+b.len};}
Text text_e(){return {"",0};}
Text text_mapping(char f,Text x){return {f?std::string(static_cast<std::size_t>(x.len),f):x.s,x.len};}
char text_composition(char f,char g){return f?f:g;}
char text_id(){return 0;}
void noncommutative(){
    using TextTree=PersistentLazySegmentTree<Text,text_op,text_e,char,text_mapping,text_composition,text_id,20000>;
    std::vector<TextTree>versions{TextTree(std::vector<Text>{{"a",1},{"b",1},{"c",1},{"d",1},{"e",1}})};
    std::vector<std::string>arrays{"abcde"};
    std::mt19937_64 rng(4869);
    for(int step=0;step<150;++step){
        const std::size_t base=static_cast<std::size_t>(rng()%versions.size());
        auto next=versions[base];auto a=arrays[base];
        int l=static_cast<int>(rng()%6),r=static_cast<int>(rng()%6);
        if(l>r)std::swap(l,r);
        const char c=static_cast<char>('a'+rng()%6);
        next.apply(l,r,c);
        std::fill(a.begin()+l,a.begin()+r,c);
        versions.push_back(next);arrays.push_back(a);
    }
    for(std::size_t v=0;v<versions.size();++v)for(int l=0;l<=5;++l)for(int r=l;r<=5;++r){
        const std::string target=arrays[v].substr(static_cast<std::size_t>(l),static_cast<std::size_t>(r-l));
        require(versions[v].prod(l,r).s==target);
        auto prefix=[&](Text x){return target.starts_with(x.s);};
        auto suffix=[&](Text x){return target.ends_with(x.s);};
        int right=l,left=r;
        std::string current;
        while(right<5&&target.starts_with(current+arrays[v][right]))current+=arrays[v][right++];
        current.clear();
        while(left>0&&target.ends_with(arrays[v][left-1]+current))current=arrays[v][--left]+current;
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
Value value_op(Value a,Value b){return Value(std::max(a.x,b.x));}
Value value_e(){return Value(LLONG_MIN);}
Value value_mapping(Paint f,Value x){return Value(paint(f,x.x));}

void boundaries(){
    Tree empty;
    require(empty.size()==0&&empty.nodes_used()==0&&empty.all_prod()==e());
    empty.apply(0,0,F{2,3});
    require(empty.prod(0,0)==e()&&empty.nodes_used()==0);
    require(empty.max_right<short_segment>(0)==0&&empty.min_left<short_segment>(0)==0);
    Tree identities(3);
    identities.apply(0,3,F{2,7});
    require(identities.get(1)==e()&&identities.all_prod()==e());
    Tree sample(std::vector<S>{{1,1},{2,1},{3,1}});
    auto old=sample;
    sample.apply(0,3,F{2,1});
    auto intermediate=sample;
    sample.apply(0,3,F{3,4});
    require(sample.get(0).sum==13&&sample.get(1).sum==19&&sample.get(2).sum==25);
    require(old.all_prod().sum==6&&intermediate.all_prod().sum==15);
    sample.set(1,S{9,1});
    sample.apply(2,F{0,8});
    require(sample.all_prod().sum==30&&sample.max_right<short_segment>(0)==2&&sample.min_left<short_segment>(3)==1);
    const int used=sample.nodes_used();
    sample.apply(2,2,F{0,100});
    require(sample.nodes_used()==used);
    MaxTree huge(LLONG_MAX);
    require(huge.nodes_used()==0);
    huge.apply(0,LLONG_MAX,Paint(4));
    auto huge_old=huge;
    huge.apply(LLONG_MAX/2,LLONG_MAX,Paint(7));
    huge.set(0,9);
    require(huge_old.get(LLONG_MAX-1)==4&&huge.get(LLONG_MAX/2-1)==4&&huge.get(LLONG_MAX-1)==7);
    require(huge.max_right(1,[](ll x){return x<=4;})==LLONG_MAX/2);
    require(huge.min_left(LLONG_MAX,[](ll x){return x<=6;})==LLONG_MAX);
    require(huge.min_left(LLONG_MAX,[](ll x){return x<=7;})==1);
    require(huge_old.prod(0,LLONG_MAX)==4&&huge.prod(0,0)==LLONG_MIN);
    require(huge.nodes_used()<1000);
    require(Value::live==0);
    {
        using Wrapped=PersistentLazySegmentTree<Value,value_op,value_e,Paint,value_mapping,compose_paint,no_paint,100>;
        Wrapped survivor;
        {
            Wrapped original(4);
            original.apply(0,4,Paint(8));
            survivor=original;
            original.set(2,Value(10));
            require(survivor.get(2).x==8&&original.get(2).x==10);
        }
        require(survivor.get(3).x==8);
        require(Value::live==survivor.nodes_used());
        Wrapped moved(std::move(survivor));
        require(survivor.size()==0&&survivor.all_prod().x==LLONG_MIN&&moved.all_prod().x==8);
        survivor=std::move(moved);
        require(moved.size()==0&&moved.nodes_used()==0&&survivor.all_prod().x==8);
        Wrapped*alias=&survivor;
        survivor=std::move(*alias);
        require(survivor.get(0).x==8);
    }
    require(Value::live==0);
    using Small=PersistentLazySegmentTree<S,op,e,F,mapping,composition,id,1>;
    Small small(1);
    small.apply(0,1,F{1,2});
    require(small.nodes_used()==1);
    require(stops_silently([]{Tree invalid(-1);}));
    require(stops_silently([&]{small.apply(0,F{1,1});}));
    require(stops_silently([&]{empty.apply(0,F{1,1});}));
    require(stops_silently([&]{sample.set(-1,S{0,1});}));
    require(stops_silently([&]{sample.set(3,S{0,1});}));
    require(stops_silently([&]{sample.get(3);}));
    require(stops_silently([&]{sample.apply(-1,1,F{1,0});}));
    require(stops_silently([&]{sample.apply(2,1,F{1,0});}));
    require(stops_silently([&]{sample.apply(0,4,F{1,0});}));
    require(stops_silently([&]{sample.apply(LLONG_MAX,F{1,0});}));
    require(stops_silently([&]{sample.prod(2,1);}));
    require(stops_silently([&]{sample.prod(0,4);}));
    require(stops_silently([&]{sample.max_right(4,[](S){return true;});}));
    require(stops_silently([&]{sample.min_left(-1,[](S){return true;});}));
    require(stops_silently([&]{sample.max_right(3,[](S){return false;});}));
    require(stops_silently([&]{sample.min_left(0,[](S){return false;});}));
}

int main(){exhaustive();affine_branches();nonidentity_mapping();noncommutative();boundaries();std::cout<<"Persistent Lazy Segment Tree: "<<checks<<" checks passed; 60000 branching updates, ACL comparison.\n";}
