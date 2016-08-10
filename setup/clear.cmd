@echo off

echo -------------------------
echo %~dp0
echo -------------------------

call "%~dp0pack.cmd" "false"

rem ----- remove setup -----

if exist "%~dp0setup.exe" (
    del -f "%~dp0setup.exe"
)

rem pause

:exit
exit /b 0
