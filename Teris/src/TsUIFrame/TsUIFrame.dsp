# Microsoft Developer Studio Project File - Name="TsUIFrame" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=TsUIFrame - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TsUIFrame.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TsUIFrame.mak" CFG="TsUIFrame - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TsUIFrame - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "TsUIFrame - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TsUIFrame - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../obj/TsUIFrame/Release"
# PROP Intermediate_Dir "../../obj/TsUIFrame/Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../../include/wtl" /I "../../include/HYTeris" /D "WIN32" /D "NDEBUG" /D "UNICODE" /D "_UNICODE" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/Release/TsUIFrame.lib"

!ELSEIF  "$(CFG)" == "TsUIFrame - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../obj/TsUIFrame/Debug"
# PROP Intermediate_Dir "../../obj/TsUIFrame/Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../include/wtl" /I "../../include/HYTeris" /D "WIN32" /D "_DEBUG" /D "UNICODE" /D "_UNICODE" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../lib/Debug/TsUIFrame.lib"

!ENDIF 

# Begin Target

# Name "TsUIFrame - Win32 Release"
# Name "TsUIFrame - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\TsButton.cpp
# End Source File
# Begin Source File

SOURCE=.\TsDataKernel.cpp
# End Source File
# Begin Source File

SOURCE=.\TsDialogBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TsDisplayContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\TsGameContext.cpp
# End Source File
# Begin Source File

SOURCE=.\TsGameMainWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\TsMainButtonPane.cpp
# End Source File
# Begin Source File

SOURCE=.\TsMainControlPane.cpp
# End Source File
# Begin Source File

SOURCE=.\TsMainDisplayPane.cpp
# End Source File
# Begin Source File

SOURCE=.\TsMainStatusPane.cpp
# End Source File
# Begin Source File

SOURCE=.\TsResMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\TsSetupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TsUIDataMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\TsUIInstanceApi.cpp
# End Source File
# Begin Source File

SOURCE=.\TsUIModuleBase.cpp
# End Source File
# Begin Source File

SOURCE=.\TsWebContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\TsWinFrame.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\HYTeris\TsButton.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HYTeris\TsDataKernel.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HYTeris\TsDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HYTeris\TsDialogBase.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HYTeris\TsDisplayContainer.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HYTeris\TsGameContext.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HYTeris\TsGameMainWnd.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HYTeris\TsMainButtonPane.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HYTeris\TsMainControlPane.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HYTeris\TsMainDisplayPane.h
# End Source File
# Begin Source File

SOURCE=.\TsMainStatusPane.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HYTeris\TsResMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HYTeris\TsSetupDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HYTeris\TsUIDataMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HYTeris\TsUIInstanceApi.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HYTeris\TsUIModuleBase.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HYTeris\TsWebContainer.h
# End Source File
# Begin Source File

SOURCE=..\..\include\HYTeris\TsWinFrame.h
# End Source File
# End Group
# End Target
# End Project
