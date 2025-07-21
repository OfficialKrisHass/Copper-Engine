@echo off

set /p version=<%~dp0\VERSION
setx COPPER_VERSION "%version%"

echo.

set PHYSX_PRESET=vc17win64-cpu-only
:: call Copper-Engine/lib/PhysX/physx/generate_projects.bat %PHYSX_PRESET%

echo.

:: Find MSBuild using vswhere
for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
    set "MSBUILD=%%i"
)

if not defined MSBUILD (
    echo MSBuild not found!
    exit /b 1
)

"%MSBUILD%" Copper-Engine/lib/PhysX/physx/compiler/%PHYSX_PRESET%/ALL_BUILD.vcxproj /p:Configuration=checked /p:Platform=x64
"%MSBUILD%" Copper-Engine/lib/PhysX/physx/compiler/%PHYSX_PRESET%/ALL_BUILD.vcxproj /p:Configuration=release /p:Platform=x64

echo.

"%MSBUILD%" Copper-Engine/lib/mono/msvc/libmono-dynamic.vcxproj /p:Configuration=Debug /p:Platform=x64
"%MSBUILD%" Copper-Engine/lib/mono/msvc/libmono-dynamic.vcxproj /p:Configuration=Release /p:Platform=x64

echo.

call scripts/RunPremake.bat