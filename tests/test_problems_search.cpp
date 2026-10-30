// test_problems_search.cpp - Member 2 (Problems & Search) Test Suite
// Advanced Data Structures - CodeRank
#include "Models.h"
#include "AVLTree.h"
#include "Trie.h"
#include "KMP.h"
#include "ProblemService.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>

using namespace std;

void testAVL() {
    cout << "\n--- TESTING AVL TREE ---\n";
    AVLTree tree;
    tree.insert(Problem(1, "Watermelon", 800));
    tree.insert(Problem(2, "Way Too Long Words", 800));
    tree.insert(Problem(3, "Theatre Square", 1000));
    tree.insert(Problem(16, "Registration System", 1300));
    tree.insert(Problem(29, "Dynamic Shortest Path", 2800));

    assert(!tree.isEmpty());
    vector<Problem> range = tree.listInRange(800, 1000);
    assert(range.size() == 3);
    cout << "AVL Tree range queries: PASSED\n";

    assert(tree.deleteById(1) == true);
    assert(tree.deleteById(999) == false);
    cout << "AVL Tree deletions: PASSED\n";
}

void testTrie() {
    cout << "\n--- TESTING TRIE AUTOCOMPLETE ---\n";
    Trie trie;
    trie.insert("tourist");
    trie.insert("Touring_Pro");
    trie.insert("tournament");
    trie.insert("Watermelon");

    assert(trie.search("tourist") == true);
    assert(trie.search("TOURIST") == true); // Case-insensitive
    assert(trie.search("tour") == false);   // Prefix only
    assert(trie.startsWith("tour") == true);

    vector<string> suggestions = trie.getAllWithPrefix("tour", 5);
    assert(suggestions.size() == 3);
    cout << "Trie autocomplete: PASSED\n";
}

void testKMP() {
    cout << "\n--- TESTING KMP PLAGIARISM CHECKER ---\n";
    KMP kmp;
    vector<int> lps = kmp.buildLPS("ABCDABD");
    assert(lps[5] == 2 && lps.back() == 0);

    vector<int> matches = kmp.search("ABABDABACDABABCABAB", "ABABCABAB");
    assert(matches.size() == 1 && matches[0] == 10);

    vector<Submission> history;
    string codeBase = "#include <iostream>\nusing namespace std;\nint main() { int w; cin >> w; cout << w; }";
    history.push_back(Submission(1, 1, 10, "Accepted", codeBase));

    PlagiarismResult r1 = kmp.checkPlagiarism(codeBase, 1, history);
    assert(r1.isPlagiarized == true);

    PlagiarismResult r2 = kmp.checkPlagiarism("// completely different code\nint solve() { return 0; }", 1, history);
    assert(r2.isPlagiarized == false);

    cout << "KMP pattern matching and plagiarism check: PASSED\n";
}

int main() {
    cout << "======================================================================\n";
    cout << "       MEMBER 2 (PROBLEMS & SEARCH) TEST SUITE                        \n";
    cout << "======================================================================\n";

    testAVL();
    testTrie();
    testKMP();

    cout << "\n>> ALL MEMBER 2 TESTS COMPLETED WITH 100% SUCCESS!\n";
    return 0;
}
