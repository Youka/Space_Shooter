@echo off

rem Update environment by external script
call env.bat
rem Set current directory to parent
cd ..
rem Execute Makefile with default target (=full build) in current directory
mingw32-make

rem Wait for user input to keep console open for feedback
pause