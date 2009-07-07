;NSIS Modern User Interface
;Derived from the Welcome/Finish Page Example Script
;Originally written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;General

  ;Name and file
  Name "molsKetch"
  OutFile "install.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\molsKetch"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\molsKetch" ""

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "./License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Program files" basic

  SetOutPath "$INSTDIR"

  File "mingwm10.dll"
  File "libopenbabel-2.dll"
  File "libinchi-0.dll"
  File "QtGUI4.dll"
  File "QtAssistantCLient4.dll"
  File "QtCore4.dll"
  File "QtNetwork4.dll"
  File "QtXml4.dll"
  File "assistant.exe"
  File "molsKetch.exe"
  File "molsKetch.ico"

  File /r "doc"
  File /r "library"
      
  ;Store installation folder
  WriteRegStr HKCU "Software\molsKetch" "Install_Dir" $INSTDIR
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\molsKetch" "DisplayName" "molsKetch"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\molsKetch" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\molsKetch" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\molsKetch" "NoRepair" 1

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd


; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\molsKetch"
    CreateShortCut "$SMPROGRAMS\molsKetch\molsKetch.lnk" "$INSTDIR\molsKetch.exe" "" "$INSTDIR\molsKetch.ico" 0
	CreateShortCut "$SMPROGRAMS\molsKetch\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0

  
SectionEnd


;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_molsKetch ${LANG_ENGLISH} "The basic installation"

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${basic} $(DESC_molsKetch)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\libopenbabel-2.dll"
  Delete "$INSTDIR\libinchi-0.dll"
  Delete "$INSTDIR\QtGUI4.dll"
  Delete "$INSTDIR\QtAssistantCLient4.dll"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\QtXml4.dll"
  Delete "$INSTDIR\assistant.exe"
  Delete "$INSTDIR\molsKetch.exe"
  Delete "$INSTDIR\molsKetch.ico"

  Delete "$INSTDIR\Uninstall.exe"
  
  Delete "$INSTDIR\library\custom\*.*"
  Delete "$INSTDIR\library\*.*"
  Delete "$INSTDIR\doc\en\*.*"
  Delete "$INSTDIR\doc\nl\*.*"
  Delete "$INSTDIR\doc\*.*"
  
  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\molsKetch\*.*"
  
  ; Remove directories used
  RMDir "$INSTDIR\library\custom"
  RMDir "$INSTDIR\library"
  RMDir "$INSTDIR\doc\en"
  RMDir "$INSTDIR\doc\nl"
  RMDir "$INSTDIR\doc"
  RMDir "$SMPROGRAMS\molsKetch"
  RMDir "$INSTDIR"
    
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\molsKetch"
  DeleteRegKey HKCU "Software\molsKetch"

SectionEnd
