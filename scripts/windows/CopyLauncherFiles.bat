@echo off

echo.
echo ---- COPYING LAUNCHER FILES ----
echo.

set configuration=

call:set_configuration %1

set launcher_dir=%~dp0..\..\Copper-Launcher
set build_dir=%~dp0..\..\Build\windows-x86_64-%configuration%\Copper-Launcher

if "%configuration"=="Release" (
    echo Copying Launcher assets
    xcopy %launcher_dir%\assets %build_dir%\assets\ /s /e /q /y

    echo.
    echo Copying etc
    xcopy %launcher_dir%\imgui.ini %build_dir%\ /q /y
)

pause
exit /b 0

:: Set configuration to, if possible, first argument, otherwise let the user pick
:set_configuration
if not "%1"=="Debug" (
    if not "%1"=="Release" (
        call:pick_configuration
    ) else set configuration=%1
) else set configuration=%1
exit /b 0

:: List to pick the configuration
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
