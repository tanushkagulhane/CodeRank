#include "AVLTree.h"

AVLTree::AVLTree() {
    root = nullptr;
}
AVLTree::~AVLTree() {
    destroyTree(root);
}

void AVLTree::destroyTree(AVLNode* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

int AVLTree::getHeightNode(AVLNode* node) {
    if (node == nullptr) return 0;
    return node->height;
}

int AVLTree::maxVal(int a, int b) {
    return (a > b) ? a : b;
}

int AVLTree::getBalance(AVLNode* node) {
    if (node == nullptr) return 0;
    return getHeightNode(node->left) - getHeightNode(node->right);
}

int AVLTree::compare(const Problem& a, const Problem& b) {
    if (a.difficulty < b.difficulty) return -1;
    if (a.difficulty > b.difficulty) return 1;
    if (a.id < b.id) return -1;
    if (a.id > b.id) return 1;
    return 0;
}


AVLNode* AVLTree::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = maxVal(getHeightNode(y->left), getHeightNode(y->right)) + 1;
    x->height = maxVal(getHeightNode(x->left), getHeightNode(x->right)) + 1;

    return x;
}

AVLNode* AVLTree::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = maxVal(getHeightNode(x->left), getHeightNode(x->right)) + 1;
    y->height = maxVal(getHeightNode(y->left), getHeightNode(y->right)) + 1;

    return y;
}

AVLNode* AVLTree::insertNode(AVLNode* node, const Problem& p) {
    if (node == nullptr) {
        return new AVLNode(p);
    }

    int cmp = compare(p, node->problem);
    if (cmp < 0) {
        node->left = insertNode(node->left, p);
    } else if (cmp > 0) {
        node->right = insertNode(node->right, p);
    } else {
        return node;
    }

    node->height = 1 + maxVal(getHeightNode(node->left), getHeightNode(node->right));
    int balance = getBalance(node);

    // Case 1: Left-Left (LL) Case
    if (balance > 1 && compare(p, node->left->problem) < 0) {
        return rightRotate(node);
    }

    // Case 2: Right-Right (RR) Case
    if (balance < -1 && compare(p, node->right->problem) > 0) {
        return leftRotate(node);
    }

    // Case 3: Left-Right (LR) Case
    if (balance > 1 && compare(p, node->left->problem) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Case 4: Right-Left (RL) Case
    if (balance < -1 && compare(p, node->right->problem) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void AVLTree::insert(const Problem& p) {
    root = insertNode(root, p);
}

AVLNode* AVLTree::getMinValueNode(AVLNode* node) {
    AVLNode* curr = node;
    while (curr->left != nullptr) {
        curr = curr->left;
    }
    return curr;
}

AVLNode* AVLTree::deleteNode(AVLNode* node, const Problem& p, bool& deleted) {
    if (node == nullptr) {
        return nullptr;
    }

    int cmp = compare(p, node->problem);
    if (cmp < 0) {
        node->left = deleteNode(node->left, p, deleted);
    } else if (cmp > 0) {
        node->right = deleteNode(node->right, p, deleted);
    } else {
        deleted = true;
        if (node->left == nullptr || node->right == nullptr) {
            AVLNode* temp = node->left ? node->left : node->right;

            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            } else {
                *node = *temp;
            }
            delete temp;
        } else {
            AVLNode* temp = getMinValueNode(node->right);
            node->problem = temp->problem;
            node->right = deleteNode(node->right, temp->problem, deleted);
        }
    }
    if (node == nullptr) {
        return nullptr;
    }

    node->height = 1 + maxVal(getHeightNode(node->left), getHeightNode(node->right));
    int balance = getBalance(node);

    // Case 1: Left-Left (LL) Cas
    if (balance > 1 && getBalance(node->left) >= 0) {
        return rightRotate(node);
    }

    // Case 2: Left-Right (LR) Case
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Case 3: Right-Right (RR) Case
    if (balance < -1 && getBalance(node->right) <= 0) {
        return leftRotate(node);
    }

    // Case 4: Right-Left (RL) Case
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

bool AVLTree::deleteProblem(const Problem& p) {
    bool deleted = false;
    root = deleteNode(root, p, deleted);
    return deleted;
}

Problem* AVLTree::findByIdHelper(AVLNode* node, int problemId) {
    if (node == nullptr) return nullptr;
    if (node->problem.id == problemId) return &(node->problem);
    Problem* leftRes = findByIdHelper(node->left, problemId);
    if (leftRes != nullptr) return leftRes;
    return findByIdHelper(node->right, problemId);
}

bool AVLTree::deleteById(int problemId) {
    Problem* found = findByIdHelper(root, problemId);
    if (found == nullptr) {
        return false;
    }
    Problem target = *found; // save a copy
    bool deleted = false;
    root = deleteNode(root, target, deleted);
    return deleted;
}

void AVLTree::searchHelper(AVLNode* node, int difficulty, std::vector<Problem>& results) {
    if (node == nullptr) return;

    if (difficulty < node->problem.difficulty) {
        searchHelper(node->left, difficulty, results);
    } else if (difficulty > node->problem.difficulty) {
        searchHelper(node->right, difficulty, results);
    } else {
        searchHelper(node->left, difficulty, results);
        results.push_back(node->problem);
        searchHelper(node->right, difficulty, results);
    }
}

std::vector<Problem> AVLTree::search(int difficulty) {
    std::vector<Problem> results;
    searchHelper(root, difficulty, results);
    return results;
}

void AVLTree::rangeHelper(AVLNode* node, int minDiff, int maxDiff, std::vector<Problem>& results) {
    if (node == nullptr) return;

    if (node->problem.difficulty >= minDiff) {
        rangeHelper(node->left, minDiff, maxDiff, results);
    }

    if (node->problem.difficulty >= minDiff && node->problem.difficulty <= maxDiff) {
        results.push_back(node->problem);
    }
    if (node->problem.difficulty <= maxDiff) {
        rangeHelper(node->right, minDiff, maxDiff, results);
    }
}

std::vector<Problem> AVLTree::listInRange(int minDifficulty, int maxDifficulty) {
    std::vector<Problem> results;
    rangeHelper(root, minDifficulty, maxDifficulty, results);
    return results;
}

void AVLTree::inOrderHelper(AVLNode* node, std::vector<Problem>& results) {
    if (node == nullptr) return;
    inOrderHelper(node->left, results);
    results.push_back(node->problem);
    inOrderHelper(node->right, results);
}

std::vector<Problem> AVLTree::listAll() {
    std::vector<Problem> results;
    inOrderHelper(root, results);
    return results;
}

int AVLTree::getHeight() {
    return getHeightNode(root);
}

bool AVLTree::isEmpty() {
    return root == nullptr;
}
