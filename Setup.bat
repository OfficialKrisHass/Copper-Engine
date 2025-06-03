@echo off

python scripts/extract_binaries.py

set /p version=<%~dp0\VERSION
setx COPPER_VERSION "%version%"

premake\premake5.exe vs2022

PAUSE
