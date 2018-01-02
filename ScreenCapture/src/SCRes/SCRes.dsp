# Microsoft Developer Studio Project File - Name="SCRes" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SCRes - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SCRes.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SCRes.mak" CFG="SCRes - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SCRes - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SCRes - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SCRes - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../obj/Release/SCRes"
# PROP Intermediate_Dir "../../obj/Release/SCRes"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SCRES_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../include/wtl" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "_USRDLL" /D "SCRES_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../bin/Release/SCRes.dll" /noentry
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SCRes - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../obj/Debug/SCRes"
# PROP Intermediate_Dir "../../obj/Debug/SCRes"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SCRES_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /D "_USRDLL" /D "SCRES_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../bin/Debug/SCRes.dll" /pdbtype:sept /noentry
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "SCRes - Win32 Release"
# Name "SCRes - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SCRes.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\about_wave_bk.BMP
# End Source File
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\btn_font_bold.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\btn_font_bold.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\btn_font_bold_2.bmp
# End Source File
# Begin Source File

SOURCE=.\btn_pen_Big.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\btn_pen_Big.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\btn_pen_Big_2.bmp
# End Source File
# Begin Source File

SOURCE=.\btn_pen_bk.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\btn_pen_bk_2.bmp
# End Source File
# Begin Source File

SOURCE=.\btn_pen_Middle.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\btn_pen_Middle.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\btn_pen_Middle_2.bmp
# End Source File
# Begin Source File

SOURCE=.\btn_pen_Small.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\btn_pen_Small.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\btn_pen_Small_2.bmp
# End Source File
# Begin Source File

SOURCE=.\canvas_s.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\canvas_s.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\close_btn.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\close_btn_bk.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\default_.cur
# End Source File
# Begin Source File

SOURCE=.\Res\dialog_btn_BK_1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\dialog_btn_img_1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\eraser_c.cur
# End Source File
# Begin Source File

SOURCE=.\float_drag_Bk.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\float_drag_Bk.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\float_drag_Bk_2.bmp
# End Source File
# Begin Source File

SOURCE=.\float_drag_img.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\float_drag_img.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\float_drag_img_2.bmp
# End Source File
# Begin Source File

SOURCE=.\float_menu_Bk.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\float_menu_Bk.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\float_menu_Bk_2.bmp
# End Source File
# Begin Source File

SOURCE=.\float_menu_img.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\float_menu_img.bmp
# End Source File
# Begin Source File

SOURCE=.\float_menu_img_2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\float_menu_img_2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\float_tool_Bk.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\float_tool_Bk_2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\info_bubble_bk_1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\info_bubble_bk_2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\menu_check_box.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\menu_check_box_2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\pen_curs.cur
# End Source File
# Begin Source File

SOURCE=.\Res\select_c.cur
# End Source File
# Begin Source File

SOURCE=.\Res\start_btn.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\start_btn_bk.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sub_float_tool_bk_1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\sub_float_tool_bk_2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\title_bk_1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\title_bk_2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_arrow_btn_img.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_arrow_btn_img_2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_btn_bk.bmp
# End Source File
# Begin Source File

SOURCE=.\tool_btn_bk.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_btn_bk_2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_btn_img.bmp
# End Source File
# Begin Source File

SOURCE=.\tool_btn_img.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_btn_img_2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_eclipse_btn_img.bmp
# End Source File
# Begin Source File

SOURCE=.\tool_eclipse_btn_img.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_eclipse_btn_img_2.bmp
# End Source File
# Begin Source File

SOURCE=.\tool_eraser_btn_img.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_eraser_btn_img_2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_line_btn_img.bmp
# End Source File
# Begin Source File

SOURCE=.\tool_line_btn_img.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_line_btn_img_2.bmp
# End Source File
# Begin Source File

SOURCE=.\tool_pen_btn_img.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_pen_btn_img_2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_rect_btn_img.bmp
# End Source File
# Begin Source File

SOURCE=.\tool_rect_btn_img.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_rect_btn_img_2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_sep.bmp
# End Source File
# Begin Source File

SOURCE=.\tool_sep.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_sep_2.bmp
# End Source File
# Begin Source File

SOURCE=.\tool_text_btn_img.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_text_btn_img_2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_win_select_btn_img_1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\tool_win_select_btn_img_2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\water_mark_1.bmp
# End Source File
# Begin Source File

SOURCE=.\water_mark_1.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\water_mark_2.bmp
# End Source File
# Begin Source File

SOURCE=.\water_mark_2.bmp
# End Source File
# End Group
# End Target
# End Project
