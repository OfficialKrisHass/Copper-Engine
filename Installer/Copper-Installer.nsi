!include "MUI.nsh"

# Visual studio, or at least the installer, has to be downloaded in order for Copper-Engine to work
# In the future, we should somehow package the installer so that we can run it as well.

!define /file VERSION ..\VERSION

Name "Copper-Engine"
InstallDir "$PROGRAMFILES\Copper-Engine"
OutFile "Copper-Engine_x86_v${VERSION}.exe"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\LICENSE"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

!macro FindVCRedist

    !tempfile _vcredist
    !system '"%PROGRAMFILES(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Redist.14.Latest -find "VC\Redist\MSVC\*\vc_redist.x64.exe" | powershell -Command "$input | Select-Object -First 1" > "${_vcredist}"'

    !define /file VCREDIST_PATH "${_vcredist}"
    !delfile "${_vcredist}"

    !ifndef VCREDIST_PATH
        !error "Visual Studio VC++ Redistributable not found"
    !endif

!macroend

!insertmacro FindVCRedist

Section "VC++ Redist"

    SetOutPath "$INSTDIR\"
    File /oname=vcredist_x64.exe "${VCREDIST_PATH}"
    ExecWait "$INSTDIR\vcredist_x64.exe"
    Delete "$INSTDIR\vcredist_x64.exe"

SectionEnd

Section

    SetOutPath $INSTDIR

    File /r /x *.pdb /x Intermediate /x Temp "..\Build\windows-x86_64-Release\Copper-Editor"
    File /r /x *.pdb /x Intermediate "..\Build\windows-x86_64-Release\Copper-Launcher"

    CreateShortCut "$SMPROGRAMS\Copper-Launcher.lnk" "$INSTDIR\Copper-Launcher\Copper-Launcher.exe"
    CreateShortCut "$SMPROGRAMS\Copper-Editor.lnk" "$INSTDIR\Copper-Editor\Copper-Editor.exe"

    CreateDirectory "$APPDATA\Copper-Editor"

    CreateDirectory "$APPDATA\Copper-Launcher"
    FileOpen $0 "$APPDATA\Copper-Launcher\LauncherData.cup" w

    FileWrite $0 "Editor Path: $INSTDIR\Copper-Editor\Copper-Editor.exe$\r$\n"
    FileWrite $0 "Project Entries: []$\r$\n"

    FileClose $0

    WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

Section "Uninstall"

    RMDir /r "$INSTDIR\Copper-Editor"
    RMDir /r "$INSTDIR\Copper-Launcher"

    RMDir /r "$APPDATA\Copper-Launcher"
    RMDir /r "$APPDATA\Copper-Editor"

    Delete "$INSTDIR\uninstall.exe"

    RMDir "$INSTDIR"

SectionEnd