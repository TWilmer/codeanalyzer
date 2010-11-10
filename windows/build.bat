SET PATH=%PATH%;"C:\Program Files\Windows Installer XML v3.5\bin\"
set DIFXAPPDIR="C:\Program Files\Windows Installer XML v3.5\bin\"
heat.exe dir "./SourceDir" -nologo -cg "CoreFiles" -dr "DESTINATION" -srd -template:fragment -ke -gg -out "./components.wxs"
candle.exe -ext "C:\Program Files\Windows Installer XML v3.5\bin\WixDifxAppExtension.dll"  -nologo -sw1077 "./codeanalyzer.wxs" "./components.wxs" 
light.exe  -ext WixUIExtension   -ext "C:\Program Files\Windows Installer XML v3.5\bin\WixDifxAppExtension.dll" "C:\Program Files\Windows Installer XML v3.5\bin\difxapp_x86.wixlib" -nologo -b "./SourceDir" -cultures:en-us -sice:ICE38 -sice:ICE64 -sice:ICE91 -out "./codeanalyzer.msi" "./codeanalyzer.wixobj" "./components.wixobj" 
rem  "C:\Program Files\Windows Installer XML v3.5\bin\difxapp_x86.wixlib"

rem   -ext "C:\Program Files\Windows Installer XML v3.5\bin\WixDifxAppExtension.dll"
