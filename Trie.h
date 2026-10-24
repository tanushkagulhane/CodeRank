#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>
#include <cctype>

struct TrieNode {
    TrieNode* children[128];
    bool isEndOfWord;
    std::string originalWord;

    TrieNode() {
        isEndOfWord = false;
        originalWord = "";
        for (int i = 0; i < 128; i++) {
            children[i] = nullptr;
        }
    }
};

class Trie {
private:
    TrieNode* root;

    int getIndex(char c);

    bool isEmptyNode(TrieNode* node);
    bool deleteHelper(TrieNode* curr, const std::string& word, int depth, bool& deleted);
    void collectWords(TrieNode* curr, std::vector<std::string>& results, int maxLimit);
    void destroyTrie(TrieNode* node);

public:
    Trie();
    ~Trie();

    void insert(const std::string& word);
    bool deleteWord(const std::string& word);
    bool search(const std::string& word);

    bool startsWith(const std::string& prefix);

    std::vector<std::string> getAllWithPrefix(const std::string& prefix, int maxLimit = 10);
};

#endif // TRIE_H
