#include <bits/stdc++.h>
#include "silent-check.hpp"
#include "DataStructureBasic/coordinate_compression.cpp"
#include <cstdlib>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <tuple>

void check(bool ok,int line) {
    if (!ok) { std::cerr << "Check failed at line " << line << '\n'; std::abort(); }
}
#define CHECK(...) check(static_cast<bool>((__VA_ARGS__)),__LINE__)

template<class Error,class F>
void expect_throw(F operation) {
    CHECK(stops_silently(operation));
}

void regressions() {
    Zip<long long> z;
    CHECK(z.ready() && z.size()==0 && z.empty() && z.pending_size()==0);
    CHECK(z.get(10)==0 && z.index(10)==-1 && !z.contains(10));
    CHECK(z.zip({10,20})==std::vector<int>({0,0}));
    CHECK(z.begin()==z.end() && z.unzip({}).empty());
    expect_throw<std::out_of_range>([&]{ z.zip_exact({10}); });
    expect_throw<std::out_of_range>([&]{ z.at(0); });
    z.add(30); z.add({10,30});
    CHECK(!z.ready() && z.pending_size()==3);
    expect_throw<std::logic_error>([&]{ z.get(10); });
    expect_throw<std::logic_error>([&]{ z.size(); });
    expect_throw<std::logic_error>([&]{ z.empty(); });
    expect_throw<std::logic_error>([&]{ z[0]; });
    expect_throw<std::logic_error>([&]{ z.values(); });
    expect_throw<std::logic_error>([&]{ z.begin(); });
    expect_throw<std::logic_error>([&]{ z.end(); });
    expect_throw<std::logic_error>([&]{ z.zip({}); });
    expect_throw<std::logic_error>([&]{ z.zip_exact({}); });
    expect_throw<std::logic_error>([&]{ z.unzip({}); });
    z.build();
    CHECK(z.ready() && z.size()==2 && z[0]==10 && z[1]==30);
    CHECK(z.get(20)==1 && z.lower(20)==1 && z.upper(20)==1 && z.index(20)==-1);
    CHECK(z.index(30)==1 && z.contains(30));
    CHECK(z.equal_range(30)==std::pair(1,2));
    CHECK(z.zip({30,20,10})==std::vector<int>({1,1,0}));
    CHECK(z.zip_exact({30,10})==std::vector<int>({1,0}));
    CHECK(z.unzip({1,0})==std::vector<long long>({30,10}));
    CHECK(z.count_range(10,30)==1 && z.count_range(30,10)==0);
    expect_throw<std::out_of_range>([&]{ z[-1]; });
    expect_throw<std::out_of_range>([&]{ z[z.size()]; });
    expect_throw<std::out_of_range>([&]{ z.unzip({2}); });
    expect_throw<std::out_of_range>([&]{ z.zip_exact({20}); });
    z.reserve(64); z.build();
    CHECK(z.ready() && z.pending_size()==2);
    z.add(std::vector<long long>{}); z.add({});
    CHECK(z.ready());
    z.add(z.values());
    CHECK(!z.ready() && z.pending_size()==4);
    z.build(); CHECK(z.size()==2);
    z.add(z[0]); z.build(); CHECK(z.size()==2);
    const int old_index=z.index(30);
    z.add(0); z.build(); CHECK(z.index(30)==old_index+1);
    z.assign(z.values()); CHECK(z.size()==3);
    z.assign({5,5,-5}); CHECK(z.values()==std::vector<long long>({-5,5}));

    Zip<long long> copied=z;
    copied.add(100); copied.build(); CHECK(z.size()==2 && copied.size()==3);
    Zip<long long> moved=std::move(copied);
    CHECK(copied.ready() && copied.empty() && moved.size()==3);
    copied=std::move(moved);
    CHECK(moved.ready() && moved.empty() && copied.size()==3);
    moved=copied; moved.clear(); CHECK(copied.size()==3 && moved.empty());
    copied.add(200); Zip<long long> dirty=std::move(copied);
    CHECK(!dirty.ready() && copied.ready() && copied.empty());
    dirty.build(); CHECK(dirty.size()==4);
    Zip<long long> dirty_copy; dirty_copy.add(1); moved=dirty_copy;
    CHECK(!moved.ready()); moved.build(); CHECK(moved.size()==1);
}

struct Record { int key; int payload; }; // == / < を定義しない型
struct ByKey { bool operator()(const Record& a,const Record& b) const { return a.key<b.key; } };
struct Direction {
    bool descending;
    explicit Direction(bool d):descending(d) {}
    bool operator()(int a,int b) const { return descending ? a>b : a<b; }
};

void types_and_comparators() {
    const long long lo=std::numeric_limits<long long>::min(),hi=std::numeric_limits<long long>::max();
    Zip<long long> extreme{hi,lo,0,hi};
    CHECK(extreme.values()==std::vector<long long>({lo,0,hi}));
    CHECK(extreme.get(lo)==0 && extreme.upper(hi)==3 && extreme.index(hi)==2);
    CHECK(extreme.count_range(lo,hi)==2);
    Zip<std::string> words{"pear","apple","pear","banana"};
    CHECK(words.zip_exact({"banana","apple"})==std::vector<int>({1,0}));
    Zip<std::pair<int,int>> pairs{{1,3},{1,2},{0,100}};
    CHECK(pairs.index({1,2})==1 && pairs.index({1,3})==2);
    Zip<std::tuple<int,int,int>> tuples{{1,2,3},{1,2,1}};
    CHECK(tuples.index({1,2,1})==0);
    Zip<Record,ByKey> records({{2,10},{1,100},{2,20}},ByKey{});
    CHECK(records.size()==2 && records.index({2,999})==1 && records[0].key==1);
    CHECK(records.unzip({1})[0].key==2);
    Zip<int,Direction> descending({10,30,20,30},Direction(true));
    CHECK(descending.values()==std::vector<int>({30,20,10}));
    CHECK(descending.lower(25)==1 && descending.upper(20)==2 && descending.count_range(30,10)==2);
    auto moved=std::move(descending);
    descending.add({1,3,2}); descending.build();
    CHECK(descending.values()==std::vector<int>({3,2,1}) && moved.values()[0]==30);
    Zip<int,Direction> comparison_only(Direction(true));
    comparison_only.add(10); comparison_only.build(); CHECK(comparison_only.index(10)==0);
    Zip<bool> flags(std::vector<bool>{true,false,true});
    static_assert(std::is_same_v<decltype(flags[0]),bool>);
    CHECK(flags.size()==2 && !flags[0] && flags[1]);
    CHECK(flags.zip_exact({true,false})==std::vector<int>({1,0}));
    CHECK(flags.unzip({1,0})==std::vector<bool>({true,false}));
    flags.add(flags[0]); flags.build(); CHECK(flags.size()==2);
    flags.add(flags.values()); flags.build(); CHECK(flags.size()==2);
    const std::vector<std::pair<int,long long>> points{{20,100},{10,200},{20,200}};
    CHECK(coordinate_zip::zip2d(points)==std::vector<std::pair<int,int>>({{1,0},{0,1},{1,1}}));
    CHECK(coordinate_zip::zip2d(points,std::greater<int>{},std::greater<long long>{})
          ==std::vector<std::pair<int,int>>({{0,1},{1,0},{0,0}}));
    CHECK(coordinate_zip::zip(std::vector<int>{30,10,30,20})==std::vector<int>({2,0,2,1}));
    CHECK(coordinate_zip::zip(std::vector<int>{30,10,30,20},std::greater<int>{})==std::vector<int>({0,2,0,1}));
    CHECK(coordinate_zip::zip2d(std::vector<std::pair<int,int>>{}).empty());
    CHECK(coordinate_zip::zip(std::vector<int>{}).empty());
}

template<class T,class Compare>
std::size_t randomized(Compare cmp) {
    constexpr int seeds=64,steps=1024;
    for(int seed=0;seed<seeds;++seed) {
        std::mt19937_64 rng(0x5a6970436f6f7264ULL+static_cast<std::uint64_t>(seed));
        Zip<T,Compare> z(cmp);
        std::set<T,Compare> reference(cmp);
        for(int step=0;step<steps;++step) {
            const T x=static_cast<T>(rng()%81)-40;
            const int operation=static_cast<int>(rng()%8);
            if(operation<=2) { z.add(x); reference.insert(x); }
            else if(operation<=4) {
                std::vector<T> values;
                for(int i=0;i<8;++i) values.push_back(static_cast<T>(rng()%81)-40);
                z.add(values); reference.insert(values.begin(),values.end());
            } else if(operation==5) { z.reserve(128); }
            else if(operation==6) {
                z.assign({x,static_cast<T>(x+1),x});
                reference.clear(); reference.insert(x); reference.insert(static_cast<T>(x+1));
            } else { z.clear(); reference.clear(); }
            if(!z.ready()) expect_throw<std::logic_error>([&]{ z.lower(x); });
            z.build(); z.build();
            CHECK(z.ready() && z.size()==static_cast<int>(reference.size()));
            CHECK(z.empty()==reference.empty());
            const std::vector<T> dictionary(reference.begin(),reference.end());
            CHECK(z.values()==dictionary && z.pending_size()==dictionary.size());
            CHECK(std::vector<T>(z.begin(),z.end())==dictionary);
            std::vector<T> queries;
            std::vector<int> ranks;
            for(int i=0;i<16;++i) {
                const T q=static_cast<T>(rng()%101)-50;
                const int lower=static_cast<int>(std::distance(reference.begin(),reference.lower_bound(q)));
                const int upper=static_cast<int>(std::distance(reference.begin(),reference.upper_bound(q)));
                CHECK(z.lower(q)==lower && z.upper(q)==upper && z.get(q)==lower);
                CHECK(z.equal_range(q)==std::pair(lower,upper));
                CHECK(z.index(q)==(reference.count(q)!=0 ? lower : -1));
                CHECK(z.contains(q)==(reference.count(q)!=0));
                queries.push_back(q); ranks.push_back(lower);
            }
            CHECK(z.zip(queries)==ranks);
            const auto ids=z.zip_exact(dictionary);
            CHECK(z.unzip(ids)==dictionary);
            for(std::size_t i=0;i<dictionary.size();++i) {
                CHECK(ids[i]==static_cast<int>(i) && z[static_cast<int>(i)]==dictionary[i]);
            }
            const T left=static_cast<T>(rng()%101)-50,right=static_cast<T>(rng()%101)-50;
            int expected=0;
            for(T value:reference) expected+=(!cmp(value,left) && cmp(value,right));
            CHECK(z.count_range(left,right)==expected);
            if(step%32==0) {
                const std::set<T,Compare> query_values(queries.begin(),queries.end(),cmp);
                std::vector<int> expected_ids;
                for(T q:queries) expected_ids.push_back(static_cast<int>(std::distance(query_values.begin(),query_values.lower_bound(q))));
                CHECK(coordinate_zip::zip(queries,cmp)==expected_ids);
            }
        }
    }
    return static_cast<std::size_t>(seeds)*steps;
}

int main() {
    regressions(); types_and_comparators();
    const auto steps=randomized<long long>(std::less<long long>{})+randomized<int>(std::greater<int>{});
    std::cout << "Coordinate compression: regressions, types, and " << steps << " randomized steps passed\n";
}
