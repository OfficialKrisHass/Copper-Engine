!include "MUI.nsh"

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

Section

    SetOutPath $INSTDIR

    File /r /x *.pdb /x Intermediate "..\Build\windows-x86_64-Release\Copper-Editor"
    File /r /x *.pdb /x Intermediate "..\Build\windows-x86_64-Release\Copper-Launcher"

    CreateShortCut "$SMPROGRAMS\Copper-Launcher.lnk" "$INSTDIR\Copper-Launcher\Copper-Launcher.exe"
    CreateShortCut "$SMPROGRAMS\Copper-Editor.lnk" "$INSTDIR\Copper-Editor\Copper-Editor.exe"

    CreateDirectory "$APPDATA\Copper-Engine"
    FileOpen $0 "$APPDATA\Copper-Engine\LauncherData.cup" w

    FileWrite $0 "Editor Path: $INSTDIR\Copper-Editor\Copper-Editor.exe$\r$\n"
    FileWrite $0 "Project Entries: []$\r$\n"

    FileClose $0

    WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

Section "Uninstall"

    RMDir /r "$INSTDIR\Copper-Editor"
    RMDir /r "$INSTDIR\Copper-Launcher"

    RMDIR /r "$APPDATA\Copper-Engine"

    Delete "$INSTDIR\uninstall.exe"

    RMDir "$INSTDIR"

SectionEnd