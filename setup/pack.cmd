@echo off

echo ---------------------------------------------------------
echo %~dp0
echo ---------------------------------------------------------

rem ----- 设置变量 -----

% 框架路径 %
set "jframe_dir=%JFRAME_DIR%"
% smartkits路径 %
set smartkits_dir="%SMARTKITS_DIR%"
% Qt SDK 路径 %
set qt_dir="%QTDIR%"
% pack or unpack %
set "pack_flag=%1"
if not defined pack_flag set pack_flag="true"

rem ----- check variables -----

if not defined jframe_dir (
    echo jframe_dir is invalid, will quit.
    goto exit
)

if not defined qt_dir (
    echo qt_dir is invalid, will quit.
    goto exit
)

rem ----- copy extern packages -----

% ... copy wizards packages ... %

if %pack_flag% == "true" (
    xcopy "%jframe_dir%\tools\wizards\qtcreator\setup\packages\*" "%~dp0packages\" /s /y
)
rem ----- call package.cmd -----

% ... %
for /f "tokens=*" %%a in ('dir /s/b/a-d "package.cmd"') do (
    call %%a %jframe_dir% %smartkits_dir% %pack_flag% %qt_dir%
)

% ... %

rem ----- generate setup -----
if %pack_flag% == "true" (
    echo ...s
    echo ...
    echo ======================================================
    echo Now it's packing to generate setup.exe. Please wait...
    binarycreator.exe --offline-only -c "%~dp0config/config.xml" -p "%~dp0packages" setup.exe
    echo packing finished.

    pause
)

:exit
exit /b 0
