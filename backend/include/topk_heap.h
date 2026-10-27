// topk_heap.h - hand-written binary min-heap used to find top-k users.
// Keep a min-heap of size k: root = worst of the current best k.
// For each user: if better than root, replace root and sift down.
// Time O(n log k), memory O(k).
#pragma once
#include <vector>
#include <algorithm>
#include <utility>

class TopKHeap {
public:
    using Entry = std::pair<int,int>;   // (score, id)

    explicit TopKHeap(int k) : k_(k) {}

    // "a is worse than b": lower score, or same score and larger id
    static bool worse(const Entry& a, const Entry& b) {
        return a.first < b.first || (a.first == b.first && a.second > b.second);
    }

    void offer(int score, int id) {
        Entry e{score, id};
        if (k_ <= 0) return;
        if ((int)h_.size() < k_) { h_.push_back(e); siftUp((int)h_.size() - 1); }
        else if (worse(h_[0], e)) { h_[0] = e; siftDown(0); }
    }

    // result sorted best-first (destroys the heap)
    std::vector<Entry> extractSorted() {
        std::vector<Entry> out;
        while (!h_.empty()) {
            out.push_back(h_[0]);
            h_[0] = h_.back(); h_.pop_back();
            if (!h_.empty()) siftDown(0);
        }
        std::reverse(out.begin(), out.end());   // popped worst-first
        return out;
    }

private:
    int k_;
    std::vector<Entry> h_;

    void siftUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (worse(h_[i], h_[p])) { std::swap(h_[i], h_[p]); i = p; }
            else break;
        }
    }
    void siftDown(int i) {
        int n = (int)h_.size();
        while (true) {
            int l = 2 * i + 1, r = l + 1, w = i;
            if (l < n && worse(h_[l], h_[w])) w = l;
            if (r < n && worse(h_[r], h_[w])) w = r;
            if (w == i) break;
            std::swap(h_[i], h_[w]); i = w;
        }
    }
};
