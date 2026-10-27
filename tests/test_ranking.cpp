// randomized test vs brute force. Build: g++ -std=c++17 -O2 test_ranking.cpp -o test_ranking
#include <iostream>
#include <map>
#include <random>
#include <algorithm>
#include <cassert>
#include "ranking_engine.h"
using namespace std;

int main() {
    const int MAXS = 500;
    RankingEngine eng(MAXS);
    map<int,int> ref;                  // id -> score
    mt19937 rng(12345);
    auto rnd = [&](int a, int b) { return (int)(rng() % (b - a + 1)) + a; };

    auto sortedRef = [&]() {
        vector<pair<int,int>> v;       // (score,id)
        for (auto& kv : ref) v.push_back({kv.second, kv.first});
        sort(v.begin(), v.end(), [](auto& a, auto& b) {
            return a.first != b.first ? a.first > b.first : a.second < b.second; });
        return v;
    };

    for (int step = 0; step < 20000; ++step) {
        int op = rnd(0, 9);
        if (op <= 5) {                                  // add/update
            int id = rnd(1, 300), s = rnd(0, MAXS);     // many ties on purpose
            User u; u.id = id; u.name = "u" + to_string(id); u.score = s;
            eng.addOrUpdate(u); ref[id] = s;
        } else if (op == 6) {                           // remove
            int id = rnd(1, 300);
            assert(eng.remove(id) == (ref.erase(id) > 0));
        }
        if (step % 50 == 0) {
            auto v = sortedRef();
            assert(eng.size() == (int)v.size());
            for (int i = 0; i < (int)v.size(); ++i)
                assert(eng.rank(v[i].second) == i + 1);
            int k = rnd(0, 20);
            auto t1 = eng.topK(k), t2 = eng.topKTreap(k);
            int m = min<int>(k, v.size());
            assert((int)t1.size() == m && (int)t2.size() == m);
            for (int i = 0; i < m; ++i) {
                assert(t1[i].id == v[i].second && t2[i].id == v[i].second);
            }
            int lo = rnd(0, MAXS), hi = rnd(0, MAXS);
            int cnt = 0, mx = -1;
            for (auto& p : v) if (p.first >= lo && p.first <= hi) { cnt++; mx = max(mx, p.first); }
            assert(eng.countInRange(lo, hi) == cnt);
            assert(eng.maxInRange(lo, hi) == mx);
        }
    }
    assert(eng.rank(99999) == -1);
    cout << "ALL TESTS PASSED (" << eng.size() << " users at end)\n";
}
