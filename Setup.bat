@echo off

set /p version=<%~dp0\VERSION
setx COPPER_VERSION "%version%"

echo.

call scripts/RunPremake.bat