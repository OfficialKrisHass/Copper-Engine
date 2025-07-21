@echo off

if exist %cd%\premake5.lua (
    premake\premake5.exe vs2022
)
if exist %cd%\..\premake5.lua (
    %cd%\..\premake\premake5.exe --file=..\premake5.lua vs2022
)

pause