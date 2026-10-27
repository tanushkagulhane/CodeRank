#!/usr/bin/env bash
# =====================================================================
#   CODERANK - TEST RUNNER (LINUX / MACOS)
# =====================================================================
set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd "$DIR/.."

echo "====================================================================="
echo "  RUNNING MASTER INTEGRATION TEST SUITE"
echo "====================================================================="
./test_integrated.out

echo ""
echo "====================================================================="
echo "  RUNNING MEMBER 1 RANKING TEST SUITE"
echo "====================================================================="
./test_ranking.out

echo ""
echo "====================================================================="
echo "  RUNNING MEMBER 2 PROBLEMS & SEARCH TEST SUITE"
echo "====================================================================="
./test_problems_search.out

echo ""
echo "====================================================================="
echo "  RUNNING MEMBER 3 JUDGING & TEAMS TEST SUITE"
echo "====================================================================="
./test_judging_teams.out

echo ""
echo "All test suites completed successfully!"
