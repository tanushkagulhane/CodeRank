
// dsu.h - Disjoint Set Union (DSU) for Teams
// Member 3 (Judging & Teams) - Advanced Data Structures
//
// Features:
// - Path compression on find: O(alpha(n)) amortized
// - Union by rank: keeps tree shallow
// - Maintains team score as the sum of member scores
// - Tracks member lists and team metadata
#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iostream>

struct TeamInfo {
    int rootId;
    std::string name;
    int totalScore;
    std::vector<int> memberIds;

    TeamInfo() : rootId(-1), name(""), totalScore(0) {}
    TeamInfo(int id, const std::string& n, int score, const std::vector<int>& members)
        : rootId(id), name(n), totalScore(score), memberIds(members) {}
};

class DisjointSetUnion {
public:
    DisjointSetUnion() = default;

    // Register a user into their own independent set (or initial team)
    void makeSet(int userId, const std::string& teamName = "", int initialScore = 0) {
        if (parent_.find(userId) != parent_.end()) {
            return; // Already registered
        }
        parent_[userId] = userId;
        rank_[userId] = 0;
        teamScore_[userId] = initialScore;
        userScore_[userId] = initialScore;
        teamName_[userId] = teamName.empty() ? ("Team_" + std::to_string(userId)) : teamName;
        teamMembers_[userId] = {userId};
    }

    // Find representative with path compression: O(alpha(n))
    int find(int u) {
        auto it = parent_.find(u);
        if (it == parent_.end()) {
            return -1; // Not found
        }
        if (it->second != u) {
            it->second = find(it->second); // Path compression
        }
        return it->second;
    }

    // Const version of find (without path compression) for read-only queries
    int findConst(int u) const {
        auto it = parent_.find(u);
        if (it == parent_.end()) return -1;
        int curr = u;
        while (parent_.at(curr) != curr) {
            curr = parent_.at(curr);
        }
        return curr;
    }

    // Check if two users belong to the same team
    bool areInSameTeam(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU == -1 || rootV == -1) return false;
        return rootU == rootV;
    }

    // Union two teams using Union by Rank: O(alpha(n))
    // Team score is the sum of member scores
    bool unionSets(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);

        if (rootU == -1 || rootV == -1 || rootU == rootV) {
            return false; // Invalid or already in same team
        }

        // Union by rank: attach smaller rank tree under larger rank tree
        if (rank_[rootU] < rank_[rootV]) {
            std::swap(rootU, rootV);
        }

        parent_[rootV] = rootU;
        if (rank_[rootU] == rank_[rootV]) {
            rank_[rootU]++;
        }

        // Combine team scores: Team score is the sum of member scores
        teamScore_[rootU] += teamScore_[rootV];

        // Merge member lists
        teamMembers_[rootU].insert(teamMembers_[rootU].end(),
                                   teamMembers_[rootV].begin(),
                                   teamMembers_[rootV].end());
        teamMembers_.erase(rootV);

        // Keep rootU's name or merge descriptive names if desired
        if (teamName_[rootU].find(teamName_[rootV]) == std::string::npos &&
            teamName_[rootU].length() + teamName_[rootV].length() < 40) {
            teamName_[rootU] += "+" + teamName_[rootV];
        }

        return true;
    }

    // Update individual user score and dynamically reflect in total team score
    void updateUserScore(int userId, int deltaScore) {
        if (parent_.find(userId) == parent_.end()) {
            makeSet(userId, "", deltaScore);
            return;
        }
        userScore_[userId] += deltaScore;
        int root = find(userId);
        if (root != -1) {
            teamScore_[root] += deltaScore;
        }
    }

    // Set individual user score directly
    void setUserScore(int userId, int newScore) {
        if (parent_.find(userId) == parent_.end()) {
            makeSet(userId, "", newScore);
            return;
        }
        int oldScore = userScore_[userId];
        int delta = newScore - oldScore;
        updateUserScore(userId, delta);
    }

    // Get total score of a team: sum of all member scores
    int getTeamScore(int userOrRootId) {
        int root = find(userOrRootId);
        if (root == -1) return 0;
        return teamScore_[root];
    }

    // Get all user IDs belonging to the team
    std::vector<int> getTeamMembers(int userOrRootId) {
        int root = find(userOrRootId);
        if (root == -1) return {};
        return teamMembers_[root];
    }

    // Get team name
    std::string getTeamName(int userOrRootId) {
        int root = find(userOrRootId);
        if (root == -1) return "Unknown";
        return teamName_[root];
    }

    // Set custom team name
    void setTeamName(int userOrRootId, const std::string& name) {
        int root = find(userOrRootId);
        if (root != -1) {
            teamName_[root] = name;
        }
    }

    // Return all distinct active teams
    std::vector<TeamInfo> getAllTeams() {
        std::vector<TeamInfo> teams;
        for (const auto& kv : teamMembers_) {
            int root = kv.first;
            teams.emplace_back(root, teamName_[root], teamScore_[root], kv.second);
        }
        return teams;
    }

    // Number of distinct teams
    int getTeamCount() const {
        return static_cast<int>(teamMembers_.size());
    }

    // Total registered users
    int getUserCount() const {
        return static_cast<int>(parent_.size());
    }

    // Get rank of a node (internal tree height for testing union by rank)
    int getRank(int u) const {
        auto it = rank_.find(u);
        return it != rank_.end() ? it->second : -1;
    }

    // Get parent pointer directly (for path compression testing)
    int getParent(int u) const {
        auto it = parent_.find(u);
        return it != parent_.end() ? it->second : -1;
    }

private:
    std::unordered_map<int, int> parent_;             // user -> parent
    std::unordered_map<int, int> rank_;               // root -> rank
    std::unordered_map<int, int> teamScore_;          // root -> total team score
    std::unordered_map<int, int> userScore_;          // user -> individual score
    std::unordered_map<int, std::string> teamName_;   // root -> team name
    std::unordered_map<int, std::vector<int>> teamMembers_; // root -> list of member user IDs
};
