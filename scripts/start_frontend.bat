@echo off
REM Launches the CodeRank modern frontend with Python HTTP server
cd /d "%~dp0..\frontend"
start http://localhost:8000
echo Starting CodeRank web server on http://localhost:8000 ...
python -m http.server 8000
