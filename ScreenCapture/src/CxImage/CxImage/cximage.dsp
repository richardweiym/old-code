# Microsoft Developer Studio Project File - Name="CxImage" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=CxImage - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cximage.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cximage.mak" CFG="CxImage - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CxImage - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "CxImage - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CxImage - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CxImage___Win32_Debug"
# PROP BASE Intermediate_Dir "CxImage___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../obj/Debug/CxImage"
# PROP Intermediate_Dir "../../../obj/Debug/CxImage"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\jpeg" /I "..\png" /I "..\zlib" /I "..\mng" /I "..\tiff" /I "..\j2k" /I "..\jasper\include" /D "_LIB" /D "JAS_WIN_MSVC_BUILD" /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "UNICODE" /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_NON_CONFORMING_SWPRINTFS" /FD /GZ /c
# SUBTRACT BASE CPP /Fr /YX
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\jpeg" /I "..\png" /I "..\zlib" /I "..\mng" /I "..\tiff" /I "..\j2k" /I "..\jasper\include" /D "_LIB" /D "JAS_WIN_MSVC_BUILD" /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "UNICODE" /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_NON_CONFORMING_SWPRINTFS" /FD /GZ /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../lib/Debug/cximage.lib"

!ELSEIF  "$(CFG)" == "CxImage - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CxImage___Win32_Release"
# PROP BASE Intermediate_Dir "CxImage___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../obj/Release/CxImage"
# PROP Intermediate_Dir "../../../obj/Release/CxImage"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\jpeg" /I "..\png" /I "..\zlib" /I "..\mng" /I "..\tiff" /I "..\j2k" /I "..\jasper\include" /D "_LIB" /D "JAS_WIN_MSVC_BUILD" /D "WIN32" /D "NDEBUG" /D "_UNICODE" /D "UNICODE" /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_NON_CONFORMING_SWPRINTFS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /W3 /GX /O2 /I "..\jpeg" /I "..\png" /I "..\zlib" /I "..\mng" /I "..\tiff" /I "..\j2k" /I "..\jasper\include" /D "_LIB" /D "JAS_WIN_MSVC_BUILD" /D "WIN32" /D "NDEBUG" /D "_UNICODE" /D "UNICODE" /D "_CRT_SECURE_NO_DEPRECATE" /D "_CRT_NON_CONFORMING_SWPRINTFS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../lib/Release/cximage.lib"

!ENDIF 

# Begin Target

# Name "CxImage - Win32 Debug"
# Name "CxImage - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\tif_xfile.cpp
# End Source File
# Begin Source File

SOURCE=.\ximabmp.cpp
# End Source File
# Begin Source File

SOURCE=.\ximadsp.cpp
# End Source File
# Begin Source File

SOURCE=.\ximaenc.cpp
# End Source File
# Begin Source File

SOURCE=.\ximaexif.cpp
# End Source File
# Begin Source File

SOURCE=.\xImage.cpp
# End Source File
# Begin Source File

SOURCE=.\ximagif.cpp
# End Source File
# Begin Source File

SOURCE=.\ximahist.cpp
# End Source File
# Begin Source File

SOURCE=.\ximaico.cpp
# End Source File
# Begin Source File

SOURCE=.\ximainfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ximaint.cpp
# End Source File
# Begin Source File

SOURCE=.\ximajas.cpp
# End Source File
# Begin Source File

SOURCE=.\ximajbg.cpp
# End Source File
# Begin Source File

SOURCE=.\ximajpg.cpp
# End Source File
# Begin Source File

SOURCE=.\ximalpha.cpp
# End Source File
# Begin Source File

SOURCE=.\ximalyr.cpp
# End Source File
# Begin Source File

SOURCE=.\ximamng.cpp
# End Source File
# Begin Source File

SOURCE=.\ximapal.cpp
# End Source File
# Begin Source File

SOURCE=.\ximapcx.cpp
# End Source File
# Begin Source File

SOURCE=.\ximapng.cpp
# End Source File
# Begin Source File

SOURCE=.\ximaraw.cpp
# End Source File
# Begin Source File

SOURCE=.\ximasel.cpp
# End Source File
# Begin Source File

SOURCE=.\ximaska.cpp
# End Source File
# Begin Source File

SOURCE=.\ximatga.cpp
# End Source File
# Begin Source File

SOURCE=.\ximath.cpp
# End Source File
# Begin Source File

SOURCE=.\ximatif.cpp
# End Source File
# Begin Source File

SOURCE=.\ximatran.cpp
# End Source File
# Begin Source File

SOURCE=.\ximawbmp.cpp
# End Source File
# Begin Source File

SOURCE=.\ximawmf.cpp
# End Source File
# Begin Source File

SOURCE=.\ximawnd.cpp
# End Source File
# Begin Source File

SOURCE=.\xmemfile.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\xfile.h
# End Source File
# Begin Source File

SOURCE=.\ximabmp.h
# End Source File
# Begin Source File

SOURCE=.\ximacfg.h
# End Source File
# Begin Source File

SOURCE=.\ximadef.h
# End Source File
# Begin Source File

SOURCE=.\ximage.h
# End Source File
# Begin Source File

SOURCE=.\ximagif.h
# End Source File
# Begin Source File

SOURCE=.\ximaico.h
# End Source File
# Begin Source File

SOURCE=.\ximaiter.h
# End Source File
# Begin Source File

SOURCE=.\ximajas.h
# End Source File
# Begin Source File

SOURCE=.\ximajbg.h
# End Source File
# Begin Source File

SOURCE=.\ximajpg.h
# End Source File
# Begin Source File

SOURCE=.\ximamng.h
# End Source File
# Begin Source File

SOURCE=.\ximapcx.h
# End Source File
# Begin Source File

SOURCE=.\ximapng.h
# End Source File
# Begin Source File

SOURCE=.\ximaraw.h
# End Source File
# Begin Source File

SOURCE=.\ximaska.h
# End Source File
# Begin Source File

SOURCE=.\ximatga.h
# End Source File
# Begin Source File

SOURCE=.\ximath.h
# End Source File
# Begin Source File

SOURCE=.\ximatif.h
# End Source File
# Begin Source File

SOURCE=.\ximawbmp.h
# End Source File
# Begin Source File

SOURCE=.\ximawmf.h
# End Source File
# Begin Source File

SOURCE=.\xiofile.h
# End Source File
# Begin Source File

SOURCE=.\xmemfile.h
# End Source File
# End Group
# End Target
# End Project
