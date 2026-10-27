// team_leaderboard.h - Team Leaderboard ranking logic
// Member 3 (Judging & Teams) - Advanced Data Structures
//
// Calculates team standings where team score is the sum of member scores.
// Supports both public (frozen) standings and real standings.
#pragma once

#include "dsu.h"
#include "freeze_reveal.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>

struct TeamStanding {
    int rank;
    int teamId;
    std::string teamName;
    int totalScore;
    int memberCount;
    std::vector<int> members;
    int pendingCount;

    TeamStanding()
        : rank(0), teamId(0), teamName(""), totalScore(0), memberCount(0), pendingCount(0) {}
    TeamStanding(int r, int id, const std::string& name, int score,
                 const std::vector<int>& mems, int pending = 0)
        : rank(r), teamId(id), teamName(name), totalScore(score),
          memberCount(static_cast<int>(mems.size())), members(mems), pendingCount(pending) {}
};

class TeamLeaderboard {
public:
    TeamLeaderboard() = default;

    // Generate sorted team standings
    // If freezeEngine is provided and usePublicScores is true, uses frozen public scores
    static std::vector<TeamStanding> getStandings(DisjointSetUnion& dsu,
                                                 const FreezeRevealEngine* freezeEngine = nullptr,
                                                 bool usePublicScores = false) {
        std::vector<TeamInfo> rawTeams = dsu.getAllTeams();
        std::vector<TeamStanding> standings;

        for (const auto& t : rawTeams) {
            int score = t.totalScore;
            int pending = 0;

            if (freezeEngine != nullptr) {
                if (usePublicScores) {
                    // Sum public scores of members
                    score = 0;
                    for (int memberId : t.memberIds) {
                        score += freezeEngine->getPublicScore(memberId);
                        pending += freezeEngine->getPendingCountForUser(memberId);
                    }
                } else {
                    for (int memberId : t.memberIds) {
                        pending += freezeEngine->getPendingCountForUser(memberId);
                    }
                }
            }

            standings.emplace_back(0, t.rootId, t.name, score, t.memberIds, pending);
        }

        // Sort descending by score, tie-break by smaller team ID
        std::sort(standings.begin(), standings.end(), [](const TeamStanding& a, const TeamStanding& b) {
            if (a.totalScore != b.totalScore) return a.totalScore > b.totalScore;
            return a.teamId < b.teamId;
        });

        // Assign 1-based ranks
        for (size_t i = 0; i < standings.size(); ++i) {
            standings[i].rank = static_cast<int>(i + 1);
        }

        return standings;
    }

    // Get rank of a specific team: 1-based rank, or -1 if not found
    static int getTeamRank(int teamId, DisjointSetUnion& dsu,
                           const FreezeRevealEngine* freezeEngine = nullptr,
                           bool usePublicScores = false) {
        std::vector<TeamStanding> standings = getStandings(dsu, freezeEngine, usePublicScores);
        int rootId = dsu.find(teamId);
        for (const auto& s : standings) {
            if (s.teamId == rootId) {
                return s.rank;
            }
        }
        return -1;
    }

    // Get top-K teams
    static std::vector<TeamStanding> getTopK(int k, DisjointSetUnion& dsu,
                                            const FreezeRevealEngine* freezeEngine = nullptr,
                                            bool usePublicScores = false) {
        std::vector<TeamStanding> standings = getStandings(dsu, freezeEngine, usePublicScores);
        if (k < static_cast<int>(standings.size())) {
            standings.resize(k);
        }
        return standings;
    }

    // Display formatted team leaderboard
    static void printLeaderboard(DisjointSetUnion& dsu,
                                 const FreezeRevealEngine* freezeEngine = nullptr,
                                 bool usePublicScores = false,
                                 const std::string& title = "CODERANK TEAM LEADERBOARD") {
        std::vector<TeamStanding> standings = getStandings(dsu, freezeEngine, usePublicScores);

        std::cout << "\n========================================================================================\n";
        std::cout << "  " << title;
        if (freezeEngine && freezeEngine->isFrozen() && usePublicScores) {
            std::cout << " [STATUS: FROZEN - PUBLIC VIEW]";
        } else if (freezeEngine && freezeEngine->isFrozen() && !usePublicScores) {
            std::cout << " [STATUS: REAL INTERNAL VIEW]";
        }
        std::cout << "\n========================================================================================\n";
        std::cout << std::left << std::setw(6)  << "Rank"
                  << std::setw(10) << "Team ID"
                  << std::setw(28) << "Team Name"
                  << std::setw(14) << "Total Score"
                  << std::setw(10) << "Members"
                  << "Pending Updates\n";
        std::cout << "----------------------------------------------------------------------------------------\n";

        if (standings.empty()) {
            std::cout << "  (No teams registered yet)\n";
        } else {
            for (const auto& s : standings) {
                std::cout << std::left << std::setw(6)  << s.rank
                          << std::setw(10) << s.teamId
                          << std::setw(28) << s.teamName
                          << std::setw(14) << s.totalScore
                          << std::setw(10) << s.memberCount;
                if (s.pendingCount > 0) {
                    std::cout << "?" << s.pendingCount << " pending";
                } else {
                    std::cout << "-";
                }
                std::cout << "\n";
            }
        }
        std::cout << "========================================================================================\n";
    }
};
