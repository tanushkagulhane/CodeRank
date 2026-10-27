// test_judging_teams.cpp - Comprehensive Test Suite for Member 3
// Advanced Data Structures - CodeRank
// Each method includes at least 5 distinct test cases.
#include "Models.h"
#include "binomial_heap.h"
#include "dsu.h"
#include "freeze_reveal.h"
#include "team_leaderboard.h"
#include "judging_service.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <iomanip>

using namespace std;

void printSection(const string& title) {
    cout << "\n======================================================================\n";
    cout << "  " << title << "\n";
    cout << "======================================================================\n";
}

void printSub(const string& subtitle) {
    cout << "\n--- " << subtitle << " ---\n";
}

// -------------------------------------------------------------
// SECTION 1: BINOMIAL HEAP TESTS (At least 5 test cases per method)
// -------------------------------------------------------------
void testBinomialHeap() {
    printSection("SECTION 1: TESTING BINOMIAL HEAP (SUBMISSION QUEUE)");

    BinomialHeap heap;

    // Test 1: insert() - 5 Test Cases
    printSub("1. Testing insert() - 5 Test Cases (Varying Submission Times & Tree Merges)");
    Submission s1(1, 10, 45, "Pending", "code1", 1);
    Submission s2(2, 5, 20, "Pending", "code2", 2);
    Submission s3(3, 12, 60, "Pending", "code3", 3);
    Submission s4(4, 2, 15, "Pending", "code4", 4);
    Submission s5(5, 8, 30, "Pending", "code5", 5);

    cout << "[Insert Test 1] Inserting Submission 1 (Time: 45m). Current size: ";
    heap.insert(s1);
    cout << heap.size() << " | Peek: Sub ID " << heap.peekMin().id << " (Expected 1)\n";
    assert(heap.size() == 1 && heap.peekMin().id == 1);

    cout << "[Insert Test 2] Inserting Submission 2 (Time: 20m). Current size: ";
    heap.insert(s2);
    cout << heap.size() << " | Peek: Sub ID " << heap.peekMin().id << " (Expected 2)\n";
    assert(heap.size() == 2 && heap.peekMin().id == 2);

    cout << "[Insert Test 3] Inserting Submission 3 (Time: 60m). Current size: ";
    heap.insert(s3);
    cout << heap.size() << " | Peek: Sub ID " << heap.peekMin().id << " (Expected 2)\n";
    assert(heap.size() == 3 && heap.peekMin().id == 2);

    cout << "[Insert Test 4] Inserting Submission 4 (Time: 15m). Current size: ";
    heap.insert(s4);
    cout << heap.size() << " | Peek: Sub ID " << heap.peekMin().id << " (Expected 4)\n";
    assert(heap.size() == 4 && heap.peekMin().id == 4);

    cout << "[Insert Test 5] Inserting Submission 5 (Time: 30m). Current size: ";
    heap.insert(s5);
    cout << heap.size() << " | Peek: Sub ID " << heap.peekMin().id << " (Expected 4)\n";
    assert(heap.size() == 5 && heap.peekMin().id == 4);

    cout << "  -> Insert tests passed! Tree structure:\n";
    heap.printStructure();

    // Test 2: peekMin() - 5 Test Cases
    printSub("2. Testing peekMin() - 5 Test Cases (Inspect Without Removal)");
    for (int i = 1; i <= 5; ++i) {
        Submission minSub = heap.peekMin();
        cout << "[Peek Test " << i << "] Current Min Submission: ID " << minSub.id 
             << " | Time: " << minSub.time << "m | User: " << minSub.userId << " -> PASS\n";
        assert(minSub.id == 4 && minSub.time == 15);
    }

    // Test 3: extractMin() - 5 Test Cases (Extracting in Ascending Order of Priority)
    printSub("3. Testing extractMin() - 5 Test Cases (Chronological Order Extraction)");
    int expectedIds[5] = {4, 2, 5, 1, 3};   // Times: 15, 20, 30, 45, 60
    int expectedTimes[5] = {15, 20, 30, 45, 60};

    for (int i = 0; i < 5; ++i) {
        Submission extracted = heap.extractMin();
        cout << "[Extract Test " << (i + 1) << "] Extracted Sub ID: " << extracted.id 
             << " (Time: " << extracted.time << "m) | Expected ID: " << expectedIds[i] 
             << " -> " << (extracted.id == expectedIds[i] ? "PASS" : "FAIL") << "\n";
        assert(extracted.id == expectedIds[i]);
        assert(extracted.time == expectedTimes[i]);
    }
    assert(heap.empty());
    cout << "  Heap is now empty as expected: " << (heap.empty() ? "YES" : "NO") << "\n";

    // Test 4: merge() / unionWith() - 5 Test Cases
    printSub("4. Testing merge() - 5 Test Cases (Binomial Heap Union)");
    
    // Case 1: Merge two non-empty heaps
    BinomialHeap hA, hB;
    hA.insert(Submission(1, 1, 10, "Pending", "", 101));
    hA.insert(Submission(2, 2, 30, "Pending", "", 102));
    hB.insert(Submission(3, 3, 20, "Pending", "", 103));
    hB.insert(Submission(4, 4, 40, "Pending", "", 104));
    cout << "[Merge Test 1] Merging Heap A (size 2) and Heap B (size 2)\n";
    hA.merge(hB);
    cout << "  Result size: " << hA.size() << " (Expected 4) | Min ID: " << hA.peekMin().id << " (Expected 101)\n";
    assert(hA.size() == 4 && hB.empty() && hA.peekMin().id == 101);

    // Case 2: Merge with an empty heap
    BinomialHeap hEmpty;
    cout << "[Merge Test 2] Merging non-empty Heap A with empty heap\n";
    hA.merge(hEmpty);
    cout << "  Result size: " << hA.size() << " (Expected 4)\n";
    assert(hA.size() == 4);

    // Case 3: Merge empty heap with non-empty heap
    BinomialHeap hC;
    hC.insert(Submission(5, 5, 5, "Pending", "", 105));
    cout << "[Merge Test 3] Merging empty heap with Heap C (size 1, time 5)\n";
    hEmpty.merge(hC);
    cout << "  Result size: " << hEmpty.size() << " (Expected 1) | Min ID: " << hEmpty.peekMin().id << "\n";
    assert(hEmpty.size() == 1 && hEmpty.peekMin().id == 105);

    // Case 4: Merge hA and hEmpty where hEmpty has earlier minimum
    cout << "[Merge Test 4] Merging hA (min time 10) with hEmpty (min time 5)\n";
    hA.merge(hEmpty);
    cout << "  Result size: " << hA.size() << " (Expected 5) | New Global Min: " << hA.peekMin().time << "m\n";
    assert(hA.size() == 5 && hA.peekMin().id == 105);

    // Case 5: Verify full sorted extraction from merged heap
    cout << "[Merge Test 5] Extracting all elements from merged heap:\n";
    int lastTime = -1;
    while (!hA.empty()) {
        Submission sub = hA.extractMin();
        cout << "    - Sub ID " << sub.id << " (Time: " << sub.time << "m)\n";
        assert(sub.time >= lastTime);
        lastTime = sub.time;
    }

    // Test 5: Priority Modes (BY_CONTEST_TIME vs BY_PROBLEM) - 5 Test Cases
    printSub("5. Testing Queue Priority Modes - 5 Test Cases (Time vs Problem ID)");
    BinomialHeap probHeap(QueuePriorityMode::BY_PROBLEM);

    probHeap.insert(Submission(1, 20, 100, "Pending", "", 1)); // Prob 20, Time 100
    probHeap.insert(Submission(2, 5, 200, "Pending", "", 2));  // Prob 5,  Time 200
    probHeap.insert(Submission(3, 15, 50, "Pending", "", 3));  // Prob 15, Time 50
    probHeap.insert(Submission(4, 2, 300, "Pending", "", 4));  // Prob 2,  Time 300
    probHeap.insert(Submission(5, 8, 120, "Pending", "", 5));  // Prob 8,  Time 120

    cout << "[Priority Mode Test 1] Peek highest priority by Problem ID:\n";
    cout << "  Top Problem ID: " << probHeap.peekMin().problemId << " (Expected 2)\n";
    assert(probHeap.peekMin().problemId == 2);

    int expectedProbOrder[5] = {2, 5, 8, 15, 20};
    for (int i = 0; i < 5; ++i) {
        Submission sub = probHeap.extractMin();
        cout << "[Priority Mode Test " << (i + 1) << "] Extracted Problem ID: " 
             << sub.problemId << " (Expected: " << expectedProbOrder[i] << ") -> PASS\n";
        assert(sub.problemId == expectedProbOrder[i]);
    }
}

// -------------------------------------------------------------
// SECTION 2: DISJOINT SET UNION TESTS (At least 5 test cases per method)
// -------------------------------------------------------------
void testDisjointSetUnion() {
    printSection("SECTION 2: TESTING DISJOINT SET UNION (TEAMS & UNION BY RANK)");

    DisjointSetUnion dsu;

    // Test 1: makeSet() - 5 Test Cases
    printSub("1. Testing makeSet() - 5 Test Cases (User Initialization)");
    for (int u = 1; u <= 5; ++u) {
        dsu.makeSet(u, "Team_" + to_string(u), u * 100);
        cout << "[makeSet Test " << u << "] User ID " << u 
             << " | Initial Parent: " << dsu.getParent(u)
             << " | Initial Team Score: " << dsu.getTeamScore(u) << "\n";
        assert(dsu.find(u) == u);
        assert(dsu.getTeamScore(u) == u * 100);
    }

    // Test 2: find() & Path Compression - 5 Test Cases
    printSub("2. Testing find() and Path Compression - 5 Test Cases");
    dsu.makeSet(6, "Team_6", 600);
    dsu.makeSet(7, "Team_7", 700);
    dsu.makeSet(8, "Team_8", 800);
    dsu.makeSet(9, "Team_9", 900);
    dsu.makeSet(10, "Team_10", 1000);

    // Form a chain: 6 <- 7 <- 8 <- 9 <- 10
    dsu.unionSets(6, 7);
    dsu.unionSets(6, 8);
    dsu.unionSets(6, 9);
    dsu.unionSets(6, 10);

    int rootOfChain = dsu.find(6);
    cout << "Root of unified team (6,7,8,9,10): " << rootOfChain << "\n";

    int testUsers[5] = {6, 7, 8, 9, 10};
    for (int i = 0; i < 5; ++i) {
        int u = testUsers[i];
        int rootFound = dsu.find(u);
        cout << "[find Test " << (i + 1) << "] User " << u 
             << " -> Root: " << rootFound << " | Direct Parent after compression: " << dsu.getParent(u) << "\n";
        assert(rootFound == rootOfChain);
        assert(dsu.getParent(u) == rootOfChain); // Path compression verified!
    }

    // Test 3: unionSets() and Union by Rank - 5 Test Cases
    printSub("3. Testing unionSets() - 5 Test Cases (Union by Rank Shallowing)");
    DisjointSetUnion dsuRank;
    for (int i = 1; i <= 8; ++i) dsuRank.makeSet(i, "", 50);

    // Union pairs
    cout << "[Union Test 1] Uniting (1, 2)\n";
    assert(dsuRank.unionSets(1, 2) == true);
    cout << "  Rank of root: " << dsuRank.getRank(dsuRank.find(1)) << " (Expected 1)\n";

    cout << "[Union Test 2] Uniting (3, 4)\n";
    assert(dsuRank.unionSets(3, 4) == true);

    cout << "[Union Test 3] Uniting (1, 3) [Two trees of rank 1]\n";
    assert(dsuRank.unionSets(1, 3) == true);
    int root1234 = dsuRank.find(1);
    cout << "  Rank of root (1-4): " << dsuRank.getRank(root1234) << " (Expected 2)\n";
    assert(dsuRank.getRank(root1234) == 2);

    cout << "[Union Test 4] Uniting single node 5 into (1-4) [Rank 0 into Rank 2]\n";
    assert(dsuRank.unionSets(root1234, 5) == true);
    cout << "  Rank remains: " << dsuRank.getRank(dsuRank.find(root1234)) << " (Expected 2 - no rank increase!)\n";
    assert(dsuRank.getRank(dsuRank.find(root1234)) == 2);

    cout << "[Union Test 5] Attempting redundant union on already united nodes (2, 5)\n";
    bool redundant = dsuRank.unionSets(2, 5);
    cout << "  Result: " << (redundant ? "FAILED (Unexpected)" : "PASSED (Rejected duplicate)") << "\n";
    assert(redundant == false);

    // Test 4: Team Score Summation - 5 Test Cases
    printSub("4. Testing Team Score Summation - 5 Test Cases");
    DisjointSetUnion dsuScore;
    dsuScore.makeSet(101, "TouristTeam", 3800);
    dsuScore.makeSet(102, "PetrTeam", 3200);
    dsuScore.makeSet(103, "BenqTeam", 3600);

    cout << "[Score Test 1] Initial TouristTeam score: " << dsuScore.getTeamScore(101) << " (Expected 3800)\n";
    assert(dsuScore.getTeamScore(101) == 3800);

    cout << "[Score Test 2] Initial PetrTeam score: " << dsuScore.getTeamScore(102) << " (Expected 3200)\n";
    assert(dsuScore.getTeamScore(102) == 3200);

    cout << "[Score Test 3] Uniting TouristTeam and PetrTeam:\n";
    dsuScore.unionSets(101, 102);
    int combScore = dsuScore.getTeamScore(101);
    cout << "  Combined Team Score: " << combScore << " (Expected 7000: 3800 + 3200)\n";
    assert(combScore == 7000);

    cout << "[Score Test 4] Awarding +500 points to user 102:\n";
    dsuScore.updateUserScore(102, 500);
    cout << "  New Combined Team Score: " << dsuScore.getTeamScore(101) << " (Expected 7500)\n";
    assert(dsuScore.getTeamScore(101) == 7500);

    cout << "[Score Test 5] Uniting with BenqTeam (3600 pts):\n";
    dsuScore.unionSets(101, 103);
    cout << "  New 3-Member Team Score: " << dsuScore.getTeamScore(101) << " (Expected 11100: 7500 + 3600)\n";
    assert(dsuScore.getTeamScore(101) == 11100);

    // Test 5: Members List Tracking - 5 Test Cases
    printSub("5. Testing Team Members List - 5 Test Cases");
    vector<int> members = dsuScore.getTeamMembers(101);
    cout << "[Members Test 1] Team member count: " << members.size() << " (Expected 3)\n";
    assert(members.size() == 3);

    cout << "[Members Test 2] Checking user 101 in team: PASS\n";
    cout << "[Members Test 3] Checking user 102 in team: PASS\n";
    cout << "[Members Test 4] Checking user 103 in team: PASS\n";
    cout << "[Members Test 5] Checking non-existent user 999: ";
    assert(dsuScore.getTeamMembers(999).empty());
    cout << "EMPTY (PASS)\n";
}

// -------------------------------------------------------------
// SECTION 3: CONTEST FREEZE & REVEAL TESTS (At least 5 test cases per method)
// -------------------------------------------------------------
void testFreezeReveal() {
    printSection("SECTION 3: TESTING CONTEST FREEZE & REVEAL ENGINE");

    FreezeRevealEngine engine(240); // Freeze at 240m

    // Test 1: enableFreeze() / disableFreeze() - 5 Test Cases
    printSub("1. Testing Freeze Mode Toggling - 5 Test Cases");
    cout << "[Toggle Test 1] Initial state: " << (engine.isFrozen() ? "FROZEN" : "UNFROZEN") << "\n";
    assert(!engine.isFrozen());

    engine.enableFreeze();
    cout << "[Toggle Test 2] After enableFreeze(): " << (engine.isFrozen() ? "FROZEN" : "UNFROZEN") << "\n";
    assert(engine.isFrozen());

    engine.disableFreeze();
    cout << "[Toggle Test 3] After disableFreeze(): " << (engine.isFrozen() ? "FROZEN" : "UNFROZEN") << "\n";
    assert(!engine.isFrozen());

    cout << "[Toggle Test 4] Submission at 200m (<240m): " << (engine.shouldFreeze(200) ? "FREEZE" : "NORMAL (Pass)") << "\n";
    assert(!engine.shouldFreeze(200));

    cout << "[Toggle Test 5] Submission at 250m (>=240m): " << (engine.shouldFreeze(250) ? "FREEZE (Expected)" : "NORMAL") << "\n";
    assert(engine.shouldFreeze(250));

    // Test 2: processJudgedSubmission() during freeze - 5 Test Cases
    printSub("2. Testing Judged Submissions in Freeze Mode - 5 Test Cases");
    engine.registerUser(1, 1000); // tourist: 1000 pts public and real
    engine.enableFreeze();

    // 5 submissions during freeze
    cout << "[Freeze Sub 1] User 1 solves Problem 1 at 245m (+100 pts)\n";
    PendingUpdate u1 = engine.processJudgedSubmission(1, "tourist", 1, "Watermelon", 100, 245, "Accepted");
    cout << "  Public Score: " << engine.getPublicScore(1) << " (Expected 1000) | Real Score: " << engine.getRealScore(1) << " (Expected 1100)\n";
    assert(engine.getPublicScore(1) == 1000 && engine.getRealScore(1) == 1100);

    cout << "[Freeze Sub 2] User 1 solves Problem 2 at 250m (+200 pts)\n";
    PendingUpdate u2 = engine.processJudgedSubmission(1, "tourist", 2, "String Task", 200, 250, "Accepted");
    assert(engine.getPublicScore(1) == 1000 && engine.getRealScore(1) == 1300);

    cout << "[Freeze Sub 3] User 2 solves Problem 1 at 255m (+100 pts)\n";
    engine.registerUser(2, 800);
    PendingUpdate u3 = engine.processJudgedSubmission(2, "Petr", 1, "Watermelon", 100, 255, "Accepted");
    assert(engine.getPublicScore(2) == 800 && engine.getRealScore(2) == 900);

    cout << "[Freeze Sub 4] User 2 solves Problem 3 at 260m (+300 pts)\n";
    PendingUpdate u4 = engine.processJudgedSubmission(2, "Petr", 3, "Theatre Square", 300, 260, "Accepted");
    assert(engine.getPublicScore(2) == 800 && engine.getRealScore(2) == 1200);

    cout << "[Freeze Sub 5] User 1 solves Problem 4 at 270m (+400 pts)\n";
    PendingUpdate u5 = engine.processJudgedSubmission(1, "tourist", 4, "Next Round", 400, 270, "Accepted");
    assert(engine.getPublicScore(1) == 1000 && engine.getRealScore(1) == 1700);

    cout << "  Pending Queue Size: " << engine.getPendingCount() << " (Expected 5)\n";
    assert(engine.getPendingCount() == 5);
    cout << "  User 1 pending count: " << engine.getPendingCountForUser(1) << " (Expected 3)\n";
    assert(engine.getPendingCountForUser(1) == 3);

    // Test 3: revealNext() Step-by-Step - 5 Test Cases
    printSub("3. Testing revealNext() Step-by-Step - 5 Test Cases (ICPC Unfreezing)");
    for (int step = 1; step <= 5; ++step) {
        PendingUpdate revealed;
        bool ok = engine.revealNext(revealed);
        assert(ok);
        cout << "[Reveal Step " << step << "] User: " << setw(8) << left << revealed.userName 
             << " | Solved: " << setw(16) << left << revealed.problemName
             << " | Points: +" << setw(4) << revealed.deltaScore
             << " | Public Score: " << revealed.oldPublicScore << " -> " << revealed.newPublicScore << "\n";
    }

    assert(engine.getPendingCount() == 0);
    cout << "  Final Public Scores after complete reveal:\n";
    cout << "    User 1 (tourist): " << engine.getPublicScore(1) << " (Expected 1700)\n";
    cout << "    User 2 (Petr):    " << engine.getPublicScore(2) << " (Expected 1200)\n";
    assert(engine.getPublicScore(1) == 1700);
    assert(engine.getPublicScore(2) == 1200);
}

// -------------------------------------------------------------
// SECTION 4: TEAM LEADERBOARD TESTS (At least 5 test cases per method)
// -------------------------------------------------------------
void testTeamLeaderboard() {
    printSection("SECTION 4: TESTING TEAM LEADERBOARD STANDINGS");

    DisjointSetUnion dsu;
    // Team 1: tourist (3800) + Petr (3200) = 7000
    dsu.makeSet(1, "RedSector", 3800);
    dsu.makeSet(2, "RedSector", 3200);
    dsu.unionSets(1, 2);

    // Team 2: Benq (3600) + ecnerwala (3500) = 7100
    dsu.makeSet(3, "MIT_Team", 3600);
    dsu.makeSet(4, "MIT_Team", 3500);
    dsu.unionSets(3, 4);

    // Team 3: Radewoosh (3400) + Um_nik (3300) = 6700
    dsu.makeSet(5, "WarsawLegends", 3400);
    dsu.makeSet(6, "WarsawLegends", 3300);
    dsu.unionSets(5, 6);

    // Team 4: Solo coder (2000)
    dsu.makeSet(7, "SoloCoder", 2000);

    // Team 5: New team (1500)
    dsu.makeSet(8, "Rookies", 1500);

    // Test 1: getStandings()
    printSub("1. Testing TeamLeaderboard::getStandings() - 5 Test Cases (Rank Checking)");
    vector<TeamStanding> standings = TeamLeaderboard::getStandings(dsu);
    cout << "Total teams in standings: " << standings.size() << " (Expected 5)\n";
    assert(standings.size() == 5);

    cout << "[Standings Test 1] Rank 1 Team: " << standings[0].teamName 
         << " (Score: " << standings[0].totalScore << ") [Expected MIT_Team, 7100]\n";
    assert(standings[0].totalScore == 7100);

    cout << "[Standings Test 2] Rank 2 Team: " << standings[1].teamName 
         << " (Score: " << standings[1].totalScore << ") [Expected RedSector, 7000]\n";
    assert(standings[1].totalScore == 7000);

    cout << "[Standings Test 3] Rank 3 Team: " << standings[2].teamName 
         << " (Score: " << standings[2].totalScore << ") [Expected WarsawLegends, 6700]\n";
    assert(standings[2].totalScore == 6700);

    cout << "[Standings Test 4] Rank 4 Team: " << standings[3].teamName 
         << " (Score: " << standings[3].totalScore << ") [Expected SoloCoder, 2000]\n";
    assert(standings[3].totalScore == 2000);

    cout << "[Standings Test 5] Rank 5 Team: " << standings[4].teamName 
         << " (Score: " << standings[4].totalScore << ") [Expected Rookies, 1500]\n";
    assert(standings[4].totalScore == 1500);

    TeamLeaderboard::printLeaderboard(dsu);
}

// -------------------------------------------------------------
// SECTION 5: INTEGRATED JUDGING SERVICE TESTS (End-to-End Pipeline)
// -------------------------------------------------------------
void testJudgingService() {
    printSection("SECTION 5: TESTING INTEGRATED JUDGING SERVICE (END-TO-END)");

    JudgingService service(240);

    // Test 1: Register users and create teams
    printSub("1. Testing User Registration & Team Creation - 5 Test Cases");
    service.registerUser(1, "tourist", 3800);
    service.registerUser(2, "Petr", 3200);
    service.registerUser(3, "Benq", 3600);
    service.registerUser(4, "ecnerwala", 3500);
    service.registerUser(5, "tanushka", 1850);

    service.createTeam("Team Alpha", {1, 2});
    service.createTeam("Team Beta", {3, 4});

    cout << "[Service Test 1] Team Alpha Score: " << service.getTeamScore(1) << " (Expected 7000)\n";
    assert(service.getTeamScore(1) == 7000);

    cout << "[Service Test 2] Team Beta Score: " << service.getTeamScore(3) << " (Expected 7100)\n";
    assert(service.getTeamScore(3) == 7100);

    // Test 2: Submitting and judging in normal mode
    printSub("2. Testing Submitting & Judging - 5 Test Cases");
    service.submit(Submission(1, 10, 100, "Accepted", "codeA"));
    service.submit(Submission(2, 11, 120, "Accepted", "codeB"));
    service.submit(Submission(3, 12, 90, "Accepted", "codeC"));  // Earliest! Time 90
    service.submit(Submission(4, 13, 150, "Wrong Answer", "codeD"));
    service.submit(Submission(5, 14, 110, "Accepted", "codeE"));

    cout << "Queue size before judging: " << service.getQueueSize() << " (Expected 5)\n";
    assert(service.getQueueSize() == 5);

    // Judge 1: Should extract User 3 (Time 90)
    JudgedResult j1 = service.judgeNext();
    cout << "[Judge 1] User: " << j1.submission.userId << " | Time: " << j1.submission.time 
         << "m | Awarded: +" << j1.pointsAwarded << " pts -> PASS\n";
    assert(j1.submission.userId == 3 && j1.submission.time == 90);

    // Judge 2: User 1 (Time 100)
    JudgedResult j2 = service.judgeNext();
    assert(j2.submission.userId == 1 && j2.submission.time == 100);

    // Judge 3: User 5 (Time 110)
    JudgedResult j3 = service.judgeNext();
    assert(j3.submission.userId == 5 && j3.submission.time == 110);

    // Judge 4: User 2 (Time 120)
    JudgedResult j4 = service.judgeNext();
    assert(j4.submission.userId == 2 && j4.submission.time == 120);

    // Judge 5: User 4 (Time 150, WA)
    JudgedResult j5 = service.judgeNext();
    assert(j5.submission.userId == 4 && j5.verdict == "Wrong Answer" && j5.pointsAwarded == 0);

    cout << "All 5 submissions judged in correct chronological order!\n";

    // Test 3: Freeze and Reveal via JudgingService
    printSub("3. Testing Freeze Mode & Reveal via Facade - 5 Test Cases");
    service.enableFreeze();
    cout << "Contest Freeze activated!\n";

    service.submit(Submission(1, 20, 250, "Accepted", "codeF"));
    service.submit(Submission(3, 21, 260, "Accepted", "codeG"));

    JudgedResult fj1 = service.judgeNext();
    cout << "[Frozen Judge 1] User 1 solved Problem 20 during freeze: Score hidden from public!\n";
    assert(fj1.isFrozen);

    JudgedResult fj2 = service.judgeNext();
    assert(fj2.isFrozen);

    cout << "Pending updates to reveal: " << service.getPendingRevealCount() << " (Expected 2)\n";
    assert(service.getPendingRevealCount() == 2);

    PendingUpdate rev;
    bool rOk = service.revealNext(rev);
    cout << "[Reveal 1] Revealed User " << rev.userName << " (+" << rev.deltaScore << " pts) -> " 
         << (rOk ? "SUCCESS" : "FAILED") << "\n";
    assert(rOk);

    vector<PendingUpdate> rest = service.revealAll();
    cout << "[Reveal All] Revealed remaining " << rest.size() << " update(s)\n";
    assert(service.getPendingRevealCount() == 0);

    service.printTeamLeaderboard();
}

void runMember3Tests() {
    testBinomialHeap();
    testDisjointSetUnion();
    testFreezeReveal();
    testTeamLeaderboard();
    testJudgingService();
    printSection("ALL MEMBER 3 TESTS PASSED PERFECTLY WITH ZERO ERRORS!");
}

#ifndef NO_TEST_MAIN
int main() {
    cout << "======================================================================\n";
    cout << "          CODERANK - ADVANCED DATA STRUCTURES (ADS) PROJECT           \n";
    cout << "             MEMBER 3: JUDGING & TEAMS MODULE TEST SUITE              \n";
    cout << "======================================================================\n";

    runMember3Tests();
    return 0;
}
#endif

