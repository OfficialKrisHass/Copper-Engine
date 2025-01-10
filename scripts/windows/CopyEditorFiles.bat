@echo off

echo.
echo ---- COPYING EDITOR FILES ----
echo.

set configuration=

call:set_configuration %1

set editor_dir=%~dp0..\..\Copper-Editor
set build_dir=%~dp0..\..\Build\windows-x86_64-%configuration%\Copper-Editor

if "%configuration"=="Release" (
    echo Copying Editor assets
    xcopy %editor_dir%\assets %build_dir%\assets\ /s /e /q /y

    echo.
    echo Copying Editor utils
    xcopy %editor_dir%\util %build_dir%\util\ /s /e /q /y

    echo.
    echo Copying mono files
    xcopy %editor_dir%\lib\mono\lib\windows\mono %build_dir%\lib\mono\ /s /e /q /y

    echo.
    echo Copying misc
    xcopy %editor_dir%\imgui.ini %build_dir%\ /q /y
    echo.
)

echo Copying Editor binaries
xcopy %editor_dir%\lib\PhysX\windows\%configuration% %build_dir%\ /q /y
xcopy %editor_dir%\lib\mono\bin\windows\%configuration%\mono-2.0-sgen.dll %build_dir%\ /q /y

pause
exit /b 0

# Set configuration to, if possible, first argument, otherwise let the user pick
:set_configuration
if not "%1"=="Debug" (
    if not "%1"=="Release" (
        call:pick_configuration
    ) else set configuration=%1
) else set configuration=%1
exit /b 0

:pick_configuration
echo No configuration was provided as an argument, pick one
echo.

echo 1. Debug
echo 2. Release
echo 3. Exit (Default)
set choice=
set /p choice=">"

if "%choice%"=="1" (
    set configuration=Debug
    exit /b 0
)
if "%choice%"=="2" (
    set configuration=Release
    exit /b 0
)
exit 0