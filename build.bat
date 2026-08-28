@echo off
rem ---------------------------------------------------------------------------
rem  Windows build wrapper for cmd.exe / PowerShell users.
rem  Reuses the cross-platform "source prj.cfg && make" flow via Git Bash.
rem
rem  Usage:
rem    build.bat              -> make -j8
rem    build.bat clean        -> make clean
rem    build.bat -j20 all     -> passes args straight to make
rem
rem  Requires: Git for Windows (Git Bash) + Arm GNU toolchain in prj.cfg.
rem ---------------------------------------------------------------------------
setlocal
cd /d "%~dp0"

set "BASH=C:\Program Files\Git\bin\bash.exe"
if not exist "%BASH%" set "BASH=C:\Program Files\Git\usr\bin\bash.exe"
if not exist "%BASH%" (
    echo [ERR] Git Bash not found. Install "Git for Windows" or build from a Git Bash shell.
    exit /b 1
)

set "ARGS=%*"
if "%ARGS%"=="" set "ARGS=-j8"

"%BASH%" -lc "source ./prj.cfg && exec make %ARGS%"
exit /b %ERRORLEVEL%
