
@echo off

echo --- %~dp0 ---

rem -----

set jframe_dir="%~dp0"
set qt_dir="%QTDIR%"
set qt_ver=%1
set debug_suffix=%2

rem -----

if not exist "%jframe_dir%\bin\" (
    mkdir "%jframe_dir%\bin\"
)

if not exist "%jframe_dir%\bin\3rdpart\" (
    mkdir "%jframe_dir%\bin\3rdpart\"
)

if not exist "%jframe_dir%\bin\core\" (
    mkdir "%jframe_dir%\bin\core\"
)

if not exist "%jframe_dir%\bin\3rdpart\Qt\plugins\" (
    mkdir "%jframe_dir%\bin\3rdpart\Qt\plugins\"
)

rem -----

copy "%qt_dir%\bin\Qt?Core%debug_suffix%*.dll" "%jframe_dir%\bin\3rdpart\Qt\"
copy "%qt_dir%\bin\Qt?Gui%debug_suffix%*.dll" "%jframe_dir%\bin\3rdpart\Qt\"
copy "%qt_dir%\bin\Qt?Widgets%debug_suffix%*.dll" "%jframe_dir%\bin\3rdpart\Qt\"
copy "%qt_dir%\bin\Qt?Xml%debug_suffix%*.dll" "%jframe_dir%\bin\3rdpart\Qt\"
copy "%qt_dir%\bin\Qt?Sql%debug_suffix%*.dll" "%jframe_dir%\bin\3rdpart\Qt\"
copy "%qt_dir%\bin\Qt?DBus%debug_suffix%*.dll" "%jframe_dir%\bin\3rdpart\Qt\"
xcopy "%qt_dir%\plugins\iconengines\*.dll" "%jframe_dir%\bin\3rdpart\Qt\plugins\iconengines\" /s /q /y
xcopy "%qt_dir%\plugins\imageformats\*.dll" "%jframe_dir%\bin\3rdpart\Qt\plugins\imageformats\" /s /q /y
xcopy "%qt_dir%\plugins\sqldrivers\*.dll" "%jframe_dir%\bin\3rdpart\Qt\plugins\sqldrivers\" /s /q /y
xcopy "%qt_dir%\plugins\platforms\*.dll" "%jframe_dir%\bin\3rdpart\Qt\plugins\platforms\" /s /q /y

rem ---
if "%qt_ver%" == "5.7.0" (
    copy "%qt_dir%\bin\Qt?DBus%debug_suffix%*.dll" "%jframe_dir%\bin\3rdpart\Qt\"
)

rem -----

copy "%jframe_dir%\lib\3rdpart\jwt%debug_suffix%*.dll" "%jframe_dir%\bin\3rdpart\"
copy "%jframe_dir%\lib\3rdpart\log4cpp%debug_suffix%*.dll" "%jframe_dir%\bin\3rdpart\"
copy "%jframe_dir%\lib\3rdpart\qtribbon%debug_suffix%*.dll" "%jframe_dir%\bin\3rdpart\"
copy "%jframe_dir%\lib\3rdpart\qtwinmigrate%debug_suffix%*.dll" "%jframe_dir%\bin\3rdpart\"
copy "%jframe_dir%\lib\3rdpart\tinyxml%debug_suffix%*.dll" "%jframe_dir%\bin\3rdpart\"

rem -----

if exist "%jframe_dir%\lib\core\jframeworkdir.dll" (
    echo copy jframeworkdir
    copy "%jframe_dir%\lib\core\jframeworkdir.dll" "%jframe_dir%\bin\"
    copy "%jframe_dir%\lib\core\jframeworkdir.pdb" "%jframe_dir%\bin\"
)

if exist "%jframe_dir%\lib\core\jframeworkenv.dll" (
    echo copy jframeworkenv
    copy "%jframe_dir%\lib\core\jframeworkenv.dll" "%jframe_dir%\bin\core\"
    copy "%jframe_dir%\lib\core\jframeworkenv.pdb" "%jframe_dir%\bin\core\"
)

rem -----

if exist "%jframe_dir%\lib\core\jframe_factory%debug_suffix%.dll" (
    echo copy jframe_factory
    copy "%jframe_dir%\lib\core\jframe_factory%debug_suffix%.dll" "%jframe_dir%\bin\core\"
    copy "%jframe_dir%\lib\core\jframe_factory%debug_suffix%.pdb" "%jframe_dir%\bin\core\"
)

if exist "%jframe_dir%\lib\core\jframe_kernel%debug_suffix%.dll" (
    echo copy jframe_kernel
    copy "%jframe_dir%\lib\core\jframe_kernel%debug_suffix%.dll" "%jframe_dir%\bin\core\"
    copy "%jframe_dir%\lib\core\jframe_kernel%debug_suffix%.pdb" "%jframe_dir%\bin\core\"
)

if exist "%jframe_dir%\lib\core\jframe_facade%debug_suffix%.dll" (
    echo copy jframe_facade
    copy "%jframe_dir%\lib\core\jframe_facade%debug_suffix%.dll" "%jframe_dir%\bin\core\"
    copy "%jframe_dir%\lib\core\jframe_facade%debug_suffix%.pdb" "%jframe_dir%\bin\core\"
)
