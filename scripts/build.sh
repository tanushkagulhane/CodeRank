#!/usr/bin/env bash
# =====================================================================
#   CODERANK - MASTER BUILD SCRIPT (LINUX / MACOS)
# =====================================================================
set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd "$DIR/.."

echo "[1/5] Compiling Master Platform Executable..."
g++ -std=c++14 -O2 -I backend/include \
    backend/src/AVLTree.cpp \
    backend/src/Trie.cpp \
    backend/src/KMP.cpp \
    backend/src/ProblemService.cpp \
    backend/src/judging_service.cpp \
    backend/src/timing_comparison.cpp \
    backend/src/coderank_platform.cpp \
    backend/src/main.cpp \
    -o coderank.out
echo "[SUCCESS] coderank.out generated!"

echo ""
echo "[2/5] Compiling Master Integration Test Suite..."
g++ -std=c++14 -O2 -I backend/include \
    backend/src/AVLTree.cpp \
    backend/src/Trie.cpp \
    backend/src/KMP.cpp \
    backend/src/ProblemService.cpp \
    backend/src/judging_service.cpp \
    backend/src/timing_comparison.cpp \
    backend/src/coderank_platform.cpp \
    tests/test_integrated_platform.cpp \
    -o test_integrated.out
echo "[SUCCESS] test_integrated.out generated!"

echo ""
echo "[3/5] Compiling Member 1 Ranking Engine Test..."
g++ -std=c++14 -O2 -I backend/include tests/test_ranking.cpp -o test_ranking.out
echo "[SUCCESS] test_ranking.out generated!"

echo ""
echo "[4/5] Compiling Member 2 Problems & Search Test..."
g++ -std=c++14 -O2 -I backend/include \
    backend/src/AVLTree.cpp \
    backend/src/Trie.cpp \
    backend/src/KMP.cpp \
    backend/src/ProblemService.cpp \
    tests/test_problems_search.cpp \
    -o test_problems_search.out
echo "[SUCCESS] test_problems_search.out generated!"

echo ""
echo "[5/5] Compiling Member 3 Judging & Teams Test..."
g++ -std=c++14 -O2 -I backend/include \
    backend/src/judging_service.cpp \
    tests/test_judging_teams.cpp \
    -o test_judging_teams.out
echo "[SUCCESS] test_judging_teams.out generated!"

echo ""
echo "====================================================================="
echo "  ALL TARGETS COMPILED WITH 100% SUCCESS!"
echo "====================================================================="
