#include "KMP.h"
#include <sstream>

KMP::KMP() {}

bool KMP::isBoilerplate(const std::string& chunk) {
    if (chunk.find("#include") != std::string::npos) return true;
    if (chunk.find("using namespace") != std::string::npos) return true;
    return false;
}

std::vector<int> KMP::buildLPS(const std::string& pattern) {
    int m = (int)pattern.length();
    std::vector<int> lps(m, 0);

    if (m == 0) return lps;

    int len = 0;
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }

    return lps;
}

std::vector<int> KMP::search(const std::string& text, const std::string& pattern) {
    std::vector<int> matches;
    int n = (int)text.length();
    int m = (int)pattern.length();

    if (m == 0 || n < m) return matches;

    std::vector<int> lps = buildLPS(pattern);
    int i = 0;
    int j = 0;

    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == m) {
            matches.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    return matches;
}

std::string KMP::cleanCode(const std::string& code) {
    std::string result = "";
    std::stringstream ss(code);
    std::string line;

    while (std::getline(ss, line)) {
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) continue;
        std::string trimmed = line.substr(start);
        if (trimmed[0] == '#') continue;

        if (trimmed.rfind("using namespace", 0) == 0) continue;
        if (trimmed.rfind("//", 0) == 0) continue;

        bool inSpace = false;
        std::string cleanLine = "";
        for (size_t i = 0; i < trimmed.length(); i++) {
            char c = trimmed[i];
            if (c == '/' && i + 1 < trimmed.length() && trimmed[i + 1] == '/') {
                break;
            }

            if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
                if (!inSpace && !cleanLine.empty()) {
                    cleanLine += ' ';
                    inSpace = true;
                }
            } else {
                cleanLine += c;
                inSpace = false;
            }
        }

        while (!cleanLine.empty() && cleanLine.back() == ' ') {
            cleanLine.pop_back();
        }

        if (!cleanLine.empty()) {
            if (!result.empty()) result += " ";
            result += cleanLine;
        }
    }

    return result;
}

PlagiarismResult KMP::checkPlagiarism(const std::string& newCode, int problemId, const std::vector<Submission>& storedSubmissions) {
    PlagiarismResult result;
    result.isPlagiarized = false;

    std::string cleanNew = cleanCode(newCode);
    if (cleanNew.empty()) return result;

    const int WINDOW_SIZE = 30;

    for (const Submission& oldSub : storedSubmissions) {
        if (problemId != -1 && oldSub.problemId != problemId) {
            continue;
        }

        std::string cleanOld = cleanCode(oldSub.code);
        if (cleanOld.empty()) continue;

        if ((int)cleanNew.length() < WINDOW_SIZE) {
            if (cleanNew.length() >= 10 && !isBoilerplate(cleanNew)) {
                std::vector<int> matches = search(cleanOld, cleanNew);
                if (!matches.empty()) {
                    result.isPlagiarized = true;
                    result.matchedSubmission = oldSub;
                    result.matchedChunk = cleanNew;
                    result.matchPosition = matches[0];
                    return result;
                }
            }
        } else {
            for (size_t i = 0; i + WINDOW_SIZE <= cleanNew.length(); i += 5) {
                std::string chunk = cleanNew.substr(i, WINDOW_SIZE);
                if (isBoilerplate(chunk)) continue;

                std::vector<int> matches = search(cleanOld, chunk);
                if (!matches.empty()) {
                    result.isPlagiarized = true;
                    result.matchedSubmission = oldSub;
                    result.matchedChunk = chunk;
                    result.matchPosition = matches[0];
                    return result;
                }
            }

            std::string lastChunk = cleanNew.substr(cleanNew.length() - WINDOW_SIZE, WINDOW_SIZE);
            if (!isBoilerplate(lastChunk)) {
                std::vector<int> lastMatches = search(cleanOld, lastChunk);
                if (!lastMatches.empty()) {
                    result.isPlagiarized = true;
                    result.matchedSubmission = oldSub;
                    result.matchedChunk = lastChunk;
                    result.matchPosition = lastMatches[0];
                    return result;
                }
            }
        }
    }

    return result;
}

PlagiarismResult KMP::checkPlagiarism(const Submission& newSub, const std::vector<Submission>& storedSubmissions) {
    return checkPlagiarism(newSub.code, newSub.problemId, storedSubmissions);
}

PlagiarismResult KMP::checkPlagiarism(const std::string& newCode, const std::vector<Submission>& storedSubmissions) {
    return checkPlagiarism(newCode, -1, storedSubmissions);
}
