@echo off

echo -------------------------
echo %~dp0
echo -------------------------

call "%~dp0pack.cmd" "false"

rem ----- remove wizards -----

if exist "%~dp0packages\com.smartsoft.jframe.wizards\" (
    echo =====
    rd /s /q "%~dp0packages\com.smartsoft.jframe.wizards\"
)
if exist "%~dp0packages\com.smartsoft.jframe.wizards.qtcreator\" (
    rd /s /q "%~dp0packages\com.smartsoft.jframe.wizards.qtcreator\"
)
if exist "%~dp0packages\com.smartsoft.jframe.wizards.visualstudio\" (
    rd /s /q "%~dp0packages\com.smartsoft.jframe.wizards.visualstudio\"
)

rem ----- remove setup -----

if exist "%~dp0setup.exe" (
    del -f "%~dp0setup.exe"
)

rem pause

:exit
exit /b 0
