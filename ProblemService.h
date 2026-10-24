#ifndef PROBLEMSERVICE_H
#define PROBLEMSERVICE_H

#include "Models.h"
#include "AVLTree.h"
#include "Trie.h"
#include "KMP.h"
#include <vector>
#include <string>
#include <unordered_map>

class ProblemService {
private:
    AVLTree problemTree;          // AVL Tree to organize problems by difficulty
    Trie problemTrie;             // Trie for autocomplete on problem names
    Trie userTrie;                // Trie for autocomplete on usernames
    KMP plagiarismChecker;        // KMP engine for detecting code plagiarism

    std::vector<Submission> storedSubmissions;     // History of past submissions
    std::unordered_map<int, Problem> problemMap;   // Fast lookup from problemId to Problem

public:
    ProblemService();

    void addProblem(const Problem& p);

    bool removeProblem(int problemId);

    std::vector<std::string> searchByPrefix(const std::string& prefix, int maxLimit = 10);

    std::vector<Problem> listByDifficulty(int minDifficulty, int maxDifficulty);

    PlagiarismResult checkSubmission(const Submission& sub);

    void addStoredSubmission(const Submission& sub);

    void addUser(const User& u);

    std::vector<std::string> searchUserByPrefix(const std::string& prefix, int maxLimit = 10);

    std::vector<Problem> listAllProblems();
    bool loadProblemsFromCSV(const std::string& filename);
    bool loadUsersFromCSV(const std::string& filename);

    AVLTree& getTree() { return problemTree; }
    Trie& getProblemTrie() { return problemTrie; }
    Trie& getUserTrie() { return userTrie; }
    KMP& getKMP() { return plagiarismChecker; }
    const std::vector<Submission>& getSubmissions() const { return storedSubmissions; }
};

#endif // PROBLEMSERVICE_H
