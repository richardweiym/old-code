# Microsoft Developer Studio Project File - Name="SmsServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SmsServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SmsServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SmsServer.mak" CFG="SmsServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SmsServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SmsServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SmsServer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../obj/SmsServer/Release"
# PROP Intermediate_Dir "../../obj/SmsServer/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib gprs.lib  smsRegister.lib /nologo /subsystem:windows /machine:I386 /out:"../../bin/Release/SmsServer.exe" /libpath:"../../lib/release"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "SmsServer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../obj/SmsServer/debug"
# PROP Intermediate_Dir "../../obj/SmsServer/debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib gprs.lib  smsRegister.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../../bin/debug/SmsServer.exe" /pdbtype:sept /libpath:"../../lib/debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "SmsServer - Win32 Release"
# Name "SmsServer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SmsAccountManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsCommunicateBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsDeviceBase.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsGPRSDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsServer.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsServer.rc
# End Source File
# Begin Source File

SOURCE=.\SmsServiceMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsSocketCommunicate.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\SmsAccountManager.h
# End Source File
# Begin Source File

SOURCE=.\SmsCommonFunction.h
# End Source File
# Begin Source File

SOURCE=.\SmsCommunicateAgent.h
# End Source File
# Begin Source File

SOURCE=.\SmsCommunicateBase.h
# End Source File
# Begin Source File

SOURCE=.\SmsConfig.h
# End Source File
# Begin Source File

SOURCE=.\SmsControlDef.h
# End Source File
# Begin Source File

SOURCE=.\SmsDeviceBase.h
# End Source File
# Begin Source File

SOURCE=.\SmsDeviceExport.h
# End Source File
# Begin Source File

SOURCE=.\SmsEventSink.h
# End Source File
# Begin Source File

SOURCE=.\SmsFactory.h
# End Source File
# Begin Source File

SOURCE=.\SmsGPRSDevice.h
# End Source File
# Begin Source File

SOURCE=.\SmsMessage.h
# End Source File
# Begin Source File

SOURCE=.\SmsServiceAgentWnd.h
# End Source File
# Begin Source File

SOURCE=.\SmsServiceMgr.h
# End Source File
# Begin Source File

SOURCE=.\SmsSocket.h
# End Source File
# Begin Source File

SOURCE=.\SmsSocketCommunicate.h
# End Source File
# Begin Source File

SOURCE=.\SmsSocketProtocol.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
