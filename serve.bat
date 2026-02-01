@echo off
REM Start a local web server for testing
echo Starting local web server...
echo Open http://localhost:3000/web/ in your browser
echo Press Ctrl+C to stop the server
python -m http.server 3000
