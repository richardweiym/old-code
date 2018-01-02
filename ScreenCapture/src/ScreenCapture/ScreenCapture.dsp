# Microsoft Developer Studio Project File - Name="ScreenCapture" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ScreenCapture - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ScreenCapture.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ScreenCapture.mak" CFG="ScreenCapture - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ScreenCapture - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ScreenCapture - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ScreenCapture - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../obj/Release/ScreenCapture"
# PROP Intermediate_Dir "../../obj/Release/ScreenCapture"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /O2 /I "../../include/wtl" /I "../../include/HYUtil" /I "../../include/ScreenCapture" /I "../../include/GDIPlus" /I "../../include/waterWave" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "STRICT" /D "_ATL_MIN_CRT" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib HYUtil.lib shlwapi.lib /nologo /subsystem:windows /machine:I386 /out:"../../bin/Release/ScreenCapture.exe" /libpath:"../../lib/Release"

!ELSEIF  "$(CFG)" == "ScreenCapture - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../obj/Debug/ScreenCapture"
# PROP Intermediate_Dir "../../obj/Debug/ScreenCapture"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../include/wtl" /I "../../include/HYUtil" /I "../../include/ScreenCapture" /I "../../include/GDIPlus" /I "../../include/waterWave" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "STRICT" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib HYUtil.lib shlwapi.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../../bin/Debug/ScreenCapture.exe" /pdbtype:sept /libpath:"../../lib/debug"

!ENDIF 

# Begin Target

# Name "ScreenCapture - Win32 Release"
# Name "ScreenCapture - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SCCanvasApp.cpp
# End Source File
# Begin Source File

SOURCE=.\SCCanvasCmdMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\SCCanvasContext.cpp
# End Source File
# Begin Source File

SOURCE=.\SCCanvasControl.cpp
# End Source File
# Begin Source File

SOURCE=.\SCCanvasData.cpp
# End Source File
# Begin Source File

SOURCE=.\SCCanvasDrawDirector.cpp
# End Source File
# Begin Source File

SOURCE=.\SCCanvasDrawer.cpp
# End Source File
# Begin Source File

SOURCE=.\SCCanvasFloatTool.cpp
# End Source File
# Begin Source File

SOURCE=.\SCCanvasGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\SCCanvasSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\SCCanvasStartWin.cpp
# End Source File
# Begin Source File

SOURCE=.\SCCanvasWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\SCDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SCIDImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\SCInstanceAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenCapture.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenCapture.rc
# End Source File
# Begin Source File

SOURCE=.\SCXImage.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCCanvasApp.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCCanvasCmdMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCCanvasConfig.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCCanvasContext.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCCanvasControl.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCCanvasData.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCCanvasDrawDirector.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCCanvasDrawer.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCCanvasFloatTool.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCCanvasGraph.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCCanvasSkin.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCCanvasStartWin.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCCanvasWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCIDDef.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCInstanceAPI.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCRes.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCWinSpy.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ScreenCapture\SCXImage.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ScreenCapture.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\res\ScreenCapture.ico
# End Source File
# End Group
# End Target
# End Project
