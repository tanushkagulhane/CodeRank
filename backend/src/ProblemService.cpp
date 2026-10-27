#include "ProblemService.h"
#include <fstream>
#include <sstream>
#include <iostream>

ProblemService::ProblemService() {}

void ProblemService::addProblem(const Problem& p) {
    problemTree.insert(p);
    problemTrie.insert(p.name);
    problemMap[p.id] = p;
}

bool ProblemService::removeProblem(int problemId) {
    auto it = problemMap.find(problemId);
    if (it == problemMap.end()) {
        return false;
    }

    Problem target = it->second;
    problemTree.deleteProblem(target);
    problemTrie.deleteWord(target.name);
    problemMap.erase(it);
    return true;
}

std::vector<std::string> ProblemService::searchByPrefix(const std::string& prefix, int maxLimit) {
    return problemTrie.getAllWithPrefix(prefix, maxLimit);
}

std::vector<Problem> ProblemService::listByDifficulty(int minDifficulty, int maxDifficulty) {
    return problemTree.listInRange(minDifficulty, maxDifficulty);
}

PlagiarismResult ProblemService::checkSubmission(const Submission& sub) {
    return plagiarismChecker.checkPlagiarism(sub, storedSubmissions);
}

void ProblemService::addStoredSubmission(const Submission& sub) {
    storedSubmissions.push_back(sub);
}
void ProblemService::addUser(const User& u) {
    userTrie.insert(u.name);
}

std::vector<std::string> ProblemService::searchUserByPrefix(const std::string& prefix, int maxLimit) {
    return userTrie.getAllWithPrefix(prefix, maxLimit);
}
std::vector<Problem> ProblemService::listAllProblems() {
    return problemTree.listAll();
}
static std::string trimString(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

bool ProblemService::loadProblemsFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    std::string line;
    bool isHeader = true;

    while (std::getline(file, line)) {
        line = trimString(line);
        if (line.empty()) continue;

        if (isHeader) {
            if (line.rfind("id", 0) == 0 || line.rfind("ID", 0) == 0) {
                isHeader = false;
                continue;
            }
            isHeader = false;
        }

        std::stringstream ss(line);
        std::string idStr, name, diffStr;

        if (std::getline(ss, idStr, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, diffStr, ',')) {

            try {
                int id = std::stoi(trimString(idStr));
                name = trimString(name);
                int diff = std::stoi(trimString(diffStr));
                addProblem(Problem(id, name, diff));
            } catch (...) {
                continue;
            }
        }
    }

    file.close();
    return true;
}

bool ProblemService::loadUsersFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return false;
    }

    std::string line;
    bool isHeader = true;

    while (std::getline(file, line)) {
        line = trimString(line);
        if (line.empty()) continue;

        if (isHeader) {
            if (line.rfind("id", 0) == 0 || line.rfind("ID", 0) == 0) {
                isHeader = false;
                continue;
            }
            isHeader = false;
        }

        std::stringstream ss(line);
        std::string idStr, name, scoreStr, teamStr;

        if (std::getline(ss, idStr, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, scoreStr, ',') &&
            std::getline(ss, teamStr, ',')) {

            try {
                int id = std::stoi(trimString(idStr));
                name = trimString(name);
                int score = std::stoi(trimString(scoreStr));
                int team = std::stoi(trimString(teamStr));
                addUser(User(id, name, score, team));
            } catch (...) {
                continue;
            }
        }
    }

    file.close();
    return true;
}
