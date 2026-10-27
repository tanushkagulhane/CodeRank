// main.cpp - Member 3 (Nandini) Entry Point & Interactive Showcase
// Advanced Data Structures - CodeRank
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
#include <iomanip>

using namespace std;

void showBanner() {
    cout << "\n======================================================================\n";
    cout << "          CODERANK - ADVANCED DATA STRUCTURES (ADS) PROJECT           \n";
    cout << "             MEMBER 3: JUDGING & TEAMS (NANDINI)                      \n";
    cout << "======================================================================\n";
    cout << " Data Structures Implemented:\n";
    cout << "   - Hard:   Binomial Heap (Submission Queue, Extract-Min, Merge)\n";
    cout << "   - Medium: Disjoint Set Union (Teams, Path Compression, Union by Rank)\n";
    cout << "   - Easy:   Freeze & Reveal Logic, Plus Team Leaderboard\n";
    cout << "   - Shared: Performance Benchmarks vs Plain Sorted List\n";
    cout << "======================================================================\n";
}

void runInteractiveDemo() {
    JudgingService service(240);

    // Seed sample users & teams
    service.registerUser(1, "tourist", 3800);
    service.registerUser(2, "Petr", 3200);
    service.registerUser(3, "Benq", 3600);
    service.registerUser(4, "ecnerwala", 3500);
    service.registerUser(5, "Radewoosh", 3400);
    service.registerUser(6, "Um_nik", 3300);
    service.registerUser(7, "tanushka", 1850);
    service.registerUser(8, "nandini", 1950);

    service.createTeam("RedSector", {1, 2});
    service.createTeam("MIT_Alums", {3, 4});
    service.createTeam("WarsawLegends", {5, 6});
    service.createTeam("CodeRankDevs", {7, 8});

    int choice = -1;
    while (choice != 0) {
        cout << "\n--------------------------------------------------\n";
        cout << "                 MEMBER 3 MENU                    \n";
        cout << "--------------------------------------------------\n";
        cout << " [1] Add Submission to Queue (Binomial Heap)\n";
        cout << " [2] Peek / Inspect Earliest Submission\n";
        cout << " [3] Judge Next Submission (Extract-Min)\n";
        cout << " [4] View Binomial Tree Structure of Queue\n";
        cout << " [5] Merge Two Submission Queues (O(log n))\n";
        cout << " [6] View Team Leaderboard (Sum of Member Scores)\n";
        cout << " [7] Merge Two Teams (DSU Union by Rank)\n";
        cout << " [8] Toggle Contest Freeze Mode (Freeze/Unfreeze)\n";
        cout << " [9] Reveal Next Frozen Submission (Dramatic Reveal!)\n";
        cout << " [10] Reveal All Pending Frozen Updates\n";
        cout << " [11] Run Performance Benchmark (ADS vs Plain List)\n";
        cout << " [12] Run Automated Assertion Tests (All 5 Sections)\n";
        cout << " [0] Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            string junk;
            getline(cin, junk);
            continue;
        }

        switch (choice) {
            case 1: {
                int uId, pId, t;
                string code;
                cout << "Enter User ID (1-8): "; cin >> uId;
                cout << "Enter Problem ID: "; cin >> pId;
                cout << "Enter Contest Time in minutes (e.g. 15, 120, 255): "; cin >> t;
                cout << "Enter simple code string / snippet: ";
                cin.ignore();
                getline(cin, code);
                if (code.empty()) code = "cout << 42;";

                Submission sub(uId, pId, t, "Accepted", code);
                service.submit(sub);
                cout << ">> Submission added to Binomial Heap queue! Queue size: " << service.getQueueSize() << "\n";
                break;
            }
            case 2: {
                if (!service.hasPendingSubmissions()) {
                    cout << ">> Submission queue is currently empty!\n";
                } else {
                    Submission top = service.peekNextSubmission();
                    cout << "\n>> Highest Priority Submission in Queue:\n";
                    cout << "   - Sub ID:      " << top.id << "\n";
                    cout << "   - User:        " << top.userId << " (" << service.getUserName(top.userId) << ")\n";
                    cout << "   - Problem:     " << top.problemId << "\n";
                    cout << "   - Contest Time:" << top.time << "m\n";
                }
                break;
            }
            case 3: {
                if (!service.hasPendingSubmissions()) {
                    cout << ">> No submissions in queue to judge!\n";
                } else {
                    JudgedResult res = service.judgeNext();
                    cout << "\n>> JUDGING RESULT:\n";
                    cout << "   - Sub ID:         " << res.submission.id << "\n";
                    cout << "   - User:           " << res.submission.userId << " (" << service.getUserName(res.submission.userId) << ")\n";
                    cout << "   - Problem:        " << res.submission.problemId << "\n";
                    cout << "   - Verdict:        " << res.verdict << "\n";
                    cout << "   - Points Awarded: +" << res.pointsAwarded << " pts\n";
                    cout << "   - New User Score: " << res.newScore << "\n";
                    cout << "   - Team Score:     " << res.teamScore << " (Team " << service.getTeamName(res.teamId) << ")\n";
                    if (res.isFrozen) {
                        cout << "   *** NOTE: CONTEST IS FROZEN! Public scoreboard unchanged until reveal! ***\n";
                    }
                }
                break;
            }
            case 4: {
                service.getQueue().printStructure();
                break;
            }
            case 5: {
                cout << "\n>> Demonstrating O(log n) Binomial Heap Queue Merge:\n";
                JudgingService room2;
                room2.submit(Submission(1, 101, 15, "Accepted", "code1"));
                room2.submit(Submission(2, 102, 35, "Accepted", "code2"));
                room2.submit(Submission(3, 103, 55, "Accepted", "code3"));
                cout << "Created Room 2 with 3 submissions. Merging into main contest queue...\n";
                service.mergeQueues(room2);
                cout << "Merge complete! New main queue size: " << service.getQueueSize() << "\n";
                break;
            }
            case 6: {
                cout << "\n[1] Public Standings (Frozen if active)\n[2] Real Standings (Internal backend view)\nChoice: ";
                int v; cin >> v;
                service.printTeamLeaderboard(v == 1);
                break;
            }
            case 7: {
                int t1, t2;
                cout << "Enter User ID in Team 1: "; cin >> t1;
                cout << "Enter User ID in Team 2: "; cin >> t2;
                bool ok = service.mergeTeams(t1, t2);
                if (ok) {
                    cout << ">> Teams merged successfully via DSU Union by Rank!\n";
                    cout << "   New Combined Team Name: " << service.getTeamName(t1) << "\n";
                    cout << "   New Combined Score:     " << service.getTeamScore(t1) << " pts\n";
                } else {
                    cout << ">> Failed to merge: either invalid users or already in the same team!\n";
                }
                break;
            }
            case 8: {
                if (service.isFrozen()) {
                    service.disableFreeze();
                    cout << ">> Contest Freeze DISABLED! Leaderboard updates are now live.\n";
                } else {
                    service.enableFreeze();
                    cout << ">> Contest Freeze ENABLED! Leaderboard updates are now withheld.\n";
                }
                break;
            }
            case 9: {
                PendingUpdate rev;
                bool ok = service.revealNext(rev);
                if (!ok) {
                    cout << ">> No pending frozen updates to reveal!\n";
                } else {
                    cout << "\n======================================================================\n";
                    cout << "  *** DRAMATIC REVEAL EVENT ***                                       \n";
                    cout << "  Contestant: " << rev.userName << " (User " << rev.userId << ")\n";
                    cout << "  Problem:    " << rev.problemName << " (Problem " << rev.problemId << ")\n";
                    cout << "  Result:     " << rev.verdict << " (+" << rev.deltaScore << " pts!)\n";
                    cout << "  Score:      " << rev.oldPublicScore << " ---> " << rev.newPublicScore << " pts!\n";
                    cout << "======================================================================\n";
                    cout << "Pending updates remaining: " << service.getPendingRevealCount() << "\n";
                }
                break;
            }
            case 10: {
                int count = service.getPendingRevealCount();
                if (count == 0) {
                    cout << ">> No pending frozen updates to reveal!\n";
                } else {
                    vector<PendingUpdate> revealed = service.revealAll();
                    cout << ">> Revealed all " << revealed.size() << " pending update(s)!\n";
                    service.printTeamLeaderboard(false);
                }
                break;
            }
            case 11: {
                TimingComparison::runAllBenchmarks(true);
                break;
            }
            case 12: {
                void runMember3Tests();
                runMember3Tests();
                break;
            }
            case 0:
                cout << "Exiting CodeRank Member 3 showcase.\n";
                break;
            default:
                cout << "Invalid choice! Please choose 0-12.\n";
        }
    }
}

// Forward declaration from test_judging_teams.cpp
void runMember3Tests();

int main(int argc, char* argv[]) {
    showBanner();

    // Check command line arguments for non-interactive test/benchmark runs
    if (argc > 1) {
        string arg = argv[1];
        if (arg == "--test") {
            runMember3Tests();
            return 0;
        } else if (arg == "--benchmark") {
            TimingComparison::runAllBenchmarks(true);
            return 0;
        }
    }

    runInteractiveDemo();
    return 0;
}
