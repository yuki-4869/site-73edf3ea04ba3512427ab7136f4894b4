#include "MathCombinatorics/and_zero_permutation.cpp"
#include <functional>
#include <iostream>

void require(bool condition) {
    if (!condition) throw std::runtime_error("AND-zero permutation test failed");
}

// Independent existence oracle using maximum bipartite matching.
bool feasible(int n, bool zero_indexed) {
    std::vector<int> matched(n, -1);
    const int first = zero_indexed ? 0 : 1;
    for (int a = 0; a < n; ++a) {
        std::vector<bool> seen(n);
        std::function<bool(int)> dfs = [&](int v) {
            for (int b = 0; b < n; ++b) {
                if (seen[b] || ((v + first) & (b + first)) != 0) continue;
                seen[b] = true;
                if (matched[b] < 0 || dfs(matched[b])) {
                    matched[b] = v;
                    return true;
                }
            }
            return false;
        };
        if (!dfs(a)) return false;
    }
    return true;
}

void check(int n, bool zero_indexed) {
    const auto b = and_zero_permutation(n, zero_indexed);
    const bool possible = zero_indexed || n % 2 == 0;
    if (n <= 24) require(possible == feasible(n, zero_indexed));
    if (!possible) { require(b.empty()); return; }
    require(b.size() == static_cast<std::size_t>(n));
    std::vector<bool> seen(n);
    const int first = zero_indexed ? 0 : 1;
    for (int i = 0; i < n; ++i) {
        const auto index = b[i] - first;
        require(0 <= index && index < n);
        require(!seen[static_cast<std::size_t>(index)]);
        seen[static_cast<std::size_t>(index)] = true;
        require(((i + first) & b[i]) == 0);
    }
}

int main() {
    require(and_zero_permutation(0).empty());
    require(and_zero_permutation(0, false).empty());
    require(and_zero_permutation(5) == std::vector<long long>{0, 2, 1, 4, 3});
    require(and_zero_permutation(4, false) == std::vector<long long>{2, 1, 4, 3});
    for (int n = 0; n <= 1024; ++n) for (bool zero : {false, true}) check(n, zero);
    for (int k = 1; k <= 20; ++k) for (int delta : {-1, 0, 1})
        for (bool zero : {false, true}) check((1 << k) + delta, zero);
    bool rejected = false;
    try { and_zero_permutation(-1); }
    catch (const std::invalid_argument&) { rejected = true; }
    require(rejected);
    std::cout << "AND-zero permutation: matching oracle, range, bijection, AND, and boundary checks passed.\n";
}
