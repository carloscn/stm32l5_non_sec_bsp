@echo off
rem ---------------------------------------------------------------------------
rem  Windows flash wrapper for cmd.exe / PowerShell users.
rem  Runs flash.sh through Git Bash (uses STM32_Programmer_CLI by default).
rem
rem  Make sure STM32CubeProgrammer's bin/ is on PATH, or add it to prj.cfg.
rem ---------------------------------------------------------------------------
setlocal
cd /d "%~dp0"

set "BASH=C:\Program Files\Git\bin\bash.exe"
if not exist "%BASH%" set "BASH=C:\Program Files\Git\usr\bin\bash.exe"
if not exist "%BASH%" (
    echo [ERR] Git Bash not found. Install "Git for Windows" or flash from a Git Bash shell.
    exit /b 1
)

"%BASH%" -lc "source ./prj.cfg && exec bash ./flash.sh"
exit /b %ERRORLEVEL%
