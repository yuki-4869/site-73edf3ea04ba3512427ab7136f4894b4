#include <bits/stdc++.h>
#include "silent-check.hpp"
using namespace std;
using ll=long long;
using LL=__int128;
#include "DiscreteConvexity/slope_trick.cpp"

size_t checked=0,states=0;
void require(bool ok){if(!ok)abort();}
struct Table{
    int bound;
    vector<LL> value;
    LL left_slope=0,right_slope=0;
    explicit Table(int b,LL c=0):bound(b),value(2*b+1,c){}
    LL eval(int x)const{
        if(x<-bound)return value.front()+left_slope*(x+bound);
        if(x>bound)return value.back()+right_slope*(x-bound);
        return value[x+bound];
    }
    void add_const(LL c){for(auto&v:value)v+=c;}
    void add_left(int a){for(int x=-bound;x<=bound;++x)value[x+bound]+=max(0,a-x);--left_slope;}
    void add_right(int a){for(int x=-bound;x<=bound;++x)value[x+bound]+=max(0,x-a);++right_slope;}
    void add_abs(int a){for(int x=-bound;x<=bound;++x)value[x+bound]+=abs(x-a);--left_slope;++right_slope;}
    void prefix_min(){for(size_t i=1;i<value.size();++i)value[i]=min(value[i],value[i-1]);right_slope=0;}
    void suffix_min(){for(int i=int(value.size())-2;i>=0;--i)value[i]=min(value[i],value[i+1]);left_slope=0;}
    void shift(int a,int b){
        auto old=*this;
        for(int x=-bound;x<=bound;++x){
            LL best=old.eval(x-a);
            for(int d=a+1;d<=b;++d)best=min(best,old.eval(x-d));
            value[x+bound]=best;
        }
    }
    void clear(){fill(value.begin(),value.end(),0);left_slope=right_slope=0;}
    void merge(const Table&other){
        require(bound==other.bound);
        for(size_t i=0;i<value.size();++i)value[i]+=other.value[i];
        left_slope+=other.left_slope;right_slope+=other.right_slope;
    }
};
void verify(const SlopeTrick&st,const Table&t){
    ++states;
    LL best=*min_element(t.value.begin(),t.value.end());
    require(st.min_value()==best);
    auto [l,r]=st.argmin();
    optional<LL> expected_l,expected_r;
    if(t.left_slope<0)expected_l=LL(find(t.value.begin(),t.value.end(),best)-t.value.begin())-t.bound;
    if(t.right_slope>0)expected_r=LL(t.value.size()-1-(find(t.value.rbegin(),t.value.rend(),best)-t.value.rbegin()))-t.bound;
    require(l==expected_l && r==expected_r);
    if(l)require(*l>-t.bound);
    if(r)require(*r<t.bound);
    for(int x=-t.bound-2;x<=t.bound+2;++x){require(st.eval(x)==t.eval(x));++checked;}
    require(st.min_value()==best && st.argmin()==make_pair(l,r));
}
void operate(SlopeTrick&st,Table&t,int type){
    switch(type){
        case 0:st.add_const(-2);t.add_const(-2);break;
        case 1:st.add_a_minus_x(-2);t.add_left(-2);break;
        case 2:st.add_a_minus_x(2);t.add_left(2);break;
        case 3:st.add_x_minus_a(-2);t.add_right(-2);break;
        case 4:st.add_x_minus_a(2);t.add_right(2);break;
        case 5:st.add_abs(0);t.add_abs(0);break;
        case 6:st.prefix_min();t.prefix_min();break;
        case 7:st.suffix_min();t.suffix_min();break;
        case 8:st.shift(-1);t.shift(-1,-1);break;
        case 9:st.shift(2);t.shift(2,2);break;
        case 10:st.shift(-1,2);t.shift(-1,2);break;
        default:st.clear();t.clear();break;
    }
}
void exhaustive(const SlopeTrick&st,const Table&t,int depth){
    verify(st,t);
    if(depth==0)return;
    for(int type=0;type<12;++type){auto next=st;auto table=t;operate(next,table,type);exhaustive(next,table,depth-1);}
}
void compare_dp(const vector<int>&a,const vector<int>&d,bool monotone){
    constexpr int b=6;
    vector<LL> dp(2*b+1,0);
    SlopeTrick st;
    for(size_t i=0;i<a.size();++i){
        auto old=dp;
        for(int x=-b;x<=b;++x){
            LL best=LL(1)<<100;
            for(int y=-b;y<=b;++y)if(monotone?y<=x:abs(x-y)<=d[i])best=min(best,old[y+b]);
            dp[x+b]=best+abs(x-a[i]);
        }
        if(monotone)st.prefix_min();else st.shift(-d[i],d[i]);
        st.add_abs(a[i]);
        require(st.min_value()==*min_element(dp.begin(),dp.end()));
        ++checked;
    }
}
void exhaustive_dp(vector<int>&a,int depth){
    compare_dp(a,vector<int>(a.size(),1),true);
    compare_dp(a,vector<int>(a.size(),1),false);
    if(depth==0)return;
    for(int x:{-2,0,2}){a.push_back(x);exhaustive_dp(a,depth-1);a.pop_back();}
}
void extreme(){
    LL lo=LLONG_MIN,hi=LLONG_MAX;
    SlopeTrick st(-13);
    st.add_abs(lo);st.add_abs(hi);
    require(st.size()==4 && st.min_value()==hi-lo-13);
    require(st.argmin()==make_pair(optional<LL>(lo),optional<LL>(hi)));
    LL huge=LL(1)<<100;
    require(st.eval(huge)==2*huge-lo-hi-13);
    st.shift(hi);st.shift(hi);
    require(st.argmin()==make_pair(optional<LL>(lo+2*hi),optional<LL>(3*hi)));
    require(st.eval(huge)==2*huge-lo-5*hi-13);
    st.shift(-huge,huge);
    require(st.argmin()==make_pair(optional<LL>(lo+2*hi-huge),optional<LL>(3*hi+huge)));
    require(st.min_value()==hi-lo-13);
    st.add_const(-huge);
    require(st.eval(0)==hi-lo-13-huge);
    st.clear();require(st.min_value()==0 && st.size()==0);
    st.add_a_minus_x(lo);st.add_x_minus_a(hi);
    require(st.min_value()==0 && st.eval(0)==0);
    auto original=st;
    st.merge(st);
    require(st.size()==4 && st.eval(-huge)==2*original.eval(-huge));
    require(stops_silently([]{SlopeTrick s;s.shift(2,1);}));
}
void large(){
    constexpr int n=200000;
    vector<LL> a;a.reserve(n);
    SlopeTrick st;
    for(int i=0;i<n;++i){LL x=LL(i)*1000000000000LL-LL(n/2)*1000000000000LL;a.push_back(x);st.add_abs(x);}
    LL expected=0;
    for(LL x:a)expected+=x<0?-x:x;
    require(st.size()==2*n && st.min_value()==expected && st.eval(0)==expected);
    require(st.argmin()==make_pair(optional<LL>(a[n/2-1]),optional<LL>(a[n/2])));
    st.shift(-100,300);require(st.min_value()==expected);
    st.prefix_min();require(st.size()==n && !st.argmin().second);
    st.suffix_min();require(st.size()==0 && st.eval(LL(1)<<100)==expected);

    constexpr int m=32768;
    vector<SlopeTrick> tree(m);
    for(int i=0;i<m;++i)tree[i].add_abs(i-m/2);
    for(int width=1;width<m;width*=2)for(int i=0;i<m;i+=2*width){
        tree[i].merge(tree[i+width]);
        require(tree[i+width].size()==0 && tree[i+width].min_value()==0);
    }
    LL total=0;for(int i=0;i<m;++i)total+=abs(i-m/2);
    require(tree[0].min_value()==total && tree[0].eval(0)==total && tree[0].size()==2*m);
}
int main(){
    exhaustive(SlopeTrick(3),Table(24,3),5);
    vector<int>a;exhaustive_dp(a,7);
    mt19937_64 gen(4869);
    for(int run=0;run<1000;++run){
        SlopeTrick st(-7);Table t(128,-7);
        for(int step=0;step<40;++step){
            operate(st,t,int(gen()%12));verify(st,t);
            if(step%11==0){auto copy=st;auto table=t;copy.add_const(19);table.add_const(19);verify(copy,table);verify(st,t);}
        }
    }
    for(int run=0;run<3000;++run){
        SlopeTrick x,y;Table a1(64),a2(64);
        int n=int(gen()%20),m=int(gen()%20);
        for(int i=0;i<n;++i)operate(x,a1,int(gen()%11));
        for(int i=0;i<m;++i)operate(y,a2,int(gen()%11));
        auto sum=a1;sum.merge(a2);
        size_t count=x.size()+y.size();
        x.merge(y);verify(x,sum);verify(y,Table(64));require(x.size()==count);
        auto twice=sum;twice.merge(sum);count=x.size();
        x.merge(x);verify(x,twice);require(x.size()==2*count);
        x.add_abs(7);twice.add_abs(7);verify(x,twice);
        y.add_abs(-9);Table reused(64);reused.add_abs(-9);verify(y,reused);
        vector<int> targets(1+gen()%12),d(targets.size());
        for(auto&v:targets)v=int(gen()%13)-6;
        for(auto&v:d)v=gen()%4;
        compare_dp(targets,d,true);compare_dp(targets,d,false);
    }
    extreme();large();
    cout<<"Passed "<<checked<<" value/DP comparisons across "<<states<<" function states; exhaustive operation sequences, merges, self-merge, 128-bit cases, 200000 absolute costs, balanced tree merges and silent-stop check\n";
}
