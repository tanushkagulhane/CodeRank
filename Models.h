#ifndef MODELS_H
#define MODELS_H

#include <string>
struct User {
    int id;
    std::string name;
    int score;
    int teamId;

    User() : id(0), name(""), score(0), teamId(0) {}
    User(int id, std::string name, int score, int teamId)
        : id(id), name(name), score(score), teamId(teamId) {}
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
    int userId;
    int problemId;
    int time;
    std::string verdict;
    std::string code;

    Submission() : userId(0), problemId(0), time(0), verdict(""), code("") {}
    Submission(int userId, int problemId, int time, std::string verdict, std::string code)
        : userId(userId), problemId(problemId), time(time), verdict(verdict), code(code) {}
};

#endif // MODELS_H
