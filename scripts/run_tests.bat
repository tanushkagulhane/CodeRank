@echo off
REM Runs all test suites for Member 1, Member 2, Member 3 and Integrated Platform
cd ..

echo =====================================================================
echo   RUNNING MASTER INTEGRATION TEST SUITE
echo =====================================================================
.\test_integrated.exe

echo.
echo =====================================================================
echo   RUNNING MEMBER 1 RANKING TEST SUITE
echo =====================================================================
.\test_ranking.exe

echo.
echo =====================================================================
echo   RUNNING MEMBER 2 PROBLEMS & SEARCH TEST SUITE
echo =====================================================================
.\test_problems_search.exe

echo.
echo =====================================================================
echo   RUNNING MEMBER 3 JUDGING & TEAMS TEST SUITE
echo =====================================================================
.\test_judging_teams.exe

pause
