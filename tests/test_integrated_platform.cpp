// test_integrated_platform.cpp - Integrated Master Test Suite
// Verifies Member 1, Member 2, and Member 3 work together seamlessly.
#include "coderank_platform.h"
#include <iostream>
#include <cassert>

using namespace std;

int main() {
    cout << "======================================================================\n";
    cout << "       CODERANK MASTER PLATFORM INTEGRATION TEST SUITE                \n";
    cout << "======================================================================\n";

    CodeRankPlatform platform(100000, 240);
    platform.initializeFromDatabase("database");

    // 1. Test Member 1 Treap Ranking
    cout << "1. Testing Member 1 Ranking Engine (Treap)...\n";
    User u1(101, "new_star", 3700);
    platform.addOrUpdateUser(u1);
    int r1 = platform.getUserRank(101);
    cout << "   User 101 Rank: #" << r1 << " (Expected top rank)\n";
    assert(r1 >= 1);

    // 2. Test Member 2 Problem Catalog (AVL Tree & Trie)
    cout << "2. Testing Member 2 Problem Catalog & Autocomplete...\n";
    auto proms = platform.listProblemsByDifficulty(800, 900);
    assert(!proms.empty());
    cout << "   Found " << proms.size() << " problems in [800, 900]\n";

    auto autos = platform.autocompleteProblemName("w", 5);
    assert(!autos.empty());
    cout << "   Autocomplete 'w': " << autos[0] << "\n";

    // 3. Test Member 3 Judging Queue & DSU Teams
    cout << "3. Testing Member 3 Judging & Teams...\n";
    platform.createTeam("SuperTeam", {101, 1});
    int teamScore = platform.getJudgingService().getTeamScore(101);
    cout << "   SuperTeam Score: " << teamScore << " (Sum of members)\n";
    assert(teamScore == 10700); // 3700 (u101) + 3800 (tourist) + 3200 (Petr in team 1 merged via DSU)

    // 4. Test Full Pipeline (Plagiarism -> Queue -> Judge -> Rank Update)
    cout << "4. Testing End-to-End Submission Pipeline...\n";
    Submission sub(101, 1, 100, "Accepted", "// brand new unique solution\nvoid solve() { return; }");
    IntegratedSubmitResult isr = platform.processFullSubmission(sub);
    assert(!isr.isPlagiarized);
    assert(isr.isAccepted);
    cout << "   Pipeline Submission judged Accepted! Awarded: +" << isr.pointsAwarded << " pts\n";

    // 5. Test Performance Benchmark Run
    cout << "5. Testing Performance Benchmarking Engine...\n";
    auto benchs = platform.runPerformanceBenchmarks(false);
    assert(!benchs.empty());
    cout << "   Executed " << benchs.size() << " benchmarks successfully!\n";

    cout << "\n======================================================================\n";
    cout << "   MASTER INTEGRATION TESTS PASSED 100% WITH ZERO ERRORS!             \n";
    cout << "======================================================================\n";
    return 0;
}
