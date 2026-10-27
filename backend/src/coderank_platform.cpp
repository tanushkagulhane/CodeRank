// coderank_platform.cpp - Implementation of CodeRank master platform facade
#include "coderank_platform.h"
#include <fstream>
#include <sstream>
#include <iostream>

CodeRankPlatform::CodeRankPlatform(int maxScore, int freezeTimeMin)
    : rankingEngine_(maxScore), judgingService_(freezeTimeMin) {}

bool CodeRankPlatform::initializeFromDatabase(const std::string& dbPath) {
    std::string usersFile = dbPath + "/users.csv";
    std::string problemsFile = dbPath + "/problems.csv";

    // 1. Load problems into Member 2's AVL tree & Trie
    bool pOk = problemService_.loadProblemsFromCSV(problemsFile);
    if (!pOk) {
        // Fallback to local
        pOk = problemService_.loadProblemsFromCSV("problems.csv");
    }

    // 2. Load users into Member 2's User Trie
    bool uOk = problemService_.loadUsersFromCSV(usersFile);
    if (!uOk) {
        uOk = problemService_.loadUsersFromCSV("users.csv");
    }

    // 3. Load users into Member 3's DSU & Freeze engine
    bool jOk = judgingService_.loadUsersFromCSV(usersFile);
    if (!jOk) {
        jOk = judgingService_.loadUsersFromCSV("users.csv");
    }

    // 4. Load users into Member 1's Treap & Segment Tree
    std::ifstream uFile(usersFile);
    if (!uFile.is_open()) {
        uFile.open("users.csv");
    }

    if (uFile.is_open()) {
        std::string line;
        bool isHeader = true;
        while (std::getline(uFile, line)) {
            if (line.empty()) continue;
            if (isHeader) { isHeader = false; continue; }
            std::stringstream ss(line);
            std::string idStr, name, scoreStr, teamStr;
            if (std::getline(ss, idStr, ',') &&
                std::getline(ss, name, ',') &&
                std::getline(ss, scoreStr, ',') &&
                std::getline(ss, teamStr, ',')) {
                User u(std::stoi(idStr), name, std::stoi(scoreStr), std::stoi(teamStr));
                rankingEngine_.addOrUpdate(u);
            }
        }
        uFile.close();
    }

    return pOk && uOk && jOk;
}

void CodeRankPlatform::addOrUpdateUser(const User& user) {
    rankingEngine_.addOrUpdate(user);
    problemService_.addUser(user);
    judgingService_.registerUser(user.id, user.name, user.score, user.teamId);
}

int CodeRankPlatform::getUserRank(int userId) const {
    return rankingEngine_.rank(userId);
}

std::vector<User> CodeRankPlatform::getTopKUsers(int k) const {
    return rankingEngine_.topK(k);
}

int CodeRankPlatform::countUsersInScoreRange(int lo, int hi) const {
    return rankingEngine_.countInRange(lo, hi);
}

int CodeRankPlatform::maxScoreInRange(int lo, int hi) const {
    return rankingEngine_.maxInRange(lo, hi);
}

void CodeRankPlatform::addProblem(const Problem& p) {
    problemService_.addProblem(p);
}

bool CodeRankPlatform::removeProblem(int problemId) {
    return problemService_.removeProblem(problemId);
}

std::vector<Problem> CodeRankPlatform::listProblemsByDifficulty(int minDiff, int maxDiff) {
    return problemService_.listByDifficulty(minDiff, maxDiff);
}

std::vector<std::string> CodeRankPlatform::autocompleteProblemName(const std::string& prefix, int limit) {
    return problemService_.searchByPrefix(prefix, limit);
}

std::vector<std::string> CodeRankPlatform::autocompleteUserName(const std::string& prefix, int limit) {
    return problemService_.searchUserByPrefix(prefix, limit);
}

PlagiarismResult CodeRankPlatform::checkPlagiarism(const Submission& sub) {
    return problemService_.checkSubmission(sub);
}

void CodeRankPlatform::queueSubmission(const Submission& sub) {
    judgingService_.submit(sub);
}

IntegratedSubmitResult CodeRankPlatform::judgeNextSubmission() {
    JudgedResult jr = judgingService_.judgeNext();

    // If accepted and not already added, update Member 1's Treap & SegTree!
    if (jr.pointsAwarded > 0) {
        const User* existing = rankingEngine_.find(jr.submission.userId);
        if (existing) {
            User updated = *existing;
            updated.score += jr.pointsAwarded;
            rankingEngine_.addOrUpdate(updated);
        }
    }

    IntegratedSubmitResult res;
    res.submission = jr.submission;
    res.isPlagiarized = false;
    res.plagiarismDetails = "";
    res.isAccepted = (jr.verdict == "Accepted" || jr.verdict == "AC");
    res.pointsAwarded = jr.pointsAwarded;
    res.newRank = rankingEngine_.rank(jr.submission.userId);
    res.newTeamScore = jr.teamScore;
    res.isFrozen = jr.isFrozen;
    return res;
}

IntegratedSubmitResult CodeRankPlatform::processFullSubmission(const Submission& sub) {
    IntegratedSubmitResult res;
    res.submission = sub;

    // Step 1: Plagiarism check via KMP (Member 2)
    PlagiarismResult pr = problemService_.checkSubmission(sub);
    if (pr.isPlagiarized) {
        res.isPlagiarized = true;
        res.plagiarismDetails = "Flagged: Matched with User " + std::to_string(pr.matchedSubmission.userId) + " (Chunk: " + pr.matchedChunk + ")";
        res.isAccepted = false;
        res.pointsAwarded = 0;
        res.newRank = rankingEngine_.rank(sub.userId);
        res.newTeamScore = judgingService_.getTeamScore(sub.userId);
        res.isFrozen = judgingService_.isFrozen();
        return res;
    }

    // Step 2: Queue submission into Binomial Heap (Member 3)
    judgingService_.submit(sub);

    // Step 3: Judge submission
    return judgeNextSubmission();
}

void CodeRankPlatform::createTeam(const std::string& teamName, const std::vector<int>& memberIds) {
    judgingService_.createTeam(teamName, memberIds);
    for (int m : memberIds) {
        int root = judgingService_.getTeamOfUser(m);
        rankingEngine_.setTeam(m, root);
    }
}

bool CodeRankPlatform::mergeTeams(int teamOrUser1, int teamOrUser2) {
    bool ok = judgingService_.mergeTeams(teamOrUser1, teamOrUser2);
    if (ok) {
        int newRoot = judgingService_.getTeamOfUser(teamOrUser1);
        std::vector<int> members = judgingService_.getTeamMembers(newRoot);
        for (int m : members) {
            rankingEngine_.setTeam(m, newRoot);
        }
    }
    return ok;
}

std::vector<TeamStanding> CodeRankPlatform::getTeamLeaderboard(bool usePublicScores) {
    return judgingService_.getTeamLeaderboard(usePublicScores);
}

void CodeRankPlatform::toggleFreeze(bool enable) {
    if (enable) judgingService_.enableFreeze();
    else judgingService_.disableFreeze();
}

bool CodeRankPlatform::isFrozen() const {
    return judgingService_.isFrozen();
}

bool CodeRankPlatform::revealNextUpdate(PendingUpdate& update) {
    return judgingService_.revealNext(update);
}

std::vector<PendingUpdate> CodeRankPlatform::revealAllUpdates() {
    return judgingService_.revealAll();
}

std::vector<BenchmarkResult> CodeRankPlatform::runPerformanceBenchmarks(bool verbose) {
    return TimingComparison::runAllBenchmarks(verbose);
}
