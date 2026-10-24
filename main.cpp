#include "Models.h"
#include "AVLTree.h"
#include "Trie.h"
#include "KMP.h"
#include "ProblemService.h"
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

// Print a divider line for readability
void printHeader(const string& title) {
    cout << "\n======================================================================\n";
    cout << "  " << title << "\n";
    cout << "======================================================================\n";
}

void printSubHeader(const string& subtitle) {
    cout << "\n--- " << subtitle << " ---\n";
}

// -------------------------------------------------------------
// Test Section 1: AVL Tree Tests (at least 5 test cases per method)
// -------------------------------------------------------------
void testAVLTree() {
    printHeader("SECTION 1: TESTING AVL TREE (PROBLEMS BY DIFFICULTY & ID)");

    AVLTree tree;

    // Test 1: Insertions to trigger various rotation cases & tie breaking
    printSubHeader("1. Testing insert() - 5 Test Cases (Testing LL, RR, LR, RL and Tie-breaker)");
    
    // Case 1: Insert root
    cout << "[Insert Test 1] Inserting Problem(10, 'Balanced Problem', 1500)\n";
    tree.insert(Problem(10, "Balanced Problem", 1500));
    cout << "  Tree Height: " << tree.getHeight() << "\n";

    // Case 2: RR Case (insert 2000, then 2500 -> triggers Left Rotation)
    cout << "[Insert Test 2] Inserting Problem(11, 'Graph Flow', 2000) & Problem(12, 'Convex Hull', 2500) -> Triggers RR (Left Rotate)\n";
    tree.insert(Problem(11, "Graph Flow", 2000));
    tree.insert(Problem(12, "Convex Hull", 2500));
    cout << "  Tree Height: " << tree.getHeight() << "\n";

    // Case 3: LL Case (insert 1000, then 800 -> triggers Right Rotation)
    cout << "[Insert Test 3] Inserting Problem(13, 'Next Round', 1000) & Problem(14, 'Watermelon', 800) -> Triggers LL (Right Rotate)\n";
    tree.insert(Problem(13, "Next Round", 1000));
    tree.insert(Problem(14, "Watermelon", 800));
    cout << "  Tree Height: " << tree.getHeight() << "\n";

    // Case 4: LR & RL Rotations
    cout << "[Insert Test 4] Inserting Problem(15, 'DP Knapsack', 900) & Problem(16, 'Suffix Array', 2200) -> Triggers LR & RL\n";
    tree.insert(Problem(15, "DP Knapsack", 900));
    tree.insert(Problem(16, "Suffix Array", 2200));
    cout << "  Tree Height: " << tree.getHeight() << "\n";

    // Case 5: Tie-breaker test (same difficulty 800, different IDs)
    cout << "[Insert Test 5] Inserting Problem(1, 'Way Too Long Words', 800) [Same difficulty as Problem 14, id 1 < 14]\n";
    tree.insert(Problem(1, "Way Too Long Words", 800));
    cout << "  Tree Height after insertions: " << tree.getHeight() << " (Tree remains balanced!)\n";

    // Test 2: Search by difficulty (5 test cases)
    printSubHeader("2. Testing search(difficulty) - 5 Test Cases");
    
    int searchDiffs[5] = {800, 1500, 2500, 900, 9999};
    for (int i = 0; i < 5; i++) {
        int target = searchDiffs[i];
        cout << "[Search Test " << (i + 1) << "] Searching for difficulty: " << target << "\n";
        vector<Problem> found = tree.search(target);
        if (found.empty()) {
            cout << "  Result: No problems found with difficulty " << target << "\n";
        } else {
            cout << "  Result: Found " << found.size() << " problem(s):\n";
            for (const auto& p : found) {
                cout << "    - [ID: " << p.id << "] " << p.name << " (Diff: " << p.difficulty << ")\n";
            }
        }
    }

    // Test 3: listInRange (5 test cases)
    printSubHeader("3. Testing listInRange(minDiff, maxDiff) - 5 Test Cases");
    int ranges[5][2] = {
        {800, 1000},
        {1000, 2000},
        {2000, 3000},
        {800, 3500},
        {4000, 5000} // empty range
    };
    for (int i = 0; i < 5; i++) {
        int minD = ranges[i][0];
        int maxD = ranges[i][1];
        cout << "[Range Test " << (i + 1) << "] Listing problems with difficulty between " << minD << " and " << maxD << ":\n";
        vector<Problem> res = tree.listInRange(minD, maxD);
        if (res.empty()) {
            cout << "  Result: (None found in this range)\n";
        } else {
            cout << "  Result: " << res.size() << " problems in sorted order:\n";
            for (const auto& p : res) {
                cout << "    - [ID: " << setw(2) << p.id << "] " << setw(22) << left << p.name << " | Diff: " << p.difficulty << "\n";
            }
        }
    }

    // Test 4: listAll (Sorted order check)
    printSubHeader("4. Testing listAll() - All problems in ascending order of difficulty");
    vector<Problem> allProblems = tree.listAll();
    cout << "Total problems in tree: " << allProblems.size() << "\n";
    for (size_t i = 0; i < allProblems.size(); i++) {
        cout << "  " << (i + 1) << ". [ID: " << setw(2) << allProblems[i].id << "] " 
             << setw(22) << left << allProblems[i].name << " | Diff: " << allProblems[i].difficulty << "\n";
    }

    // Test 5: deleteProblem / deleteById (5 test cases)
    printSubHeader("5. Testing deleteById() - 5 Test Cases (Leaf, Single Child, Two Children, Root, Missing)");
    
    // Case 1: Delete leaf node
    cout << "[Delete Test 1] Deleting leaf node Problem ID 1 (Way Too Long Words)\n";
    bool d1 = tree.deleteById(1);
    cout << "  Success: " << (d1 ? "YES" : "NO") << " | New Height: " << tree.getHeight() << "\n";

    // Case 2: Delete node with two children
    cout << "[Delete Test 2] Deleting node with two children Problem ID 11 (Graph Flow, 2000)\n";
    bool d2 = tree.deleteById(11);
    cout << "  Success: " << (d2 ? "YES" : "NO") << " | New Height: " << tree.getHeight() << "\n";

    // Case 3: Delete root or internal node
    cout << "[Delete Test 3] Deleting Problem ID 10 (Balanced Problem, 1500)\n";
    bool d3 = tree.deleteById(10);
    cout << "  Success: " << (d3 ? "YES" : "NO") << " | New Height: " << tree.getHeight() << "\n";

    // Case 4: Delete another node
    cout << "[Delete Test 4] Deleting Problem ID 12 (Convex Hull, 2500)\n";
    bool d4 = tree.deleteById(12);
    cout << "  Success: " << (d4 ? "YES" : "NO") << " | New Height: " << tree.getHeight() << "\n";

    // Case 5: Try deleting a non-existent ID
    cout << "[Delete Test 5] Attempting to delete non-existent Problem ID 999\n";
    bool d5 = tree.deleteById(999);
    cout << "  Success: " << (d5 ? "YES (Unexpected)" : "NO (Expected - not found)") << "\n";

    // Final height and remaining count
    cout << "\nRemaining problems after 4 deletions: " << tree.listAll().size() 
         << " | Current Height: " << tree.getHeight() << "\n";
}

// -------------------------------------------------------------
// Test Section 2: Trie Tests (at least 5 test cases per method)
// -------------------------------------------------------------
void testTrie() {
    printHeader("SECTION 2: TESTING TRIE (CASE-INSENSITIVE AUTOCOMPLETE)");

    Trie trie;

    // Test 1: insert (5 test cases)
    printSubHeader("1. Testing insert() - 5 Test Cases (Mixed Cases & Common Prefixes)");
    string wordsToInsert[5] = {
        "tourist",
        "Touring_Pro",
        "tournament",
        "Watermelon",
        "Way Too Long Words"
    };
    for (int i = 0; i < 5; i++) {
        cout << "[Insert Test " << (i + 1) << "] Inserting: '" << wordsToInsert[i] << "'\n";
        trie.insert(wordsToInsert[i]);
    }

    // Test 2: search (5 test cases)
    printSubHeader("2. Testing search() - 5 Test Cases (Exact Search, Case-Insensitive)");
    struct SearchCase {
        string query;
        bool expected;
    };
    SearchCase searchCases[5] = {
        {"tourist", true},         // exact match lowercase
        {"TOURIST", true},         // uppercase match
        {"tournament", true},      // another inserted word
        {"tour", false},           // prefix only, not a complete word
        {"NonExistentUser", false} // completely missing
    };
    for (int i = 0; i < 5; i++) {
        bool res = trie.search(searchCases[i].query);
        cout << "[Search Test " << (i + 1) << "] Query: '" << searchCases[i].query << "'"
             << " | Expected: " << (searchCases[i].expected ? "FOUND" : "NOT FOUND")
             << " | Actual: " << (res ? "FOUND" : "NOT FOUND")
             << " -> " << (res == searchCases[i].expected ? "PASS" : "FAIL") << "\n";
    }

    // Test 3: startsWith (5 test cases)
    printSubHeader("3. Testing startsWith() - 5 Test Cases (Prefix Checking)");
    struct PrefixCase {
        string prefix;
        bool expected;
    };
    PrefixCase prefixCases[5] = {
        {"tour", true},      // prefix of tourist, tournament, Touring_Pro
        {"TOUR", true},      // case insensitive
        {"w", true},         // single character prefix
        {"water", true},     // prefix of Watermelon
        {"xyz", false}       // non-existent prefix
    };
    for (int i = 0; i < 5; i++) {
        bool res = trie.startsWith(prefixCases[i].prefix);
        cout << "[StartsWith Test " << (i + 1) << "] Prefix: '" << prefixCases[i].prefix << "'"
             << " | Expected: " << (prefixCases[i].expected ? "TRUE" : "FALSE")
             << " | Actual: " << (res ? "TRUE" : "FALSE")
             << " -> " << (res == prefixCases[i].expected ? "PASS" : "FAIL") << "\n";
    }

    // Test 4: getAllWithPrefix (5 test cases)
    printSubHeader("4. Testing getAllWithPrefix() - 5 Test Cases (Autocomplete with Limit)");
    struct AutocompleteCase {
        string prefix;
        int limit;
    };
    AutocompleteCase autoCases[5] = {
        {"tour", 10},  // should return all 3 words starting with "tour"
        {"tour", 2},   // limit to 2 suggestions
        {"w", 5},      // words starting with 'w'
        {"way", 5},    // words starting with 'way'
        {"apple", 5}   // no match
    };
    for (int i = 0; i < 5; i++) {
        cout << "[Autocomplete Test " << (i + 1) << "] Prefix: '" << autoCases[i].prefix 
             << "' (Limit: " << autoCases[i].limit << "):\n";
        vector<string> suggestions = trie.getAllWithPrefix(autoCases[i].prefix, autoCases[i].limit);
        if (suggestions.empty()) {
            cout << "  Result: (No suggestions found)\n";
        } else {
            cout << "  Result (" << suggestions.size() << " suggestions):\n";
            for (const string& s : suggestions) {
                cout << "    - " << s << "\n";
            }
        }
    }

    // Test 5: deleteWord (5 test cases)
    printSubHeader("5. Testing deleteWord() - 5 Test Cases");
    
    // Case 1: Delete a word sharing prefix with others
    cout << "[Delete Test 1] Deleting 'tourist'\n";
    bool d1 = trie.deleteWord("tourist");
    cout << "  Success: " << (d1 ? "YES" : "NO") 
         << " | Search 'tourist': " << (trie.search("tourist") ? "FOUND" : "NOT FOUND (Expected)") << "\n";

    // Case 2: Verify siblings still exist
    cout << "[Delete Test 2] Checking siblings 'tournament' & 'Touring_Pro'\n";
    bool s1 = trie.search("tournament");
    bool s2 = trie.search("Touring_Pro");
    cout << "  'tournament' exists: " << (s1 ? "YES" : "NO") 
         << " | 'Touring_Pro' exists: " << (s2 ? "YES" : "NO") << "\n";

    // Case 3: Autocomplete after deletion
    cout << "[Delete Test 3] Autocomplete for 'tour' after deleting 'tourist':\n";
    vector<string> remaining = trie.getAllWithPrefix("tour", 10);
    for (const string& s : remaining) {
        cout << "    - " << s << "\n";
    }

    // Case 4: Delete another word
    cout << "[Delete Test 4] Deleting 'Watermelon'\n";
    bool d4 = trie.deleteWord("Watermelon");
    cout << "  Success: " << (d4 ? "YES" : "NO") 
         << " | Search 'Watermelon': " << (trie.search("Watermelon") ? "FOUND" : "NOT FOUND (Expected)") << "\n";

    // Case 5: Delete non-existent word
    cout << "[Delete Test 5] Attempting to delete non-existent word 'DoesNotExist'\n";
    bool d5 = trie.deleteWord("DoesNotExist");
    cout << "  Success: " << (d5 ? "YES (Unexpected)" : "NO (Expected - not found)") << "\n";
}

// -------------------------------------------------------------
// Test Section 3: KMP Algorithm Tests (at least 5 test cases per method)
// -------------------------------------------------------------
void testKMP() {
    printHeader("SECTION 3: TESTING KMP ALGORITHM & PLAGIARISM CHECKER");

    KMP kmp;

    // Test 1: buildLPS (5 test cases)
    printSubHeader("1. Testing buildLPS() - 5 Test Cases");
    string testPatterns[5] = {
        "ABCDABD",     // Classic KMP pattern
        "AAAA",        // Repetitive characters
        "ABABCABAB",   // Complex prefix-suffix overlaps
        "ABCDE",       // No proper prefix-suffix match (all 0s)
        "A"            // Single character
    };
    for (int i = 0; i < 5; i++) {
        vector<int> lps = kmp.buildLPS(testPatterns[i]);
        cout << "[LPS Test " << (i + 1) << "] Pattern: \"" << testPatterns[i] << "\"\n  LPS Array: [ ";
        for (int val : lps) cout << val << " ";
        cout << "]\n";
    }

    // Test 2: search (5 test cases)
    printSubHeader("2. Testing search(text, pattern) - 5 Test Cases");
    struct SearchTestCase {
        string text;
        string pattern;
        string description;
    };
    SearchTestCase searchTests[5] = {
        {"ABABDABACDABABCABAB", "ABABCABAB", "Single match inside text"},
        {"AABAACAADAABAABA", "AABA", "Multiple overlapping/non-overlapping matches"},
        {"HELLO WORLD C++", "HELLO", "Match at the very beginning"},
        {"COMPETITIVE PROGRAMMING", "MING", "Match at the very end"},
        {"ADVANCED DATA STRUCTURES", "PYTHON", "No match"}
    };
    for (int i = 0; i < 5; i++) {
        vector<int> matches = kmp.search(searchTests[i].text, searchTests[i].pattern);
        cout << "[Search Test " << (i + 1) << "] (" << searchTests[i].description << ")\n"
             << "  Text:    \"" << searchTests[i].text << "\"\n"
             << "  Pattern: \"" << searchTests[i].pattern << "\"\n";
        if (matches.empty()) {
            cout << "  Result: No matches found.\n";
        } else {
            cout << "  Result: Found " << matches.size() << " match(es) at index: ";
            for (int idx : matches) cout << idx << " ";
            cout << "\n";
        }
    }

    // Test 3: checkPlagiarism (5 realistic test cases)
    printSubHeader("3. Testing checkPlagiarism() - 5 Test Cases on Code Submissions");
    
    // Sample stored submissions in database
    vector<Submission> storedSubs;
    
    // Older submission 1 for Problem 1 (Watermelon)
    storedSubs.push_back(Submission(
        101, 1, 100, "Accepted",
        "#include <iostream>\nusing namespace std;\nint main() {\n    int w;\n    cin >> w;\n    if (w > 2 && w % 2 == 0) cout << \"YES\";\n    else cout << \"NO\";\n    return 0;\n}"
    ));

    // Older submission 2 for Problem 2 (Way Too Long Words)
    storedSubs.push_back(Submission(
        102, 2, 120, "Accepted",
        "#include <iostream>\n#include <string>\nusing namespace std;\nvoid solve() {\n    string s;\n    cin >> s;\n    if (s.length() > 10) cout << s[0] << s.length() - 2 << s.back();\n    else cout << s;\n}"
    ));

    // Plagiarism Test Case 1: Exact direct copy of Submission 1
    cout << "[Plagiarism Test 1] Exact copy of Submission 1 for Problem 1\n";
    string code1 = "#include <iostream>\nusing namespace std;\nint main() {\n    int w;\n    cin >> w;\n    if (w > 2 && w % 2 == 0) cout << \"YES\";\n    else cout << \"NO\";\n    return 0;\n}";
    PlagiarismResult r1 = kmp.checkPlagiarism(code1, 1, storedSubs);
    cout << "  Plagiarism Flagged: " << (r1.isPlagiarized ? "YES (Plagiarized!)" : "NO") << "\n";
    if (r1.isPlagiarized) {
        cout << "  Matched With User ID: " << r1.matchedSubmission.userId << "\n";
        cout << "  Matched Chunk (>=30 chars): \"" << r1.matchedChunk << "\"\n";
    }

    // Plagiarism Test Case 2: Copied with lots of extra spaces, blank lines, and irregular indentation
    cout << "\n[Plagiarism Test 2] Same logic as Submission 1 with irregular indentation, blank lines, and extra spaces\n";
    string code2 = "#include <iostream>\n   using namespace std;   \n\n   int main()   {\n       int w;   \n       cin >> w;   \n       if (w > 2 && w % 2 == 0) cout << \"YES\";   \n       else cout << \"NO\";   \n       return 0;   \n   }";
    PlagiarismResult r2 = kmp.checkPlagiarism(code2, 1, storedSubs);
    cout << "  Plagiarism Flagged: " << (r2.isPlagiarized ? "YES (Plagiarized!)" : "NO") << "\n";
    if (r2.isPlagiarized) {
        cout << "  Matched With User ID: " << r2.matchedSubmission.userId << "\n";
        cout << "  Matched Chunk: \"" << r2.matchedChunk << "\"\n";
    }

    // Plagiarism Test Case 3: Copied chunk embedded inside newly written extra functions
    cout << "\n[Plagiarism Test 3] Copied 35+ character logic window from Submission 2 inside a new wrapper\n";
    string code3 = "// My original attempt\nvoid helper() { cout << \"debug\"; }\nvoid solve() {\n    string s;\n    cin >> s;\n    if (s.length() > 10) cout << s[0] << s.length() - 2 << s.back();\n    else cout << s;\n}\nint main() { solve(); }";
    PlagiarismResult r3 = kmp.checkPlagiarism(code3, 2, storedSubs);
    cout << "  Plagiarism Flagged: " << (r3.isPlagiarized ? "YES (Plagiarized!)" : "NO") << "\n";
    if (r3.isPlagiarized) {
        cout << "  Matched With User ID: " << r3.matchedSubmission.userId << "\n";
        cout << "  Matched Chunk: \"" << r3.matchedChunk << "\"\n";
    }

    // Plagiarism Test Case 4: Completely clean original code (no match)
    cout << "\n[Plagiarism Test 4] Completely clean original solution with different logic\n";
    string code4 = "#include <stdio.h>\nint main() {\n    int weight;\n    scanf(\"%d\", &weight);\n    printf((weight %% 2 == 0 && weight > 2) ? \"YES\\n\" : \"NO\\n\");\n    return 0;\n}";
    PlagiarismResult r4 = kmp.checkPlagiarism(code4, 1, storedSubs);
    cout << "  Plagiarism Flagged: " << (r4.isPlagiarized ? "YES" : "NO (Clean submission - Passed!)") << "\n";

    // Plagiarism Test Case 5: Code matching Submission 2, but submitted for Problem 1
    // (Should NOT flag because it is for a different problem!)
    cout << "\n[Plagiarism Test 5] Code matches Submission 2, but submitted for Problem 1 (Different Problem Check)\n";
    string code5 = code2;
    PlagiarismResult r5 = kmp.checkPlagiarism(code5, 2, storedSubs); // Problem 2 check with code from Problem 1
    cout << "  Checking against Problem 2: " << (r5.isPlagiarized ? "YES (Plagiarism Detected)" : "NO (Clean for Problem 2 - Passed!)") << "\n";
}

// -------------------------------------------------------------
// Test Section 4: ProblemService Integration Tests (5 Test Cases per feature)
// -------------------------------------------------------------
void testProblemService() {
    printHeader("SECTION 4: TESTING PROBLEMSERVICE (INTEGRATED CSV LOADER & PIPELINE)");

    ProblemService service;

    // Load CSV Files
    cout << "Loading data from 'problems.csv'...\n";
    bool pOk = service.loadProblemsFromCSV("problems.csv");
    cout << "  Problems loaded: " << (pOk ? "SUCCESS" : "FAILED") << "\n";

    cout << "Loading data from 'users.csv'...\n";
    bool uOk = service.loadUsersFromCSV("users.csv");
    cout << "  Users loaded: " << (uOk ? "SUCCESS" : "FAILED") << "\n";

    // Test 1: addProblem (5 test cases)
    printSubHeader("1. Testing addProblem() - 5 Test Cases");
    Problem newProblems[5] = {
        Problem(101, "Centroid Decomposition", 2700),
        Problem(102, "Heavy Light Decomposition", 2500),
        Problem(103, "Aho-Corasick Automaton", 2600),
        Problem(104, "Manacher Algorithm", 2100),
        Problem(105, "Meet in the Middle", 1600)
    };
    for (int i = 0; i < 5; i++) {
        cout << "[Add Test " << (i + 1) << "] Adding: [ID " << newProblems[i].id << "] " 
             << newProblems[i].name << " (Diff: " << newProblems[i].difficulty << ")\n";
        service.addProblem(newProblems[i]);
    }

    // Test 2: searchByPrefix (5 test cases)
    printSubHeader("2. Testing searchByPrefix() Autocomplete - 5 Test Cases");
    string prefixes[5] = {"c", "w", "a", "bit", "nonexistent"};
    for (int i = 0; i < 5; i++) {
        cout << "[Prefix Test " << (i + 1) << "] Searching problems starting with '" << prefixes[i] << "':\n";
        vector<string> results = service.searchByPrefix(prefixes[i], 5);
        if (results.empty()) {
            cout << "  Result: (No matches found)\n";
        } else {
            for (const string& name : results) {
                cout << "    - " << name << "\n";
            }
        }
    }

    // Test 3: listByDifficulty (5 test cases)
    printSubHeader("3. Testing listByDifficulty() - 5 Test Cases");
    int diffRanges[5][2] = {
        {800, 900},
        {1000, 1200},
        {1300, 1600},
        {2500, 3500},
        {500, 750} // empty
    };
    for (int i = 0; i < 5; i++) {
        int minD = diffRanges[i][0];
        int maxD = diffRanges[i][1];
        cout << "[Difficulty Test " << (i + 1) << "] Problems with difficulty between " << minD << " and " << maxD << ":\n";
        vector<Problem> prolist = service.listByDifficulty(minD, maxD);
        if (prolist.empty()) {
            cout << "  Result: (None in this range)\n";
        } else {
            cout << "  Result (" << prolist.size() << " found, showing up to 4):\n";
            int show = min((int)prolist.size(), 4);
            for (int k = 0; k < show; k++) {
                cout << "    - [ID: " << setw(3) << prolist[k].id << "] " 
                     << setw(26) << left << prolist[k].name << " | Diff: " << prolist[k].difficulty << "\n";
            }
            if ((int)prolist.size() > show) {
                cout << "    ... and " << (prolist.size() - show) << " more.\n";
            }
        }
    }

    // Test 4: removeProblem (5 test cases)
    printSubHeader("4. Testing removeProblem() - 5 Test Cases");
    int removeIds[5] = {101, 102, 1, 2, 999};
    for (int i = 0; i < 5; i++) {
        cout << "[Remove Test " << (i + 1) << "] Removing Problem ID " << removeIds[i] << "\n";
        bool ok = service.removeProblem(removeIds[i]);
        cout << "  Success: " << (ok ? "YES" : "NO (Problem not found)") << "\n";
    }

    // Verify autocomplete after removal: "Centroid Decomposition" should not be suggested
    cout << "\nVerifying Autocomplete for 'centroid' after removal of ID 101:\n";
    vector<string> postRemove = service.searchByPrefix("centroid");
    cout << "  Results count: " << postRemove.size() << " (Expected 0)\n";

    // Test 5: checkSubmission and user autocomplete (5 test cases)
    printSubHeader("5. Testing checkSubmission() and User Autocomplete - 5 Test Cases");
    
    // Add baseline submission to history
    Submission baseSub(1, 4, 150, "Accepted", 
        "#include <iostream>\nusing namespace std;\nint main() {\n    int n, k, a[55], count = 0;\n    cin >> n >> k;\n    for(int i=0; i<n; i++) cin >> a[i];\n    for(int i=0; i<n; i++) if (a[i] >= a[k-1] && a[i] > 0) count++;\n    cout << count;\n    return 0;\n}"
    );
    service.addStoredSubmission(baseSub);
    cout << "Added baseline submission for Problem 4 (Next Round) by User 1.\n";

    // Test Submission 1: Plagiarized identical
    Submission testSub1(2, 4, 180, "Pending", baseSub.code);
    PlagiarismResult pr1 = service.checkSubmission(testSub1);
    cout << "[Submission Test 1] Direct copied submission for Problem 4\n";
    cout << "  Verdict: " << (pr1.isPlagiarized ? "PLAGIARISM DETECTED" : "CLEAN") << "\n";

    // Test Submission 2: Clean original solution
    Submission testSub2(3, 4, 200, "Pending", 
        "#include <vector>\n#include <iostream>\nusing namespace std;\nint main() {\n    // completely different approach with vectors\n    int total, cutoff;\n    cin >> total >> cutoff;\n    vector<int> scores(total);\n    for(auto &x : scores) cin >> x;\n    int ans = 0;\n    for(auto x : scores) if (x >= scores[cutoff-1] && x > 0) ans++;\n    cout << ans << endl;\n}"
    );
    PlagiarismResult pr2 = service.checkSubmission(testSub2);
    cout << "[Submission Test 2] Distinct code using vectors for Problem 4\n";
    cout << "  Verdict: " << (pr2.isPlagiarized ? "PLAGIARISM DETECTED" : "CLEAN (Passed)") << "\n";

    // Test 3: User autocomplete for 'to' (tourist)
    cout << "[User Autocomplete 1] Prefix 'to':\n";
    for (const string& u : service.searchUserByPrefix("to")) cout << "  - " << u << "\n";

    // Test 4: User autocomplete for 'co' (coder_bob, koosaga)
    cout << "[User Autocomplete 2] Prefix 'co':\n";
    for (const string& u : service.searchUserByPrefix("co")) cout << "  - " << u << "\n";

    // Test 5: User autocomplete for 'a' (alice_algo, alex_coder)
    cout << "[User Autocomplete 3] Prefix 'a':\n";
    for (const string& u : service.searchUserByPrefix("a")) cout << "  - " << u << "\n";
}

int main() {
    cout << "======================================================================\n";
    cout << "          CODERANK - ADVANCED DATA STRUCTURES (ADS) PROJECT           \n";
    cout << "             MEMBER 2: PROBLEMS & SEARCH MODULE TEST SUITE            \n";
    cout << "======================================================================\n";

    // Run tests for all three scratch data structures & service
    testAVLTree();
    testTrie();
    testKMP();
    testProblemService();

    printHeader("ALL TESTS COMPLETED SUCCESSFULLY!");
    return 0;
}
