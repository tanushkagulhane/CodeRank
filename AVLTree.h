#ifndef AVLTREE_H
#define AVLTREE_H

#include "Models.h"
#include <vector>
#include <iostream>

struct AVLNode {
    Problem problem;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(Problem p) {
        problem = p;
        height = 1;
        left = nullptr;
        right = nullptr;
    }
};

class AVLTree {
private:
    AVLNode* root;

    int getHeightNode(AVLNode* node);
    int getBalance(AVLNode* node);
    int maxVal(int a, int b);
    int compare(const Problem& a, const Problem& b);

    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);

    AVLNode* insertNode(AVLNode* node, const Problem& p);
    AVLNode* deleteNode(AVLNode* node, const Problem& p, bool& deleted);
    AVLNode* getMinValueNode(AVLNode* node);
    void destroyTree(AVLNode* node);

    void searchHelper(AVLNode* node, int difficulty, std::vector<Problem>& results);
    void rangeHelper(AVLNode* node, int minDiff, int maxDiff, std::vector<Problem>& results);
    void inOrderHelper(AVLNode* node, std::vector<Problem>& results);
    Problem* findByIdHelper(AVLNode* node, int problemId);

public:
    AVLTree();
    ~AVLTree();

    void insert(const Problem& p);
    bool deleteProblem(const Problem& p);
    bool deleteById(int problemId);
    std::vector<Problem> search(int difficulty);
    std::vector<Problem> listInRange(int minDifficulty, int maxDifficulty);
    std::vector<Problem> listAll();

    int getHeight();
    bool isEmpty();
};

#endif
