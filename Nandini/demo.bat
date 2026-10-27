@echo off
REM ======================================================================
REM   CODERANK - MEMBER 3 (NANDINI): BUILD AND DEMO SCRIPT
REM   Judged & Teams Module: Binomial Heap, DSU, Freeze/Reveal, Benchmarks
REM ======================================================================

echo [1/3] Compiling Member 3 Test Suite...
g++ -std=c++14 -O2 judging_service.cpp timing_comparison.cpp test_judging_teams.cpp -o test_judging_teams.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Test suite compilation failed!
    exit /b %ERRORLEVEL%
)
echo [SUCCESS] Test suite compiled!

echo.
echo [2/3] Compiling Member 3 End-to-End Demo Runner...
g++ -std=c++14 -O2 judging_service.cpp timing_comparison.cpp run_demo.cpp -o run_demo.exe
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Demo runner compilation failed!
    exit /b %ERRORLEVEL%
)
echo [SUCCESS] Demo runner compiled!

echo.
echo ======================================================================
echo   RUNNING MEMBER 3 UNIT AND INTEGRATION TESTS (ALL 5 SECTIONS)
echo ======================================================================
.\test_judging_teams.exe

echo.
echo ======================================================================
echo   LAUNCHING END-TO-END DEMO SCRIPT WITH BENCHMARKING
echo ======================================================================
.\run_demo.exe

pause
