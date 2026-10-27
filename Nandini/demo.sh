#!/usr/bin/env bash
# ======================================================================
#   CODERANK - MEMBER 3 (NANDINI): BUILD AND DEMO SCRIPT
#   Judging & Teams Module: Binomial Heap, DSU, Freeze/Reveal, Benchmarks
# ======================================================================

set -e

echo "[1/3] Compiling Member 3 Test Suite..."
g++ -std=c++14 -O2 judging_service.cpp timing_comparison.cpp test_judging_teams.cpp -o test_judging_teams
echo "[SUCCESS] Test suite compiled!"

echo ""
echo "[2/3] Compiling Member 3 End-to-End Demo Runner..."
g++ -std=c++14 -O2 judging_service.cpp timing_comparison.cpp run_demo.cpp -o run_demo
echo "[SUCCESS] Demo runner compiled!"

echo ""
echo "======================================================================"
echo "  RUNNING MEMBER 3 UNIT & INTEGRATION TESTS (ALL 5 SECTIONS)"
echo "======================================================================"
./test_judging_teams

echo ""
echo "======================================================================"
echo "  LAUNCHING END-TO-END DEMO SCRIPT WITH BENCHMARKING"
echo "======================================================================"
./run_demo
