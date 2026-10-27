// judging_service.h - Facade for Member 3 (Judging & Teams)
// Member 3 - Advanced Data Structures
//
// Unifies:
// - BinomialHeap (Submission Queue, extract-min, queue merge)
// - DisjointSetUnion (Teams, path compression, union by rank, sum of member scores)
// - FreezeRevealEngine (ICPC-style contest freeze and reveal)
// - TeamLeaderboard (Standings calculation)
#pragma once

#include "Models.h"
#include "binomial_heap.h"
#include "dsu.h"
#include "freeze_reveal.h"
#include "team_leaderboard.h"
#include <string>
#include <vector>
#include <unordered_map>

struct JudgedResult {
    Submission submission;
    std::string verdict;
    int pointsAwarded;
    bool isFrozen;
    int oldScore;
    int newScore;
    int teamId;
    int teamScore;
};

class JudgingService {
public:
    explicit JudgingService(int freezeTimeMinutes = 240);

    // -------------------------------------------------------------
    // Submission Queue Methods (Binomial Heap)
    // -------------------------------------------------------------
    void submit(const Submission& sub);
    bool hasPendingSubmissions() const;
    int getQueueSize() const;
    Submission peekNextSubmission() const;
    void setQueuePriorityMode(QueuePriorityMode mode);
    QueuePriorityMode getQueuePriorityMode() const;
    void mergeQueues(JudgingService& other);
    BinomialHeap& getQueue() { return submissionQueue_; }

    // -------------------------------------------------------------
    // Judging Methods
    // -------------------------------------------------------------
    // Judges next submission in queue. If overrideVerdict is empty, defaults to "AC".
    JudgedResult judgeNext(const std::string& overrideVerdict = "", int pointsAwarded = -1);
    const std::vector<Submission>& getJudgedHistory() const;

    // -------------------------------------------------------------
    // Team Management Methods (Disjoint Set Union)
    // -------------------------------------------------------------
    void registerUser(int userId, const std::string& userName, int initialScore = 0, int initialTeamId = -1);
    void createTeam(const std::string& teamName, const std::vector<int>& memberIds);
    bool mergeTeams(int userOrTeam1, int userOrTeam2);
    int getTeamOfUser(int userId);
    int getTeamScore(int userOrTeamId);
    std::vector<int> getTeamMembers(int userOrTeamId);
    std::string getTeamName(int userOrTeamId);
    std::vector<TeamStanding> getTeamLeaderboard(bool usePublicScores = false);
    void printTeamLeaderboard(bool usePublicScores = false);
    DisjointSetUnion& getDSU() { return teamDsu_; }

    // -------------------------------------------------------------
    // Contest Freeze & Reveal Methods
    // -------------------------------------------------------------
    void enableFreeze();
    void disableFreeze();
    bool isFrozen() const;
    void setFreezeThreshold(int minutes);
    int getPendingRevealCount() const;
    bool revealNext(PendingUpdate& outUpdate);
    std::vector<PendingUpdate> revealAll();
    FreezeRevealEngine& getFreezeEngine() { return freezeEngine_; }

    // -------------------------------------------------------------
    // Utility & Data Loading
    // -------------------------------------------------------------
    bool loadUsersFromCSV(const std::string& filename);
    std::string getUserName(int userId) const;
    int getUserScore(int userId) const;
    int getNextSubmissionId();

private:
    BinomialHeap submissionQueue_;
    DisjointSetUnion teamDsu_;
    FreezeRevealEngine freezeEngine_;

    int nextSubmissionId_;
    std::unordered_map<int, User> usersMap_;
    std::unordered_map<int, std::string> problemNames_;
    std::vector<Submission> judgedHistory_;
};
