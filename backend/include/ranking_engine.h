// ranking_engine.h - Member 1's facade. The menu only needs this class.
#pragma once
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "common.h"
#include "treap.h"
#include "segtree.h"
#include "topk_heap.h"

class RankingEngine {
public:
    explicit RankingEngine(int maxScore = 100000) : seg_(maxScore) {}

    // add a new user, or update their score if id already exists. O(log n)
    void addOrUpdate(const User& u) {
        check(u.score);
        auto it = users_.find(u.id);
        if (it == users_.end()) {
            users_[u.id] = u;
            treap_.insert(u.score, u.id);
            seg_.add(u.score, +1);
        } else {
            int old = it->second.score;
            treap_.updateScore(old, u.score, u.id);
            seg_.add(old, -1);
            seg_.add(u.score, +1);
            it->second.score = u.score;
            it->second.name = u.name;
        }
    }

    bool remove(int id) {
        auto it = users_.find(id);
        if (it == users_.end()) return false;
        treap_.erase(it->second.score, id);
        seg_.add(it->second.score, -1);
        users_.erase(it);
        return true;
    }

    // "what's my rank?" 1 = best, -1 if unknown. O(log n)
    int rank(int id) const {
        auto it = users_.find(id);
        return it == users_.end() ? -1 : treap_.rank(it->second.score, id);
    }

    // top k users, best first. O(n log k) via binary heap
    std::vector<User> topK(int k) const {
        TopKHeap h(k);
        for (const auto& kv : users_) h.offer(kv.second.score, kv.first);
        std::vector<User> out;
        for (auto& e : h.extractSorted()) out.push_back(users_.at(e.second));
        return out;
    }

    // same answer in O(k log n) using treap order statistics (for timing demo)
    std::vector<User> topKTreap(int k) const {
        std::vector<User> out;
        for (int i = 1; i <= k && i <= treap_.size(); ++i)
            out.push_back(users_.at(treap_.kth(i).second));
        return out;
    }

    int countInRange(int lo, int hi) const { return seg_.countRange(lo, hi); }
    int maxInRange(int lo, int hi) const { return seg_.maxInRange(lo, hi); }

    const User* find(int id) const {
        auto it = users_.find(id);
        return it == users_.end() ? nullptr : &it->second;
    }
    int size() const { return (int)users_.size(); }

    // so Member 3 can set team field without touching the structures
    bool setTeam(int id, int team) {
        auto it = users_.find(id);
        if (it == users_.end()) return false;
        it->second.team = team;
        return true;
    }

private:
    Treap treap_;
    ScoreSegTree seg_;
    std::unordered_map<int, User> users_;

    void check(int s) const {
        if (s < 0 || s > seg_.maxScore()) throw std::out_of_range("score out of range");
    }
};
