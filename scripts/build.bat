@echo off
REM =====================================================================
REM   CODERANK - MASTER BUILD SCRIPT (BACKUP-MAIN)
REM =====================================================================

cd ..

echo [1/4] Compiling Master Platform Executable...
g++ -std=c++14 -O2 -I backend/include backend/src/AVLTree.cpp backend/src/Trie.cpp backend/src/KMP.cpp backend/src/ProblemService.cpp backend/src/judging_service.cpp backend/src/timing_comparison.cpp backend/src/coderank_platform.cpp backend/src/main.cpp -o coderank.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Master compilation failed!
    exit /b %ERRORLEVEL%
)
echo [SUCCESS] coderank.exe generated!

echo.
echo [2/4] Compiling Master Integration Test Suite...
g++ -std=c++14 -O2 -I backend/include backend/src/AVLTree.cpp backend/src/Trie.cpp backend/src/KMP.cpp backend/src/ProblemService.cpp backend/src/judging_service.cpp backend/src/timing_comparison.cpp backend/src/coderank_platform.cpp tests/test_integrated_platform.cpp -o test_integrated.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Integration test compilation failed!
    exit /b %ERRORLEVEL%
)
echo [SUCCESS] test_integrated.exe generated!

echo.
echo [3/4] Compiling Member 1 Ranking Engine Test...
g++ -std=c++14 -O2 -I backend/include tests/test_ranking.cpp -o test_ranking.exe
echo [SUCCESS] test_ranking.exe generated!

echo.
echo [4/5] Compiling Member 2 Problems and Search Test...
g++ -std=c++14 -O2 -I backend/include backend/src/AVLTree.cpp backend/src/Trie.cpp backend/src/KMP.cpp backend/src/ProblemService.cpp tests/test_problems_search.cpp -o test_problems_search.exe
echo [SUCCESS] test_problems_search.exe generated!

echo.
echo [5/5] Compiling Member 3 Judging and Teams Test...
g++ -std=c++14 -O2 -I backend/include backend/src/judging_service.cpp tests/test_judging_teams.cpp -o test_judging_teams.exe
echo [SUCCESS] test_judging_teams.exe generated!

echo.
echo =====================================================================
echo   ALL TARGETS COMPILED WITH 100%% SUCCESS!
echo =====================================================================
pause
