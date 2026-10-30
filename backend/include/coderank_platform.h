// coderank_platform.h - Integrated Master Facade for CodeRank Platform
// Integrates Member 1 (RankingEngine), Member 2 (ProblemService), and Member 3 (JudgingService)
#pragma once

#include "Models.h"
#include "ranking_engine.h"
#include "ProblemService.h"
#include "judging_service.h"
#include "timing_comparison.h"
#include <string>
#include <vector>
#include <memory>
#include <iostream>

struct IntegratedSubmitResult {
    Submission submission;
    bool isPlagiarized;
    std::string plagiarismDetails;
    bool isAccepted;
    int pointsAwarded;
    int newRank;
    int newTeamScore;
    bool isFrozen;
};

class CodeRankPlatform {
public:
    explicit CodeRankPlatform(int maxScore = 100000, int freezeTimeMin = 240);

    // Initialization & Data Loading
    bool initializeFromDatabase(const std::string& dbPath = "database");

    // Member 1 (Ranking Engine) Facade Accessors
    RankingEngine& getRankingEngine() { return rankingEngine_; }
    void addOrUpdateUser(const User& user);
    int getUserRank(int userId) const;
    std::vector<User> getTopKUsers(int k) const;
    int countUsersInScoreRange(int lo, int hi) const;
    int maxScoreInRange(int lo, int hi) const;

    // Member 2 (Problems & Search) Facade Accessors
    ProblemService& getProblemService() { return problemService_; }
    void addProblem(const Problem& p);
    bool removeProblem(int problemId);
    std::vector<Problem> listProblemsByDifficulty(int minDiff, int maxDiff);
    std::vector<std::string> autocompleteProblemName(const std::string& prefix, int limit = 10);
    std::vector<std::string> autocompleteUserName(const std::string& prefix, int limit = 10);
    PlagiarismResult checkPlagiarism(const Submission& sub);

    // Member 3 (Judging & Teams) Facade Accessors
    JudgingService& getJudgingService() { return judgingService_; }
    void queueSubmission(const Submission& sub);
    IntegratedSubmitResult judgeNextSubmission();
    void createTeam(const std::string& teamName, const std::vector<int>& memberIds);
    bool mergeTeams(int teamOrUser1, int teamOrUser2);
    std::vector<TeamStanding> getTeamLeaderboard(bool usePublicScores = false);
    void toggleFreeze(bool enable);
    bool isFrozen() const;
    bool revealNextUpdate(PendingUpdate& update);
    std::vector<PendingUpdate> revealAllUpdates();

    // End-to-end integrated submission workflow:
    // 1. Checks plagiarism with KMP (Member 2)
    // 2. Queues in Binomial Heap (Member 3)
    // 3. Evaluates & awards points
    // 4. Updates Treap & Segment tree ranking (Member 1)
    // 5. Updates DSU team scores (Member 3)
    // 6. Defers or updates public scoreboard based on freeze state (Member 3)
    IntegratedSubmitResult processFullSubmission(const Submission& sub);

    // Performance Benchmarking
    std::vector<BenchmarkResult> runPerformanceBenchmarks(bool verbose = true);

private:
    RankingEngine rankingEngine_;
    ProblemService problemService_;
    JudgingService judgingService_;
};
