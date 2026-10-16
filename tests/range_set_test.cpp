#include <bits/stdc++.h>
#include "silent-check.hpp"
#include "DataStructureBasic/range_set.cpp"
#include <cassert>
#include <cstdint>
#include <random>
#include <set>
#include <sstream>
#include <string>

template<class T>
std::vector<std::pair<T,T>> reference_intervals(const std::set<T>& values) {
    std::vector<std::pair<T,T>> result;
    for (T x : values) {
        if (!result.empty() && result.back().second == x) ++result.back().second;
        else result.emplace_back(x, static_cast<T>(x+1));
    }
    return result;
}

void regressions() {
    RangeSet<> r;
    assert(r.empty() && r.size()==0 && r.cover_length()==0);
    assert(r.mex(-10)==-10 && !r.covered(0));
    assert(r.covered(3,3) && r.covered(5,2) && !r.overlaps(3,3));
    assert((r.covered_by(1,2)==std::pair{-r.TINF,-r.TINF}));
    assert((r.covered_by(3,3)==std::pair{0LL,0LL}));
    assert((r.covering_interval(3,2)==std::optional{std::pair{3LL,3LL}}));

    assert(r.insert(5,10)==5 && r.insert(1,5)==4);
    assert(r.covered(1,10) && r.mex(1)==10 && r.size()==1);
    assert((r.intervals()==std::vector<std::pair<long long,long long>>{{1,10}}));
    assert(r.erase(3,7)==4 && r.cover_length()==5);
    assert((r.intervals()==std::vector<std::pair<long long,long long>>{{1,3},{7,10}}));
    assert(r.count_covered(2,9)==3 && !r.covered(1,10));
    assert(r.erase(3,7)==0 && r.insert(3,7)==4 && r.size()==1);
    assert(r.insert(1,10)==0 && r.erase(0,100)==9 && r.empty());

    RangeSet<> a{{-10,-5},{0,5},{8,10}};
    assert(a.insert(-5,8)==8 && a.cover_length()==20 && a.size()==1);
    assert(a.erase(-20,-7)==3 && a.erase(7,20)==3);
    assert((a.intervals()==std::vector<std::pair<long long,long long>>{{-7,7}}));
    assert(a.erase(0)==1 && a.insert(0)==1);
    assert(a.erase(-7,7)==14 && a.empty());

    RangeSet<> b{{-3,3}}, c=b;
    assert(c.erase(0)==1 && b.covered(0));
    RangeSet<> d=std::move(c);
    assert(c.empty() && c.cover_length()==0 && !d.covered(0));
    c=std::move(d);
    assert(d.empty() && d.cover_length()==0 && c.cover_length()==5);
    d=c; d.clear(); assert(c.cover_length()==5);
    c.swap(d); assert(c.empty() && d.cover_length()==5);
    std::ostringstream printed;
    auto* buffer=std::cout.rdbuf(printed.rdbuf()); d.output(); std::cout.rdbuf(buffer);
    assert(printed.str()=="RangeSet: [-3,0) [1,3) \n");
    std::vector<std::pair<long long,long long>> traversed;
    for (auto [left,right] : d) traversed.emplace_back(left,right);
    assert(traversed==d.intervals());
}

void limits() {
    using T=long long;
    const T low=std::numeric_limits<T>::min(), high=std::numeric_limits<T>::max();
    RangeSet<> r;
    assert(r.insert(low)==1 && r.insert(high-1)==1 && r.cover_length()==2);
    assert(r.covered(low) && r.mex(low)==low+1);
    assert(r.mex(high-1)==high && !r.contains(high) && r.erase(high)==0);
    const auto before=r.intervals();
    assert(stops_silently([&]{r.insert(low,high);}));
    assert(r.intervals()==before && r.cover_length()==2);
    assert(stops_silently([&]{r.insert(high);}));
    assert(r.intervals()==before);

    RangeSet<T,__int128_t> wide;
    const __int128_t span=static_cast<__int128_t>(high)-low;
    assert(wide.insert(low,high)==span && wide.cover_length()==span);
    assert(wide.count_covered(low,high)==span && wide.mex(low)==high);
    assert(wide.erase(low,0)==-static_cast<__int128_t>(low));
    assert(wide.cover_length()==high && !wide.covered(-1) && wide.covered(0));
    assert(wide.insert(low,0)==-static_cast<__int128_t>(low) && wide.size()==1);
    assert(wide.erase(low,high)==span && wide.empty());

    RangeSet<T,unsigned long long> unsigned_count;
    assert(unsigned_count.insert(low,high)==std::numeric_limits<unsigned long long>::max());
    assert(unsigned_count.erase(low,high)==std::numeric_limits<unsigned long long>::max());
    RangeSet<short,long long> small_coordinates{{-100,100}};
    assert(small_coordinates.cover_length()==200);
    RangeSet<int,signed char> small_count;
    assert(small_count.insert(0,100)==100);
    assert(stops_silently([&]{small_count.insert(100,200);}));
    assert(small_count.cover_length()==100 && small_count.mex(0)==100);
}

template<class T>
std::size_t randomized() {
    constexpr int seeds=64, steps=4096;
    for(int seed=0;seed<seeds;++seed) {
        std::mt19937_64 rng(0x52616e6765536574ULL+seed);
        RangeSet<T> r;
        std::set<T> reference;
        for(int step=0;step<steps;++step) {
            T l=static_cast<T>(rng()%65)-32, right=static_cast<T>(rng()%65)-32;
            const int kind=rng()%8;
            if(kind<=1) {
                T added=0;
                for(T x=l;x<right;++x) added+=reference.insert(x).second;
                assert(r.insert(l,right)==added);
            } else if(kind<=3) {
                T removed=0;
                for(T x=l;x<right;++x) removed+=static_cast<T>(reference.erase(x));
                assert(r.erase(l,right)==removed);
            } else if(kind==4) {
                const bool added=reference.insert(l).second;
                assert(r.insert(l)==added);
            } else if(kind==5) {
                const T removed=static_cast<T>(reference.erase(l));
                assert(r.erase(l)==removed);
            } else if(kind==6 && rng()%16==0) {
                r.clear(); reference.clear();
            }
            const auto intervals=reference_intervals(reference);
            assert(r.intervals()==intervals && r.size()==intervals.size());
            assert(r.cover_length()==static_cast<T>(reference.size()));
            assert(r.empty()==reference.empty());
            T x=static_cast<T>(rng()%81)-40;
            const bool contains=reference.count(x);
            assert(r.covered(x)==contains && r.contains(x)==contains);
            T expected_mex=x; while(reference.count(expected_mex)) ++expected_mex;
            assert(r.mex(x)==expected_mex);
            std::optional<std::pair<T,T>> expected_point;
            for(auto interval:intervals) if(interval.first<=x && x<interval.second) expected_point=interval;
            assert(r.covering_interval(x)==expected_point);

            T count=0;
            for(T v=l;v<right;++v) count+=static_cast<T>(reference.count(v));
            assert(r.count_covered(l,right)==count);
            assert(r.overlaps(l,right)==(count>0));
            const bool all=l>=right || count==right-l;
            assert(r.covered(l,right)==all);
            std::optional<std::pair<T,T>> expected_interval;
            if(l>=right) expected_interval=std::pair<T,T>{l,l};
            else for(auto interval:intervals) if(interval.first<=l && right<=interval.second) expected_interval=interval;
            assert(r.covering_interval(l,right)==expected_interval);
            auto expected_legacy=l>=right ? std::pair<T,T>{0,0}
                : expected_interval.value_or(std::pair<T,T>{-r.TINF,-r.TINF});
            assert(r.covered_by(l,right)==expected_legacy);
        }
    }
    return static_cast<std::size_t>(seeds)*steps;
}

int main() {
    regressions(); limits();
    const auto operations=randomized<int>()+randomized<long long>();
    std::cout << "RangeSet: regressions, limits, and " << operations
              << " randomized operations passed\n";
}
