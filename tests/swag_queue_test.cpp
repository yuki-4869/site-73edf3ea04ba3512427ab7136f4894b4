#include <bits/stdc++.h>
#include "silent-check.hpp"
using namespace std;
using ll=long long;
#include "DataStructureBasic/swag_queue.cpp"
size_t checked=0;
void require(bool ok){if(!ok)abort();}
ll add(ll a,ll b){return a+b;}
ll zero(){return 0;}
ll minimum(ll a,ll b){return min(a,b);}
ll infinity(){return LLONG_MAX;}
string concat(string a,string b){return a+b;}
string blank(){return "";}
using Sum=SwagQueue<ll,add,zero>;
template<class S,S(*op)(S,S),S(*e)()>
void verify(const SwagQueue<S,op,e>&q,const deque<S>&d){
    require(q.size()==d.size()&&q.empty()==d.empty());
    S expected=e();for(const auto&x:d)expected=op(expected,x);
    require(q.all_prod()==expected);
    if(!d.empty())require(q.front()==d.front());
    ++checked;
}
void exhaustive(Sum&q,deque<ll>&d,int remaining){
    verify(q,d);
    if(remaining==0)return;
    for(ll x:{-1,2}){
        auto next=q;auto values=d;next.push(x);values.push_back(x);
        exhaustive(next,values,remaining-1);
    }
    if(!d.empty()){
        auto next=q;auto values=d;next.pop();values.pop_front();
        exhaustive(next,values,remaining-1);
    }
}
struct Matrix{
    array<ll,4>a;
    bool operator==(const Matrix&)const=default;
};
Matrix multiply(Matrix x,Matrix y){
    Matrix z{{0,0,0,0}};
    for(int i=0;i<2;++i)for(int j=0;j<2;++j)for(int k=0;k<2;++k)z.a[2*i+j]=(z.a[2*i+j]+x.a[2*i+k]*y.a[2*k+j])%1009;
    return z;
}
Matrix matrix_identity(){return {{1,0,0,1}};}
struct NoDefault{
    ll value;
    NoDefault()=delete;
    explicit NoDefault(ll v):value(v){}
    NoDefault(const NoDefault&)=default;
    NoDefault&operator=(const NoDefault&)=default;
    NoDefault(NoDefault&&)=delete;
    bool operator==(const NoDefault&other)const{return value==other.value;}
};
NoDefault nd_add(NoDefault a,NoDefault b){return NoDefault(a.value+b.value);}
NoDefault nd_zero(){return NoDefault(0);}
size_t op_calls=0;
ll counted_add(ll a,ll b){++op_calls;return a+b;}
int throw_after=-1;
ll sometimes_add(ll a,ll b){
    if(throw_after==0)throw 1;
    if(throw_after>0)--throw_after;
    return a+b;
}
int main(){
    Sum q;deque<ll>d;exhaustive(q,d,10);
    mt19937_64 rng(4869);
    Sum sum;SwagQueue<ll,minimum,infinity> minq;deque<ll> values;
    SwagQueue<Matrix,multiply,matrix_identity> mq;deque<Matrix> matrices;
    SwagQueue<string,concat,blank> sq;deque<string> strings;
    for(int t=0;t<50000;++t){
        if(values.empty()||(values.size()<80&&rng()%3)){
            ll x=ll(rng()%201)-100;
            sum.push(x);minq.push(x);values.push_back(x);
            Matrix m{{ll(rng()%1009),ll(rng()%1009),ll(rng()%1009),ll(rng()%1009)}};
            mq.push(m);matrices.push_back(m);
            string s(1,char('a'+rng()%26));sq.push(s);strings.push_back(s);
        }else{
            sum.pop();minq.pop();values.pop_front();mq.pop();matrices.pop_front();sq.pop();strings.pop_front();
        }
        verify(sum,values);verify(minq,values);verify(mq,matrices);verify(sq,strings);
        if(t%100==0){
            auto copy=sum;auto before=values;copy.push(999);before.push_back(999);verify(copy,before);verify(sum,values);
            auto moved=std::move(copy);verify(moved,before);copy=Sum{};require(copy.empty());
            Sum assigned;assigned=sum;verify(assigned,values);
            assigned=std::move(moved);verify(assigned,before);
        }
    }
    SwagQueue<NoDefault,nd_add,nd_zero> nd;
    nd.push(NoDefault(3));nd.push(NoDefault(5));require(nd.all_prod().value==8);nd.pop();require(nd.front().value==5);
    auto ndcopy=nd;ndcopy.push(NoDefault(7));require(nd.all_prod().value==5&&ndcopy.all_prod().value==12);
    nd.pop();require(nd.all_prod().value==0);
    require(stops_silently([]{Sum empty;empty.pop();}));
    require(stops_silently([]{const Sum empty;empty.front();}));
    require(stops_silently([]{Sum empty;empty.push(1);empty.pop();empty.pop();}));
    require(stops_silently([]{Sum empty;empty.push(1);empty.pop();empty.front();}));
    SwagQueue<ll,sometimes_add,zero> throwing;
    for(ll i=1;i<=8;++i)throwing.push(i);
    throw_after=2;bool caught=false;
    try{throwing.pop();}catch(int){caught=true;}
    throw_after=-1;
    require(caught&&throwing.size()==8&&throwing.front()==1&&throwing.all_prod()==36);
    throwing.pop();require(throwing.front()==2&&throwing.all_prod()==35);
    const ll n=1000000;
    SwagQueue<ll,counted_add,zero> big;
    for(ll i=1;i<=n;++i)big.push(i);
    for(ll i=1;i<=n;++i){
        require(big.front()==i);
        require(big.all_prod()==(i+n)*(n-i+1)/2);
        big.pop();
    }
    require(big.empty()&&big.all_prod()==0);require(op_calls<=size_t(3*n));
    cout<<"Passed "<<checked<<" deque comparisons, noncommutative and copy/move tests; 1000000-element queue, "<<op_calls<<" aggregate calls; 4 silent-stop cases\n";
}
