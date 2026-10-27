// freeze_reveal.h - Freeze and Reveal logic for Contest Leaderboards
// Member 3 (Judging & Teams) - Advanced Data Structures
//
// In competitive programming contests (e.g., ICPC), during the freeze period
// (typically the final hour), public leaderboard updates are withheld.
// Pending updates are placed in a FIFO queue.
// When the contest concludes, the reveal command applies them sequentially
// (or in bulk) to reveal the final true standings.
#pragma once

#include "Models.h"
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <iostream>
#include <iomanip>

struct PendingUpdate {
    int updateId;
    int userId;
    std::string userName;
    int problemId;
    std::string problemName;
    int deltaScore;
    int submissionTime;
    std::string verdict;
    int oldPublicScore;
    int newPublicScore;
    bool applied;

    PendingUpdate()
        : updateId(0), userId(0), userName(""), problemId(0), problemName(""),
          deltaScore(0), submissionTime(0), verdict(""), oldPublicScore(0),
          newPublicScore(0), applied(false) {}

    PendingUpdate(int uId, int usrId, const std::string& uName, int pId,
                  const std::string& pName, int delta, int subTime,
                  const std::string& v, int oldScore, int newScore)
        : updateId(uId), userId(usrId), userName(uName), problemId(pId),
          problemName(pName), deltaScore(delta), submissionTime(subTime),
          verdict(v), oldPublicScore(oldScore), newPublicScore(newScore),
          applied(false) {}
};

class FreezeRevealEngine {
public:
    explicit FreezeRevealEngine(int freezeTimeMinutes = 240)
        : isFrozen_(false), freezeThresholdTime_(freezeTimeMinutes), nextUpdateId_(1) {}

    // Enable contest freeze mode
    void enableFreeze() {
        isFrozen_ = true;
    }

    // Disable freeze mode
    void disableFreeze() {
        isFrozen_ = false;
    }

    bool isFrozen() const {
        return isFrozen_;
    }

    void setFreezeThreshold(int minutes) {
        freezeThresholdTime_ = minutes;
    }

    int getFreezeThreshold() const {
        return freezeThresholdTime_;
    }

    // Check if a submission at contest time should be frozen automatically
    bool shouldFreeze(int submissionTime) const {
        return isFrozen_ || (submissionTime >= freezeThresholdTime_);
    }

    // Register a user's initial score
    void registerUser(int userId, int initialScore) {
        if (realScores_.find(userId) == realScores_.end()) {
            realScores_[userId] = initialScore;
            publicScores_[userId] = initialScore;
            pendingSubmissionCount_[userId] = 0;
        }
    }

    // Process a judged submission:
    // If frozen: updates real internal score, queues a pending update, public score remains unchanged.
    // If not frozen: immediately applies delta to both public and real scores.
    PendingUpdate processJudgedSubmission(int userId, const std::string& userName,
                                         int problemId, const std::string& problemName,
                                         int deltaScore, int submissionTime,
                                         const std::string& verdict) {
        registerUser(userId, 0);

        int oldPub = publicScores_[userId];
        int oldReal = realScores_[userId];
        realScores_[userId] += deltaScore;

        bool frozenNow = shouldFreeze(submissionTime);

        PendingUpdate update(nextUpdateId_++, userId, userName, problemId, problemName,
                             deltaScore, submissionTime, verdict, oldPub,
                             oldPub + deltaScore);

        if (frozenNow) {
            // Keep public score frozen!
            pendingSubmissionCount_[userId]++;
            pendingQueue_.push_back(update);
        } else {
            // Apply immediately to public leaderboard
            publicScores_[userId] += deltaScore;
            update.applied = true;
            appliedHistory_.push_back(update);
        }

        return update;
    }

    // Reveal next pending update in FIFO order
    bool revealNext(PendingUpdate& revealedUpdate) {
        if (pendingQueue_.empty()) {
            return false;
        }

        revealedUpdate = pendingQueue_.front();
        pendingQueue_.pop_front();

        // Apply update to public score
        revealedUpdate.oldPublicScore = publicScores_[revealedUpdate.userId];
        publicScores_[revealedUpdate.userId] += revealedUpdate.deltaScore;
        revealedUpdate.newPublicScore = publicScores_[revealedUpdate.userId];
        revealedUpdate.applied = true;

        if (pendingSubmissionCount_[revealedUpdate.userId] > 0) {
            pendingSubmissionCount_[revealedUpdate.userId]--;
        }

        appliedHistory_.push_back(revealedUpdate);
        return true;
    }

    // Reveal all pending updates at once
    std::vector<PendingUpdate> revealAll() {
        std::vector<PendingUpdate> revealed;
        PendingUpdate u;
        while (revealNext(u)) {
            revealed.push_back(u);
        }
        return revealed;
    }

    // Check if there are unrevealed pending updates
    bool hasPendingUpdates() const {
        return !pendingQueue_.empty();
    }

    int getPendingCount() const {
        return static_cast<int>(pendingQueue_.size());
    }

    int getPendingCountForUser(int userId) const {
        auto it = pendingSubmissionCount_.find(userId);
        return it != pendingSubmissionCount_.end() ? it->second : 0;
    }

    int getPublicScore(int userId) const {
        auto it = publicScores_.find(userId);
        return it != publicScores_.end() ? it->second : 0;
    }

    int getRealScore(int userId) const {
        auto it = realScores_.find(userId);
        return it != realScores_.end() ? it->second : 0;
    }

    std::vector<PendingUpdate> getPendingUpdates() const {
        return std::vector<PendingUpdate>(pendingQueue_.begin(), pendingQueue_.end());
    }

    std::vector<PendingUpdate> getAppliedHistory() const {
        return appliedHistory_;
    }

    void reset() {
        isFrozen_ = false;
        pendingQueue_.clear();
        appliedHistory_.clear();
        publicScores_.clear();
        realScores_.clear();
        pendingSubmissionCount_.clear();
        nextUpdateId_ = 1;
    }

private:
    bool isFrozen_;
    int freezeThresholdTime_;
    int nextUpdateId_;

    std::deque<PendingUpdate> pendingQueue_;
    std::vector<PendingUpdate> appliedHistory_;
    std::unordered_map<int, int> publicScores_;            // Visible to contestants/public
    std::unordered_map<int, int> realScores_;              // True internal backend scores
    std::unordered_map<int, int> pendingSubmissionCount_;  // Count of pending attempts
};
