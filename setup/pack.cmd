@echo off

rem ----- 设置变量 -----

% 框架路径 %
cd ..\
set jframe_dir="%cd%\jframe"
cd %~dp0
% smartkits路径 %
set smartkits_dir="%SMARTKITS_DIR%"
% Qt SDK 路径 %
set qt_dir="%QTDIR%"
% pack or unpack %
set "pack_flag=%1"
if not defined pack_flag set pack_flag="true"

rem ----- call package.cmd -----

% ... %
cd %~dp0packages

% ... %
for /f "tokens=*" %%a in ('dir /s/b/a-d "package.cmd"') do (
    call %%a %jframe_dir% %smartkits_dir% %pack_flag% %qt_dir%
)

% ... %
cd %~dp0

rem ----- generate setup -----
if %pack_flag% == "true" (
    binarycreator.exe -c "%~dp0config/config.xml" -p "%~dp0packages" setup.exe

    pause
)

:exit
exit /b 0
