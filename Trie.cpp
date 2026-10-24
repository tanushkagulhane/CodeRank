#include "Trie.h"

Trie::Trie() {
    root = new TrieNode();
}

Trie::~Trie() {
    destroyTrie(root);
}
void Trie::destroyTrie(TrieNode* node) {
    if (node == nullptr) return;
    for (int i = 0; i < 128; i++) {
        if (node->children[i] != nullptr) {
            destroyTrie(node->children[i]);
        }
    }
    delete node;
}

int Trie::getIndex(char c) {
    unsigned char uc = (unsigned char)std::tolower(c);
    if (uc < 128) return uc;
    return -1;
}

void Trie::insert(const std::string& word) {
    if (word.empty()) return;

    TrieNode* curr = root;
    for (char c : word) {
        int idx = getIndex(c);
        if (idx == -1) continue;

        if (curr->children[idx] == nullptr) {
            curr->children[idx] = new TrieNode();
        }
        curr = curr->children[idx];
    }
    curr->isEndOfWord = true;
    curr->originalWord = word;
}


bool Trie::isEmptyNode(TrieNode* node) {
    for (int i = 0; i < 128; i++) {
        if (node->children[i] != nullptr) return false;
    }
    return true;
}

bool Trie::deleteHelper(TrieNode* curr, const std::string& word, int depth, bool& deleted) {
    if (curr == nullptr) return false;

    if (depth == (int)word.length()) {
        if (curr->isEndOfWord) {
            curr->isEndOfWord = false;
            curr->originalWord = "";
            deleted = true;
        }

        return isEmptyNode(curr);
    }

    int idx = getIndex(word[depth]);
    if (idx == -1 || curr->children[idx] == nullptr) {
        return false;
    }

    bool shouldDeleteChild = deleteHelper(curr->children[idx], word, depth + 1, deleted);

    if (shouldDeleteChild) {
        delete curr->children[idx];
        curr->children[idx] = nullptr;

        return (!curr->isEndOfWord && isEmptyNode(curr));
    }

    return false;
}

bool Trie::deleteWord(const std::string& word) {
    if (word.empty()) return false;
    bool deleted = false;
    deleteHelper(root, word, 0, deleted);
    return deleted;
}

bool Trie::search(const std::string& word) {
    if (word.empty()) return false;

    TrieNode* curr = root;
    for (char c : word) {
        int idx = getIndex(c);
        if (idx == -1 || curr->children[idx] == nullptr) {
            return false;
        }
        curr = curr->children[idx];
    }
    return (curr != nullptr && curr->isEndOfWord);
}

bool Trie::startsWith(const std::string& prefix) {
    if (prefix.empty()) return true;

    TrieNode* curr = root;
    for (char c : prefix) {
        int idx = getIndex(c);
        if (idx == -1 || curr->children[idx] == nullptr) {
            return false;
        }
        curr = curr->children[idx];
    }
    return curr != nullptr;
}

void Trie::collectWords(TrieNode* curr, std::vector<std::string>& results, int maxLimit) {
    if (curr == nullptr || (int)results.size() >= maxLimit) {
        return;
    }

    if (curr->isEndOfWord) {
        results.push_back(curr->originalWord);
        if ((int)results.size() >= maxLimit) return;
    }

    for (int i = 0; i < 128; i++) {
        if (curr->children[i] != nullptr) {
            collectWords(curr->children[i], results, maxLimit);
            if ((int)results.size() >= maxLimit) return;
        }
    }
}

std::vector<std::string> Trie::getAllWithPrefix(const std::string& prefix, int maxLimit) {
    std::vector<std::string> results;
    TrieNode* curr = root;

    for (char c : prefix) {
        int idx = getIndex(c);
        if (idx == -1 || curr->children[idx] == nullptr) {
            return results;
        }
        curr = curr->children[idx];
    }

    collectWords(curr, results, maxLimit);
    return results;
}
