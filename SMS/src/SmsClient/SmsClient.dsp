# Microsoft Developer Studio Project File - Name="SmsClient" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SmsClient - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SmsClient.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SmsClient.mak" CFG="SmsClient - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SmsClient - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SmsClient - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SmsClient - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../obj/SmsClient/Release"
# PROP Intermediate_Dir "../../obj/SmsClient/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib clientsocketlib.lib /nologo /subsystem:windows /machine:I386 /out:"../../bin/Release/SmsExpress.exe" /libpath:"../../lib/release"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SmsClient - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../obj/SmsClient/debug"
# PROP Intermediate_Dir "../../obj/SmsClient/debug"
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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ClientSocketLib.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../../bin/debug/SmsExpress.exe" /pdbtype:sept /libpath:"../../lib/debug"

!ENDIF 

# Begin Target

# Name "SmsClient - Win32 Release"
# Name "SmsClient - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ContactManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DBOperate.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\messageManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ServiceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsBoxTree.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsClient.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsClient.rc
# End Source File
# Begin Source File

SOURCE=.\SmsClientDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsClientView.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsContactTree.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsDetailDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\SmsGPRS\SmsGPRS.rc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\SmsIconTray.cpp
# End Source File
# Begin Source File

SOURCE=..\SmsServer\SmsMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsMessageLib.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\SmsSendMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\userManager.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ContactManager.h
# End Source File
# Begin Source File

SOURCE=.\DBOperate.h
# End Source File
# Begin Source File

SOURCE=.\mainfrm.h
# End Source File
# Begin Source File

SOURCE=.\messageManager.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\ServiceManager.h
# End Source File
# Begin Source File

SOURCE=.\SmsBoxTree.h
# End Source File
# Begin Source File

SOURCE=.\SmsClientDlg.h
# End Source File
# Begin Source File

SOURCE=.\SmsClientview.h
# End Source File
# Begin Source File

SOURCE=.\SmsConfig.h
# End Source File
# Begin Source File

SOURCE=.\SmsContactTree.h
# End Source File
# Begin Source File

SOURCE=.\SmsDetail.h
# End Source File
# Begin Source File

SOURCE=.\SmsIconTray.h
# End Source File
# Begin Source File

SOURCE=.\SmsListView.h
# End Source File
# Begin Source File

SOURCE=.\SmsMessageLib.h
# End Source File
# Begin Source File

SOURCE=.\SmsSearch.h
# End Source File
# Begin Source File

SOURCE=.\SmsSendMessage.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\ToolbarHelper.h
# End Source File
# Begin Source File

SOURCE=.\userManager.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\boxtree.bmp
# End Source File
# Begin Source File

SOURCE=.\res\close.bmp
# End Source File
# Begin Source File

SOURCE=.\res\connecte.ico
# End Source File
# Begin Source File

SOURCE=.\res\connecti.ico
# End Source File
# Begin Source File

SOURCE=.\res\contact.bmp
# End Source File
# Begin Source File

SOURCE=.\res\contacttree.bmp
# End Source File
# Begin Source File

SOURCE=.\res\disconne.ico
# End Source File
# Begin Source File

SOURCE=.\res\exit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\human.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\information.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MessageLib.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MessageState.bmp
# End Source File
# Begin Source File

SOURCE=.\res\msg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\msg.ico
# End Source File
# Begin Source File

SOURCE=.\res\msglib.bmp
# End Source File
# Begin Source File

SOURCE=.\res\msglist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sms_mess.bmp
# End Source File
# Begin Source File

SOURCE=.\res\smsBox.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SmsClient.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\res\SmsClient.ico
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# End Target
# End Project
