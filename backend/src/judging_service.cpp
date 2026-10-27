// judging_service.cpp - Implementation of JudgingService facade
#include "judging_service.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

JudgingService::JudgingService(int freezeTimeMinutes)
    : freezeEngine_(freezeTimeMinutes), nextSubmissionId_(1) {}

void JudgingService::submit(const Submission& sub) {
    Submission toInsert = sub;
    if (toInsert.id <= 0) {
        toInsert.id = nextSubmissionId_++;
    } else {
        if (toInsert.id >= nextSubmissionId_) {
            nextSubmissionId_ = toInsert.id + 1;
        }
    }
    // Synchronize aliases
    if (toInsert.user == 0 && toInsert.userId != 0) toInsert.user = toInsert.userId;
    if (toInsert.userId == 0 && toInsert.user != 0) toInsert.userId = toInsert.user;
    if (toInsert.problem == 0 && toInsert.problemId != 0) toInsert.problem = toInsert.problemId;
    if (toInsert.problemId == 0 && toInsert.problem != 0) toInsert.problemId = toInsert.problem;

    submissionQueue_.insert(toInsert);
}

bool JudgingService::hasPendingSubmissions() const {
    return !submissionQueue_.empty();
}

int JudgingService::getQueueSize() const {
    return submissionQueue_.size();
}

Submission JudgingService::peekNextSubmission() const {
    return submissionQueue_.peekMin();
}

void JudgingService::setQueuePriorityMode(QueuePriorityMode mode) {
    submissionQueue_.setPriorityMode(mode);
}

QueuePriorityMode JudgingService::getQueuePriorityMode() const {
    return submissionQueue_.getPriorityMode();
}

void JudgingService::mergeQueues(JudgingService& other) {
    submissionQueue_.merge(other.submissionQueue_);
}

JudgedResult JudgingService::judgeNext(const std::string& overrideVerdict, int pointsAwarded) {
    if (submissionQueue_.empty()) {
        throw std::runtime_error("No submissions in queue to judge!");
    }

    Submission sub = submissionQueue_.extractMin();

    std::string verdict = overrideVerdict;
    if (verdict.empty()) {
        verdict = sub.verdict.empty() ? "Accepted" : sub.verdict;
    }
    sub.verdict = verdict;

    int points = 0;
    if (verdict == "Accepted" || verdict == "AC") {
        points = (pointsAwarded >= 0) ? pointsAwarded : 100;
    }

    int userId = sub.userId;
    registerUser(userId, getUserName(userId), 0);

    int oldUserScore = usersMap_[userId].score;
    usersMap_[userId].score += points;
    int newUserScore = usersMap_[userId].score;

    // Update DSU team score
    teamDsu_.updateUserScore(userId, points);
    int teamId = teamDsu_.find(userId);
    int newTeamScore = teamDsu_.getTeamScore(teamId);

    // Process through Freeze/Reveal engine
    std::string probName = "Problem " + std::to_string(sub.problemId);
    auto itProb = problemNames_.find(sub.problemId);
    if (itProb != problemNames_.end()) {
        probName = itProb->second;
    }

    bool isFrozenNow = freezeEngine_.shouldFreeze(sub.time);
    freezeEngine_.processJudgedSubmission(userId, getUserName(userId),
                                          sub.problemId, probName,
                                          points, sub.time, verdict);

    judgedHistory_.push_back(sub);

    JudgedResult res;
    res.submission = sub;
    res.verdict = verdict;
    res.pointsAwarded = points;
    res.isFrozen = isFrozenNow;
    res.oldScore = oldUserScore;
    res.newScore = newUserScore;
    res.teamId = teamId;
    res.teamScore = newTeamScore;

    return res;
}

const std::vector<Submission>& JudgingService::getJudgedHistory() const {
    return judgedHistory_;
}

void JudgingService::registerUser(int userId, const std::string& userName, int initialScore, int initialTeamId) {
    if (usersMap_.find(userId) == usersMap_.end()) {
        std::string name = userName.empty() ? ("User_" + std::to_string(userId)) : userName;
        usersMap_[userId] = User(userId, name, initialScore, initialTeamId);
        teamDsu_.makeSet(userId, "", initialScore);
        freezeEngine_.registerUser(userId, initialScore);
    }
}

void JudgingService::createTeam(const std::string& teamName, const std::vector<int>& memberIds) {
    if (memberIds.empty()) return;

    int leader = memberIds[0];
    registerUser(leader, getUserName(leader));
    teamDsu_.setTeamName(leader, teamName);

    for (size_t i = 1; i < memberIds.size(); ++i) {
        int m = memberIds[i];
        registerUser(m, getUserName(m));
        teamDsu_.unionSets(leader, m);
    }

    int root = teamDsu_.find(leader);
    teamDsu_.setTeamName(root, teamName);
}

bool JudgingService::mergeTeams(int userOrTeam1, int userOrTeam2) {
    return teamDsu_.unionSets(userOrTeam1, userOrTeam2);
}

int JudgingService::getTeamOfUser(int userId) {
    return teamDsu_.find(userId);
}

int JudgingService::getTeamScore(int userOrTeamId) {
    return teamDsu_.getTeamScore(userOrTeamId);
}

std::vector<int> JudgingService::getTeamMembers(int userOrTeamId) {
    return teamDsu_.getTeamMembers(userOrTeamId);
}

std::string JudgingService::getTeamName(int userOrTeamId) {
    return teamDsu_.getTeamName(userOrTeamId);
}

std::vector<TeamStanding> JudgingService::getTeamLeaderboard(bool usePublicScores) {
    return TeamLeaderboard::getStandings(teamDsu_, &freezeEngine_, usePublicScores);
}

void JudgingService::printTeamLeaderboard(bool usePublicScores) {
    TeamLeaderboard::printLeaderboard(teamDsu_, &freezeEngine_, usePublicScores);
}

void JudgingService::enableFreeze() {
    freezeEngine_.enableFreeze();
}

void JudgingService::disableFreeze() {
    freezeEngine_.disableFreeze();
}

bool JudgingService::isFrozen() const {
    return freezeEngine_.isFrozen();
}

void JudgingService::setFreezeThreshold(int minutes) {
    freezeEngine_.setFreezeThreshold(minutes);
}

int JudgingService::getPendingRevealCount() const {
    return freezeEngine_.getPendingCount();
}

bool JudgingService::revealNext(PendingUpdate& outUpdate) {
    bool ok = freezeEngine_.revealNext(outUpdate);
    return ok;
}

std::vector<PendingUpdate> JudgingService::revealAll() {
    return freezeEngine_.revealAll();
}

bool JudgingService::loadUsersFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    bool isHeader = true;
    std::unordered_map<int, std::vector<int>> teamGroups;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (isHeader) {
            isHeader = false;
            if (line.find("id") != std::string::npos) continue;
        }

        std::stringstream ss(line);
        std::string idStr, name, scoreStr, teamIdStr;

        if (std::getline(ss, idStr, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, scoreStr, ',') &&
            std::getline(ss, teamIdStr, ',')) {

            int id = std::stoi(idStr);
            int score = std::stoi(scoreStr);
            int teamId = std::stoi(teamIdStr);

            registerUser(id, name, score, teamId);

            if (teamId > 0) {
                teamGroups[teamId].push_back(id);
            }
        }
    }
    file.close();

    // Union all users who share the same initial teamId
    for (const auto& kv : teamGroups) {
        int tId = kv.first;
        const auto& members = kv.second;
        if (members.empty()) continue;

        int firstUser = members[0];
        teamDsu_.setTeamName(firstUser, "Team_" + std::to_string(tId));
        for (size_t i = 1; i < members.size(); ++i) {
            teamDsu_.unionSets(firstUser, members[i]);
        }
        int root = teamDsu_.find(firstUser);
        teamDsu_.setTeamName(root, "Team_" + std::to_string(tId));
    }

    return true;
}

std::string JudgingService::getUserName(int userId) const {
    auto it = usersMap_.find(userId);
    if (it != usersMap_.end()) return it->second.name;
    return "User_" + std::to_string(userId);
}

int JudgingService::getUserScore(int userId) const {
    auto it = usersMap_.find(userId);
    if (it != usersMap_.end()) return it->second.score;
    return 0;
}

int JudgingService::getNextSubmissionId() {
    return nextSubmissionId_++;
}
