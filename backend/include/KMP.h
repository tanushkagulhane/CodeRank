#ifndef KMP_H
#define KMP_H

#include "Models.h"
#include <string>
#include <vector>

struct PlagiarismResult {
    bool isPlagiarized;
    Submission matchedSubmission;
    std::string matchedChunk;
    int matchPosition;

    PlagiarismResult() : isPlagiarized(false), matchPosition(-1) {}
    PlagiarismResult(bool plag, Submission sub, std::string chunk, int pos)
        : isPlagiarized(plag), matchedSubmission(sub), matchedChunk(chunk), matchPosition(pos) {}
};

class KMP {
private:
    bool isBoilerplate(const std::string& chunk);

public:
    KMP();

    std::vector<int> buildLPS(const std::string& pattern);

    std::vector<int> search(const std::string& text, const std::string& pattern);
    std::string cleanCode(const std::string& code);

    PlagiarismResult checkPlagiarism(const std::string& newCode, int problemId, const std::vector<Submission>& storedSubmissions);

    PlagiarismResult checkPlagiarism(const Submission& newSub, const std::vector<Submission>& storedSubmissions);
    PlagiarismResult checkPlagiarism(const std::string& newCode, const std::vector<Submission>& storedSubmissions);
};

#endif // KMP_H
