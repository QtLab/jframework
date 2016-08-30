@echo off

echo ---------------------------------------------------------
echo %~dp0
echo ---------------------------------------------------------

rem ----- 设置变量 -----

% 框架路径 %
set jframe_dir=%1
% smartkits路径 %
set smartkits_dir=%2
% 打包/清除标志 %
set pack_flag=%3
% Qt路径 %
set qt_dir=%4

rem ----- xxx -----

if %pack_flag% == "true" goto pack
if %pack_flag% == "false" goto clear

rem ----- xxx -----

:pack

xcopy "%jframe_dir%\include\*.*" "%~dp0..\data\jframe\include\" /s /y
xcopy "%jframe_dir%\lib\*.lib" "%~dp0..\data\jframe\lib\" /s /y
xcopy "%jframe_dir%\lib\*.pdb" "%~dp0..\data\jframe\lib\" /s /y

goto exit

:clear

if exist "%~dp0..\data\jframe\" (
    rd /s /q "%~dp0..\data\jframe\"
)

goto exit

:exit
exit /b 0
