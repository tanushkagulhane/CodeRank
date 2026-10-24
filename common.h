// common.h - shared classes agreed by all three members (day-1 meeting)
#pragma once
#include <string>

struct User {
    int id = 0;
    std::string name;
    int score = 0;
    int team = -1;   // -1 = no team
};

struct Problem {
    int id = 0;
    std::string name;
    int difficulty = 0;
};

struct Submission {
    int user = 0;
    int problem = 0;
    int time = 0;          // contest time in minutes
    std::string verdict;   // "AC", "WA", "PENDING", ...
};
