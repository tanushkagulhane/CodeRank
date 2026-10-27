// run_demo.cpp - Automated End-to-End Demo Script for Member 3
// Member 3 (Judging & Teams) - Advanced Data Structures
#include "Models.h"
#include "binomial_heap.h"
#include "dsu.h"
#include "freeze_reveal.h"
#include "team_leaderboard.h"
#include "judging_service.h"
#include "timing_comparison.h"
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

void stepHeader(int step, const string& title) {
    cout << "\n======================================================================\n";
    cout << "  DEMO STEP " << step << ": " << title << "\n";
    cout << "======================================================================\n";
}

int main() {
    cout << "======================================================================\n";
    cout << "          CODERANK - ADVANCED DATA STRUCTURES (ADS) PROJECT           \n";
    cout << "             MEMBER 3 (NANDINI): END-TO-END DEMO SCRIPT               \n";
    cout << "======================================================================\n";

    JudgingService service(240);

    // -------------------------------------------------------------
    // Step 1: User Registration & Team Formation via DSU
    // -------------------------------------------------------------
    stepHeader(1, "TEAM FORMATION USING DISJOINT SET UNION (DSU)");
    cout << "Registering 8 competitive programming contestants...\n";
    service.registerUser(1, "tourist", 3800);
    service.registerUser(2, "Petr", 3200);
    service.registerUser(3, "Benq", 3600);
    service.registerUser(4, "ecnerwala", 3500);
    service.registerUser(5, "Radewoosh", 3400);
    service.registerUser(6, "Um_nik", 3300);
    service.registerUser(7, "tanushka", 1850);
    service.registerUser(8, "nandini", 1950);

    cout << "Creating initial 2-member teams using DSU...\n";
    service.createTeam("RedSector", {1, 2});
    service.createTeam("MIT_Alums", {3, 4});
    service.createTeam("WarsawLegends", {5, 6});
    service.createTeam("CodeRankTeam", {7, 8});

    cout << "Initial Team Standings (Team score = Sum of member scores):\n";
    service.printTeamLeaderboard();

    // -------------------------------------------------------------
    // Step 2: DSU Team Merging with Union by Rank & Path Compression
    // -------------------------------------------------------------
    stepHeader(2, "MERGING TEAMS DYNAMICALLY WITH UNION BY RANK");
    cout << "Merging Team 'WarsawLegends' (Users 5, 6) into 'CodeRankTeam' (Users 7, 8)...\n";
    service.mergeTeams(5, 7);
    cout << "New combined team score: " << service.getTeamScore(5) 
         << " pts (Expected 3400 + 3300 + 1850 + 1950 = 10500 pts!)\n";
    cout << "Combined Team Standings after union:\n";
    service.printTeamLeaderboard();

    // -------------------------------------------------------------
    // Step 3: Submission Queue using Binomial Heap
    // -------------------------------------------------------------
    stepHeader(3, "SUBMISSION QUEUE PRIORITIZATION (BINOMIAL HEAP)");
    cout << "Adding submissions submitted at various contest times (out of order)...\n";
    service.submit(Submission(1, 10, 45, "Accepted", "code1"));
    service.submit(Submission(2, 5, 20, "Accepted", "code2"));
    service.submit(Submission(3, 12, 60, "Accepted", "code3"));
    service.submit(Submission(4, 2, 15, "Accepted", "code4")); // Earliest! 15m
    service.submit(Submission(7, 8, 30, "Accepted", "code5"));

    cout << "Inspecting Binomial Tree structure in submission queue:\n";
    service.getQueue().printStructure();

    cout << "\nEarliest submission in queue (peekMin): Sub ID " 
         << service.peekNextSubmission().id << " at time " 
         << service.peekNextSubmission().time << "m (User " 
         << service.peekNextSubmission().userId << ")\n";

    // -------------------------------------------------------------
    // Step 4: Judging Next Submissions (Extract-Min in O(log n))
    // -------------------------------------------------------------
    stepHeader(4, "JUDGING NEXT SUBMISSIONS (EXTRACT-MIN)");
    while (service.hasPendingSubmissions()) {
        JudgedResult res = service.judgeNext();
        cout << ">> Judged Sub ID " << res.submission.id 
             << " | User: " << res.submission.userId << " (" << service.getUserName(res.submission.userId) << ")"
             << " | Time: " << res.submission.time << "m"
             << " | Verdict: " << res.verdict 
             << " | Awarded: +" << res.pointsAwarded << " pts"
             << " | New Team Score: " << res.teamScore << "\n";
    }

    // -------------------------------------------------------------
    // Step 5: Binomial Heap Queue Merge Demo
    // -------------------------------------------------------------
    stepHeader(5, "MERGING TWO INDEPENDENT SUBMISSION QUEUES (O(log n))");
    JudgingService roomB;
    cout << "Creating Contest Room B with 3 submissions...\n";
    roomB.submit(Submission(1, 20, 10, "Accepted", "codeB1"));
    roomB.submit(Submission(3, 21, 25, "Accepted", "codeB2"));
    roomB.submit(Submission(6, 22, 50, "Accepted", "codeB3"));
    cout << "Room B queue size: " << roomB.getQueueSize() << "\n";

    cout << "Performing O(log n) Binomial Heap union into Main Contest Queue...\n";
    service.mergeQueues(roomB);
    cout << "Main queue size after merge: " << service.getQueueSize() 
         << " | Earliest submission: " << service.peekNextSubmission().time << "m\n";

    // Clear queue by judging
    while (service.hasPendingSubmissions()) service.judgeNext();

    // -------------------------------------------------------------
    // Step 6: Contest Freeze Simulation
    // -------------------------------------------------------------
    stepHeader(6, "CONTEST FREEZE MODE ACTIVATION (LAST HOUR)");
    cout << "Activating Contest Freeze Mode! Public leaderboard is now frozen.\n";
    service.enableFreeze();

    cout << "Contestants submit late problems during the freeze:\n";
    service.submit(Submission(1, 29, 250, "Accepted", "codeTourist"));
    service.submit(Submission(3, 30, 260, "Accepted", "codeBenq"));
    service.submit(Submission(7, 31, 275, "Accepted", "codeNandini"));

    while (service.hasPendingSubmissions()) {
        JudgedResult res = service.judgeNext();
        cout << "  Judged Sub ID " << res.submission.id << " by User " << res.submission.userId 
             << " -> ACCEPTED! (Frozen: " << (res.isFrozen ? "YES - Withheld from Public" : "NO") << ")\n";
    }

    cout << "\n--- PUBLIC VIEW OF LEADERBOARD (FROZEN) ---\n";
    service.printTeamLeaderboard(true); // Public view

    cout << "\n--- REAL INTERNAL VIEW OF LEADERBOARD (UNFROZEN) ---\n";
    service.printTeamLeaderboard(false); // Internal backend view

    // -------------------------------------------------------------
    // Step 7: Dramatic ICPC-Style Leaderboard Reveal
    // -------------------------------------------------------------
    stepHeader(7, "DRAMATIC CONTEST REVEAL");
    cout << "Contest has concluded! Commencing the dramatic reveal of withheld updates:\n\n";

    int revStep = 1;
    PendingUpdate update;
    while (service.revealNext(update)) {
        cout << "----------------------------------------------------------------------\n";
        cout << " [REVEAL #" << revStep++ << "] " << update.userName 
             << " solved " << update.problemName << "!\n";
        cout << " Points Awarded: +" << update.deltaScore << " pts\n";
        cout << " Score Change:   " << update.oldPublicScore << " ---> " << update.newPublicScore << " pts\n";
        cout << " Remaining Pending Updates: " << service.getPendingRevealCount() << "\n";
    }

    cout << "\n======================================================================\n";
    cout << "  FINAL TRUE CHAMPIONS & STANDINGS (POST-REVEAL)                      \n";
    cout << "======================================================================\n";
    service.printTeamLeaderboard(false);

    // -------------------------------------------------------------
    // Step 8: Performance Benchmark vs Plain Sorted List
    // -------------------------------------------------------------
    stepHeader(8, "TIMING COMPARISON BENCHMARK (CUSTOM ADS VS PLAIN SORTED LIST)");
    TimingComparison::runAllBenchmarks(true);

    cout << "\n======================================================================\n";
    cout << "  DEMO SCRIPT COMPLETED WITH 100% SUCCESS!                             \n";
    cout << "======================================================================\n";

    return 0;
}
