// main.cpp - Master Integrated Console Interface for CodeRank Platform
#include "coderank_platform.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

void showHeader() {
    cout << "\n======================================================================\n";
    cout << "     CODERANK PLATFORM - INTEGRATED ADVANCED DATA STRUCTURES          \n";
    cout << "======================================================================\n";
    cout << " Member 1 (Ranking Engine):   Treap, Segment Tree, Binary Heap\n";
    cout << " Member 2 (Problems & Search): AVL Tree, Trie Autocomplete, KMP Plagiarism\n";
    cout << " Member 3 (Judging & Teams):   Binomial Heap, DSU Teams, Freeze & Reveal\n";
    cout << "======================================================================\n";
}

int main() {
    showHeader();

    CodeRankPlatform platform(100000, 240);
    cout << "Initializing platform datasets from database...\n";
    bool ok = platform.initializeFromDatabase("database");
    if (!ok) {
        // Try fallback to local dir
        platform.initializeFromDatabase(".");
    }
    cout << ">> Platform initialized successfully!\n";

    int choice = -1;
    while (choice != 0) {
        cout << "\n--------------------------------------------------\n";
        cout << "               CODERANK MASTER MENU               \n";
        cout << "--------------------------------------------------\n";
        cout << " [1] Leaderboard: What's My Rank? (Treap)\n";
        cout << " [2] Leaderboard: Top-K Users (Binary Min-Heap)\n";
        cout << " [3] Leaderboard: Count & Max in Score Range (Segment Tree)\n";
        cout << " [4] Problems: List Problems by Difficulty Range (AVL Tree)\n";
        cout << " [5] Search: Prefix Autocomplete for Problems & Users (Trie)\n";
        cout << " [6] Plagiarism: Check Submitted Code against Database (KMP)\n";
        cout << " [7] Judging: Queue & Judge Submissions (Binomial Heap)\n";
        cout << " [8] Teams: View Standings & Merge Teams (DSU Union by Rank)\n";
        cout << " [9] Contest: Toggle Freeze & Dramatic ICPC Reveal\n";
        cout << " [10] End-to-End Pipeline: Submit, Plagiarism Check, Judge, Rank\n";
        cout << " [11] Performance: Run Benchmarks (Custom ADS vs Plain List)\n";
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
                int uId;
                cout << "Enter User ID (e.g. 1 for tourist, 2 for Petr, 9 for tanushka, 16 for nandini): ";
                cin >> uId;
                int r = platform.getUserRank(uId);
                if (r == -1) cout << ">> User ID " << uId << " not found in leaderboard!\n";
                else cout << ">> Rank of User ID " << uId << " is: #" << r << "\n";
                break;
            }
            case 2: {
                int k;
                cout << "Enter K (number of top users to retrieve): ";
                cin >> k;
                vector<User> topUsers = platform.getTopKUsers(k);
                cout << "\n--- TOP " << topUsers.size() << " CONTESTANTS (Binary Heap) ---\n";
                for (size_t i = 0; i < topUsers.size(); ++i) {
                    cout << "  #" << (i + 1) << " [ID " << setw(2) << topUsers[i].id << "] " 
                         << setw(16) << left << topUsers[i].name 
                         << " | Score: " << topUsers[i].score << "\n";
                }
                break;
            }
            case 3: {
                int lo, hi;
                cout << "Enter Score Range [lo, hi] (e.g. 1500 3500): ";
                cin >> lo >> hi;
                int cnt = platform.countUsersInScoreRange(lo, hi);
                int mx = platform.maxScoreInRange(lo, hi);
                cout << ">> Segment Tree Query:\n";
                cout << "   - Users with score between " << lo << " and " << hi << ": " << cnt << "\n";
                cout << "   - Highest score in this range: " << (mx == -1 ? "None" : to_string(mx)) << "\n";
                break;
            }
            case 4: {
                int minD, maxD;
                cout << "Enter Difficulty Range [minDiff, maxDiff] (e.g. 800 1200): ";
                cin >> minD >> maxD;
                vector<Problem> prolist = platform.listProblemsByDifficulty(minD, maxD);
                cout << "\n--- PROBLEMS IN DIFFICULTY RANGE " << minD << " - " << maxD << " (AVL Tree) ---\n";
                for (const auto& p : prolist) {
                    cout << "  [ID " << setw(2) << p.id << "] " << setw(28) << left << p.name << " | Diff: " << p.difficulty << "\n";
                }
                cout << "Total problems found: " << prolist.size() << "\n";
                break;
            }
            case 5: {
                cout << "Search [1] Problems or [2] Users? ";
                int t; cin >> t;
                string prefix;
                cout << "Enter prefix (e.g. 'c', 'to', 'w'): ";
                cin >> prefix;
                if (t == 1) {
                    vector<string> results = platform.autocompleteProblemName(prefix, 10);
                    cout << "\n>> Problem Autocomplete for '" << prefix << "' (Trie):\n";
                    for (const auto& s : results) cout << "   - " << s << "\n";
                } else {
                    vector<string> results = platform.autocompleteUserName(prefix, 10);
                    cout << "\n>> User Autocomplete for '" << prefix << "' (Trie):\n";
                    for (const auto& s : results) cout << "   - " << s << "\n";
                }
                break;
            }
            case 6: {
                cout << "\n--- KMP PLAGIARISM CHECKER ---\n";
                cout << "Checking sample submission against database submissions using KMP...\n";
                Submission sub(99, 1, 100, "Pending",
                    "#include <iostream>\nusing namespace std;\nint main() {\n    int w;\n    cin >> w;\n    if (w > 2 && w % 2 == 0) cout << \"YES\";\n    else cout << \"NO\";\n    return 0;\n}"
                );
                PlagiarismResult pr = platform.checkPlagiarism(sub);
                cout << ">> Result for exact code clone of Problem 1:\n";
                cout << "   Plagiarism Flagged: " << (pr.isPlagiarized ? "YES (PLAGIARISM DETECTED!)" : "NO") << "\n";
                if (pr.isPlagiarized) {
                    cout << "   Matched with User ID: " << pr.matchedSubmission.userId << "\n";
                    cout << "   Matched Chunk (>=30 chars): \"" << pr.matchedChunk << "\"\n";
                }
                break;
            }
            case 7: {
                cout << "Submission Queue Options:\n";
                cout << " [1] Inspect Queue (Binomial Tree)\n";
                cout << " [2] Judge Next Submission\n";
                cout << "Choice: ";
                int qOpt; cin >> qOpt;
                if (qOpt == 1) {
                    platform.getJudgingService().getQueue().printStructure();
                } else {
                    if (!platform.getJudgingService().hasPendingSubmissions()) {
                        cout << "Queue empty! Adding a sample submission...\n";
                        platform.queueSubmission(Submission(1, 1, 15, "Accepted", "codeSample"));
                    }
                    IntegratedSubmitResult isr = platform.judgeNextSubmission();
                    cout << ">> Judged Sub ID " << isr.submission.id 
                         << " | User: " << isr.submission.userId 
                         << " | Verdict: " << (isr.isAccepted ? "Accepted" : "Rejected")
                         << " | New Rank: #" << isr.newRank << "\n";
                }
                break;
            }
            case 8: {
                cout << "\n--- TEAMS LEADERBOARD (DSU) ---\n";
                platform.getJudgingService().printTeamLeaderboard(false);
                cout << "\nMerge two teams? (Enter 2 user IDs, or 0 0 to cancel): ";
                int u1, u2; cin >> u1 >> u2;
                if (u1 > 0 && u2 > 0) {
                    bool ok = platform.mergeTeams(u1, u2);
                    cout << ">> Merge status: " << (ok ? "SUCCESS" : "FAILED") << "\n";
                    if (ok) platform.getJudgingService().printTeamLeaderboard(false);
                }
                break;
            }
            case 9: {
                cout << "Contest Freeze Options:\n";
                cout << " Current Status: " << (platform.isFrozen() ? "FROZEN" : "LIVE") << "\n";
                cout << " [1] Toggle Freeze\n";
                cout << " [2] Reveal Next Pending Update\n";
                cout << " [3] Reveal All Updates\n";
                cout << "Choice: ";
                int fOpt; cin >> fOpt;
                if (fOpt == 1) {
                    platform.toggleFreeze(!platform.isFrozen());
                    cout << ">> Freeze is now: " << (platform.isFrozen() ? "ENABLED" : "DISABLED") << "\n";
                } else if (fOpt == 2) {
                    PendingUpdate pu;
                    if (platform.revealNextUpdate(pu)) {
                        cout << ">> REVEALED: User " << pu.userName << " solved " << pu.problemName << " (+" << pu.deltaScore << " pts)!\n";
                    } else {
                        cout << ">> No pending updates to reveal!\n";
                    }
                } else {
                    auto all = platform.revealAllUpdates();
                    cout << ">> Revealed " << all.size() << " pending update(s)!\n";
                }
                break;
            }
            case 10: {
                cout << "\n--- END-TO-END PIPELINE RUNNER ---\n";
                int uId = 9, pId = 4;
                cout << "Submitting original code for Problem 4 by User " << uId << "...\n";
                Submission newSub(uId, pId, 190, "Accepted", 
                    "#include <iostream>\nusing namespace std;\nint main() { int n, k; cin >> n >> k; cout << 42; return 0; }"
                );
                IntegratedSubmitResult res = platform.processFullSubmission(newSub);
                cout << ">> Pipeline Execution Finished:\n";
                cout << "   - Plagiarism Check: " << (res.isPlagiarized ? "FLAGGED" : "PASSED (Clean)") << "\n";
                cout << "   - Verdict:          " << (res.isAccepted ? "ACCEPTED" : "FAILED") << "\n";
                cout << "   - Points Awarded:   +" << res.pointsAwarded << " pts\n";
                cout << "   - Updated Rank:     #" << res.newRank << " (Treap)\n";
                cout << "   - Team Score:       " << res.newTeamScore << " pts (DSU)\n";
                break;
            }
            case 11: {
                platform.runPerformanceBenchmarks(true);
                break;
            }
            case 0:
                cout << "Exiting CodeRank Platform.\n";
                break;
            default:
                cout << "Invalid choice! Please choose 0-11.\n";
        }
    }

    return 0;
}
