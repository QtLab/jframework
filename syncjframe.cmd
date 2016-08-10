@echo off

rem -----

set jframe_dir="%~dp0"
set qt_dir="%QTDIR%"

rem -----

copy "%qt_dir%\bin\QtCored?.dll" "%jframe_dir%\bin\"

rem -----

rem copy "%jframe_dir%\lib\jframe\QtMFCMigrationd.dll" "%jframe_dir%\bin\"
rem copy "%jframe_dir%\lib\jframe\QtRibbond.dll" "%jframe_dir%\bin\"

rem -----

if exist "%jframe_dir%\lib\jframe\jframe_factoryd.dll" (
    echo copy jframe_factory
    copy "%jframe_dir%\lib\jframe\jframe_factoryd.dll" "%jframe_dir%\bin\"
    copy "%jframe_dir%\lib\jframe\jframe_factoryd.pdb" "%jframe_dir%\bin\"
)

if exist "%jframe_dir%\lib\jframe\jframe_kerneld.dll" (
    echo copy jframe_kernel
    copy "%jframe_dir%\lib\jframe\jframe_kerneld.dll" "%jframe_dir%\bin\"
    copy "%jframe_dir%\lib\jframe\jframe_kerneld.pdb" "%jframe_dir%\bin\"
)

if exist "%jframe_dir%\lib\jframe\jframe_facaded.dll" (
    echo copy jframe_facade
    copy "%jframe_dir%\lib\jframe\jframe_facaded.dll" "%jframe_dir%\bin\"
    copy "%jframe_dir%\lib\jframe\jframe_facaded.pdb" "%jframe_dir%\bin\"
)

if exist "%jframe_dir%\lib\jframe\jframe_logind.dll" (
    echo copy jframe_login
    copy "%jframe_dir%\lib\jframe\jframe_logind.dll" "%jframe_dir%\bin\"
    copy "%jframe_dir%\lib\jframe\jframe_logind.pdb" "%jframe_dir%\bin\"
)

if exist "%jframe_dir%\lib\jframe\jframe_layoutd.dll" (
    echo copy jframe_layoutd
    copy "%jframe_dir%\lib\jframe\jframe_layoutd.dll" "%jframe_dir%\bin\"
    copy "%jframe_dir%\lib\jframe\jframe_layoutd.pdb" "%jframe_dir%\bin\"
)
