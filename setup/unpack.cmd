@echo off

echo ----------------------------------------
echo %~dp0
echo ----------------------------------------

call "%~dp0pack.cmd" "false"

rem ----- remove setup.exe -----

if exists "%~dp0setup.exe" (
    del "%~dp0setup.exe"
)

rem pause

:exit
exit /b 0
