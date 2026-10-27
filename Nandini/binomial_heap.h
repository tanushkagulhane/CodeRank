// binomial_heap.h - Binomial Heap implementation for Submission Queue
// Member 3 (Judging & Teams) - Advanced Data Structures
//
// Supports:
// - insert: O(log n)
// - findMin / peek: O(log n)
// - extractMin: O(log n)
// - merge / unionWith: O(log n)
// Priority configurable by contest time or problem ID.
#pragma once

#include "Models.h"
#include <vector>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <algorithm>

enum class QueuePriorityMode {
    BY_CONTEST_TIME, // Earlier contest time has higher priority (default)
    BY_PROBLEM       // Lower problem ID has higher priority
};

struct BinomialNode {
    Submission data;
    int degree;
    BinomialNode* parent;
    BinomialNode* child;
    BinomialNode* sibling;

    explicit BinomialNode(const Submission& sub)
        : data(sub), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
};

class BinomialHeap {
public:
    explicit BinomialHeap(QueuePriorityMode mode = QueuePriorityMode::BY_CONTEST_TIME)
        : head_(nullptr), size_(0), priorityMode_(mode) {}

    // Destructor cleanly frees all nodes in the heap
    ~BinomialHeap() {
        clear();
    }

    // Copy constructor (deep copy)
    BinomialHeap(const BinomialHeap& other)
        : head_(nullptr), size_(0), priorityMode_(other.priorityMode_) {
        std::vector<Submission> items = other.toVector();
        for (const auto& item : items) {
            insert(item);
        }
    }

    // Copy assignment
    BinomialHeap& operator=(const BinomialHeap& other) {
        if (this != &other) {
            clear();
            priorityMode_ = other.priorityMode_;
            std::vector<Submission> items = other.toVector();
            for (const auto& item : items) {
                insert(item);
            }
        }
        return *this;
    }

    // Move constructor
    BinomialHeap(BinomialHeap&& other) noexcept
        : head_(other.head_), size_(other.size_), priorityMode_(other.priorityMode_) {
        other.head_ = nullptr;
        other.size_ = 0;
    }

    // Move assignment
    BinomialHeap& operator=(BinomialHeap&& other) noexcept {
        if (this != &other) {
            clear();
            head_ = other.head_;
            size_ = other.size_;
            priorityMode_ = other.priorityMode_;
            other.head_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    bool empty() const {
        return head_ == nullptr;
    }

    int size() const {
        return size_;
    }

    QueuePriorityMode getPriorityMode() const {
        return priorityMode_;
    }

    void setPriorityMode(QueuePriorityMode mode) {
        if (mode != priorityMode_) {
            priorityMode_ = mode;
            // Rebuild heap with new priority comparator
            std::vector<Submission> items = toVector();
            clear();
            for (const auto& item : items) {
                insert(item);
            }
        }
    }

    // Returns true if a has higher priority than b (min-heap ordering)
    bool hasHigherPriority(const Submission& a, const Submission& b) const {
        if (priorityMode_ == QueuePriorityMode::BY_CONTEST_TIME) {
            if (a.time != b.time) return a.time < b.time;
            if (a.problemId != b.problemId) return a.problemId < b.problemId;
            return a.id < b.id;
        } else {
            // BY_PROBLEM
            if (a.problemId != b.problemId) return a.problemId < b.problemId;
            if (a.time != b.time) return a.time < b.time;
            return a.id < b.id;
        }
    }

    // Insert a new submission: O(log n)
    void insert(const Submission& sub) {
        BinomialHeap tempHeap(priorityMode_);
        tempHeap.head_ = new BinomialNode(sub);
        tempHeap.size_ = 1;
        unionWith(tempHeap);
    }

    // Peek at submission with highest priority without removing it: O(log n)
    Submission peekMin() const {
        if (empty()) {
            throw std::runtime_error("Cannot peek into an empty Binomial Heap!");
        }
        BinomialNode* minNode = findMinNode();
        return minNode->data;
    }

    // Extract submission with highest priority: O(log n)
    Submission extractMin() {
        if (empty()) {
            throw std::runtime_error("Cannot extract from an empty Binomial Heap!");
        }

        // 1. Find the root with the minimum key
        BinomialNode* minNode = head_;
        BinomialNode* minPrev = nullptr;
        BinomialNode* curr = head_->sibling;
        BinomialNode* prev = head_;

        while (curr != nullptr) {
            if (hasHigherPriority(curr->data, minNode->data)) {
                minNode = curr;
                minPrev = prev;
            }
            prev = curr;
            curr = curr->sibling;
        }

        // 2. Remove minNode from the root list
        if (minPrev == nullptr) {
            head_ = minNode->sibling;
        } else {
            minPrev->sibling = minNode->sibling;
        }

        // 3. Reverse the order of minNode's children to form a valid root list
        BinomialNode* childHead = nullptr;
        BinomialNode* childCurr = minNode->child;
        while (childCurr != nullptr) {
            BinomialNode* nextChild = childCurr->sibling;
            childCurr->sibling = childHead;
            childCurr->parent = nullptr;
            childHead = childCurr;
            childCurr = nextChild;
        }

        // 4. Construct a temporary heap with the children and union
        BinomialHeap tempHeap(priorityMode_);
        tempHeap.head_ = childHead;
        
        // Count nodes in child trees
        int childNodeCount = (1 << minNode->degree) - 1;
        tempHeap.size_ = childNodeCount;
        size_ -= (childNodeCount + 1);

        unionWith(tempHeap);

        Submission result = minNode->data;
        delete minNode;
        return result;
    }

    // Merge another Binomial Heap into this one: O(log n)
    void merge(BinomialHeap& other) {
        unionWith(other);
    }

    // Union operation: combines this heap with other heap in O(log n)
    void unionWith(BinomialHeap& other) {
        if (other.empty()) return;
        if (empty()) {
            head_ = other.head_;
            size_ = other.size_;
            other.head_ = nullptr;
            other.size_ = 0;
            return;
        }

        // Step 1: Merge the root lists in non-decreasing order of degree
        BinomialNode* newHead = mergeRootLists(head_, other.head_);
        head_ = nullptr;
        other.head_ = nullptr;

        if (newHead == nullptr) {
            size_ += other.size_;
            other.size_ = 0;
            return;
        }

        // Step 2: Combine trees of equal degree
        BinomialNode* prev = nullptr;
        BinomialNode* curr = newHead;
        BinomialNode* next = curr->sibling;

        while (next != nullptr) {
            // Case 1: Degrees are different, or 3 consecutive trees have the same degree
            if ((curr->degree != next->degree) ||
                (next->sibling != nullptr && next->sibling->degree == curr->degree)) {
                prev = curr;
                curr = next;
            }
            // Case 2: curr has higher priority -> make next the child of curr
            else if (hasHigherPriority(curr->data, next->data)) {
                curr->sibling = next->sibling;
                linkTrees(next, curr);
            }
            // Case 3: next has higher priority -> make curr the child of next
            else {
                if (prev == nullptr) {
                    newHead = next;
                } else {
                    prev->sibling = next;
                }
                linkTrees(curr, next);
                curr = next;
            }
            next = curr->sibling;
        }

        head_ = newHead;
        size_ += other.size_;
        other.size_ = 0;
    }

    // Clear all nodes in the heap
    void clear() {
        destroyTree(head_);
        head_ = nullptr;
        size_ = 0;
    }

    // Convert heap contents into a vector (without modifying heap)
    std::vector<Submission> toVector() const {
        std::vector<Submission> result;
        collectSubmissions(head_, result);
        return result;
    }

    // Pretty-print binomial tree structures by order/degree
    void printStructure() const {
        std::cout << "--- Binomial Heap (Size: " << size_ << ") ---\n";
        if (empty()) {
            std::cout << "  (Heap is empty)\n";
            return;
        }
        BinomialNode* curr = head_;
        while (curr != nullptr) {
            std::cout << "  [Binomial Tree B_" << curr->degree << "] Root Submission ID " 
                      << curr->data.id << " (User: " << curr->data.userId 
                      << ", Prob: " << curr->data.problemId 
                      << ", Time: " << curr->data.time << "m)\n";
            printSubtree(curr->child, 2);
            curr = curr->sibling;
        }
    }

private:
    BinomialNode* head_;
    int size_;
    QueuePriorityMode priorityMode_;

    // Links childTree as the leftmost child of parentTree (degree increments)
    void linkTrees(BinomialNode* childTree, BinomialNode* parentTree) {
        childTree->parent = parentTree;
        childTree->sibling = parentTree->child;
        parentTree->child = childTree;
        parentTree->degree++;
    }

    // Merges two sorted root lists by degree (like mergesort step)
    BinomialNode* mergeRootLists(BinomialNode* h1, BinomialNode* h2) {
        if (!h1) return h2;
        if (!h2) return h1;

        BinomialNode* head = nullptr;
        BinomialNode* tail = nullptr;

        auto append = [&](BinomialNode* node) {
            if (!head) {
                head = tail = node;
            } else {
                tail->sibling = node;
                tail = node;
            }
        };

        while (h1 && h2) {
            if (h1->degree <= h2->degree) {
                BinomialNode* next = h1->sibling;
                append(h1);
                h1 = next;
            } else {
                BinomialNode* next = h2->sibling;
                append(h2);
                h2 = next;
            }
        }

        if (h1) tail->sibling = h1;
        if (h2) tail->sibling = h2;

        return head;
    }

    BinomialNode* findMinNode() const {
        if (!head_) return nullptr;
        BinomialNode* minNode = head_;
        BinomialNode* curr = head_->sibling;
        while (curr != nullptr) {
            if (hasHigherPriority(curr->data, minNode->data)) {
                minNode = curr;
            }
            curr = curr->sibling;
        }
        return minNode;
    }

    void destroyTree(BinomialNode* node) {
        if (!node) return;
        destroyTree(node->child);
        destroyTree(node->sibling);
        delete node;
    }

    void collectSubmissions(BinomialNode* node, std::vector<Submission>& list) const {
        if (!node) return;
        list.push_back(node->data);
        collectSubmissions(node->child, list);
        collectSubmissions(node->sibling, list);
    }

    void printSubtree(BinomialNode* node, int indent) const {
        if (!node) return;
        std::cout << std::string(indent * 2, ' ') << "|- Sub ID " << node->data.id 
                  << " (User: " << node->data.userId 
                  << ", Prob: " << node->data.problemId 
                  << ", Time: " << node->data.time << "m, Deg: " << node->degree << ")\n";
        printSubtree(node->child, indent + 1);
        printSubtree(node->sibling, indent);
    }
};
