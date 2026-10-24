// segtree.h - Segment tree over scores [0, maxScore]
// Each leaf = a score value. Stores how many users have that score.
// Node keeps: cnt (users in range) and mx (largest score present, -1 if none).
// Point update, range count, range max: all O(log S).
#pragma once
#include <vector>
#include <algorithm>

class ScoreSegTree {
public:
    explicit ScoreSegTree(int maxScore) : n_(maxScore + 1), cnt_(4 * n_, 0), mx_(4 * n_, -1) {}

    // add (delta = +1) or remove (delta = -1) one user having this score
    void add(int score, int delta) { update(1, 0, n_ - 1, score, delta); }

    // number of users with lo <= score <= hi
    int countRange(int lo, int hi) const {
        lo = std::max(lo, 0); hi = std::min(hi, n_ - 1);
        if (lo > hi) return 0;
        return count(1, 0, n_ - 1, lo, hi);
    }

    // highest score present in [lo,hi], or -1 if nobody
    int maxInRange(int lo, int hi) const {
        lo = std::max(lo, 0); hi = std::min(hi, n_ - 1);
        if (lo > hi) return -1;
        return maxq(1, 0, n_ - 1, lo, hi);
    }

    int maxScore() const { return n_ - 1; }

private:
    int n_;
    std::vector<int> cnt_, mx_;

    void update(int node, int l, int r, int pos, int d) {
        if (l == r) {
            cnt_[node] += d;
            mx_[node] = cnt_[node] > 0 ? l : -1;
            return;
        }
        int m = (l + r) / 2;
        if (pos <= m) update(2 * node, l, m, pos, d);
        else          update(2 * node + 1, m + 1, r, pos, d);
        cnt_[node] = cnt_[2 * node] + cnt_[2 * node + 1];
        mx_[node]  = std::max(mx_[2 * node], mx_[2 * node + 1]);
    }
    int count(int node, int l, int r, int ql, int qr) const {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return cnt_[node];
        int m = (l + r) / 2;
        return count(2 * node, l, m, ql, qr) + count(2 * node + 1, m + 1, r, ql, qr);
    }
    int maxq(int node, int l, int r, int ql, int qr) const {
        if (qr < l || r < ql) return -1;
        if (ql <= l && r <= qr) return mx_[node];
        int m = (l + r) / 2;
        return std::max(maxq(2 * node, l, m, ql, qr), maxq(2 * node + 1, m + 1, r, ql, qr));
    }
};
