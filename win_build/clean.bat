@echo off

rem Update environment by external script
call env.bat
rem Set current directory to parent
cd ..
rem Execute Makefile with 'clean' target in current directory
mingw32-make clean