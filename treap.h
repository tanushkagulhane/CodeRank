// treap.h - Treap with subtree sizes (order-statistic tree)
// Order: higher score first; ties broken by smaller id first.
// So rank 1 = best user. All operations are O(log n) expected.
#pragma once
#include <random>
#include <utility>

class Treap {
public:
    struct Key { int score, id; };

    Treap() = default;
    Treap(const Treap&) = delete;
    Treap& operator=(const Treap&) = delete;
    ~Treap() { destroy(root_); }

    int size() const { return sz(root_); }

    void insert(int score, int id) {
        Node* n = new Node(score, id, rng_());
        Node *l, *r;
        split(root_, Key{score, id}, l, r);      // l: strictly before key
        root_ = merge(merge(l, n), r);
    }

    // returns false if (score,id) is not present
    bool erase(int score, int id) {
        bool removed = false;
        root_ = erase(root_, Key{score, id}, removed);
        return removed;
    }

    // change score: delete old key, insert new key
    bool updateScore(int oldScore, int newScore, int id) {
        if (!erase(oldScore, id)) return false;
        insert(newScore, id);
        return true;
    }

    // 1-based rank, or -1 if not found
    int rank(int score, int id) const {
        Key k{score, id};
        int res = 0;
        const Node* t = root_;
        while (t) {
            if (t->score == score && t->id == id) return res + sz(t->l) + 1;
            if (before(k, t)) t = t->l;
            else { res += sz(t->l) + 1; t = t->r; }
        }
        return -1;
    }

    // k-th best (1-based) as (score,id); returns {-1,-1} if out of range
    std::pair<int,int> kth(int k) const {
        const Node* t = root_;
        if (k < 1 || k > sz(t)) return {-1, -1};
        while (t) {
            int ls = sz(t->l);
            if (k == ls + 1) return {t->score, t->id};
            if (k <= ls) t = t->l;
            else { k -= ls + 1; t = t->r; }
        }
        return {-1, -1};
    }

private:
    struct Node {
        int score, id, size = 1;
        unsigned pri;
        Node *l = nullptr, *r = nullptr;
        Node(int s, int i, unsigned p) : score(s), id(i), pri(p) {}
    };

    Node* root_ = nullptr;
    std::mt19937 rng_{std::random_device{}()};

    static int sz(const Node* t) { return t ? t->size : 0; }
    static void pull(Node* t) { t->size = 1 + sz(t->l) + sz(t->r); }

    // does key k come strictly before node t in the ordering?
    static bool before(const Key& k, const Node* t) {
        return k.score > t->score || (k.score == t->score && k.id < t->id);
    }
    static bool nodeBefore(const Node* t, const Key& k) {
        return t->score > k.score || (t->score == k.score && t->id < k.id);
    }

    // l = nodes strictly before k, r = the rest
    static void split(Node* t, const Key& k, Node*& l, Node*& r) {
        if (!t) { l = r = nullptr; return; }
        if (nodeBefore(t, k)) { split(t->r, k, t->r, r); pull(t); l = t; }
        else                  { split(t->l, k, l, t->l); pull(t); r = t; }
    }

    static Node* merge(Node* a, Node* b) {
        if (!a) return b;
        if (!b) return a;
        if (a->pri > b->pri) { a->r = merge(a->r, b); pull(a); return a; }
        b->l = merge(a, b->l); pull(b); return b;
    }

    static Node* erase(Node* t, const Key& k, bool& removed) {
        if (!t) return nullptr;
        if (t->score == k.score && t->id == k.id) {
            Node* res = merge(t->l, t->r);
            delete t;
            removed = true;
            return res;
        }
        if (before(k, t)) t->l = erase(t->l, k, removed);
        else              t->r = erase(t->r, k, removed);
        pull(t);
        return t;
    }

    static void destroy(Node* t) {
        if (!t) return;
        destroy(t->l); destroy(t->r); delete t;
    }
};
