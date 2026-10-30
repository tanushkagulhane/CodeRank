#ifndef MODELS_H
#define MODELS_H

#include <string>

struct User {
    int id;
    std::string name;
    int score;
    int teamId;
    int team; // Alias for teamId for complete compatibility with ranking_engine

    User() : id(0), name(""), score(0), teamId(-1), team(-1) {}
    User(int id, std::string name, int score, int teamId = -1)
        : id(id), name(name), score(score), teamId(teamId), team(teamId) {}
};

struct Problem {
    int id;
    std::string name;
    int difficulty;

    Problem() : id(0), name(""), difficulty(0) {}
    Problem(int id, std::string name, int difficulty)
        : id(id), name(name), difficulty(difficulty) {}
};

struct Submission {
    int id;
    int userId;
    int problemId;
    int user;      // Alias for userId
    int problem;   // Alias for problemId
    int time;      // Contest time in minutes
    std::string verdict; // "AC", "WA", "PENDING", ...
    std::string code;

    Submission() : id(0), userId(0), problemId(0), user(0), problem(0), time(0), verdict(""), code("") {}
    Submission(int userId, int problemId, int time, std::string verdict, std::string code = "", int subId = 0)
        : id(subId), userId(userId), problemId(problemId), user(userId), problem(problemId),
          time(time), verdict(verdict), code(code) {}
};

#endif // MODELS_H
