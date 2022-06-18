# Microsoft Developer Studio Project File - Name="csf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=csf - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "csf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "csf.mak" CFG="csf - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "csf - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CSF_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CSF_EXPORTS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x2c0a /d "NDEBUG"
# ADD RSC /l 0x2c0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"Release/PW.dll"
# Begin Target

# Name "csf - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "sdk"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\misc\parsemsg.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\misc\parsemsg.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\misc\sprites.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\misc\sprites.h
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Source File

SOURCE=.\ADE32.cpp
# End Source File
# Begin Source File

SOURCE=.\ADE32.h
# End Source File
# Begin Source File

SOURCE=.\aimbot.cpp
# End Source File
# Begin Source File

SOURCE=.\aimbot.h
# End Source File
# Begin Source File

SOURCE=.\aimsilent.cpp
# End Source File
# Begin Source File

SOURCE=.\aimsilent.h
# End Source File
# Begin Source File

SOURCE=.\apihook.cpp
# End Source File
# Begin Source File

SOURCE=.\apihook.h
# End Source File
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# Begin Source File

SOURCE=.\client.h
# End Source File
# Begin Source File

SOURCE=.\color.cpp
# End Source File
# Begin Source File

SOURCE=.\color.h
# End Source File
# Begin Source File

SOURCE=.\cros.cpp
# End Source File
# Begin Source File

SOURCE=.\cros.h
# End Source File
# Begin Source File

SOURCE=.\cSprites.cpp
# End Source File
# Begin Source File

SOURCE=.\cSprites.h
# End Source File
# Begin Source File

SOURCE=.\cvar.cpp
# End Source File
# Begin Source File

SOURCE=.\cvar.h
# End Source File
# Begin Source File

SOURCE=.\detours.cpp
# End Source File
# Begin Source File

SOURCE=.\detours.h
# End Source File
# Begin Source File

SOURCE=.\drawing.cpp
# End Source File
# Begin Source File

SOURCE=.\drawing.h
# End Source File
# Begin Source File

SOURCE=.\Engine.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine.h
# End Source File
# Begin Source File

SOURCE=.\ESP.cpp
# End Source File
# Begin Source File

SOURCE=.\ESP.h
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\gateways.cpp
# End Source File
# Begin Source File

SOURCE=.\gateways.h
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\menu.cpp
# End Source File
# Begin Source File

SOURCE=.\menu.h
# End Source File
# Begin Source File

SOURCE=.\miniradar.cpp
# End Source File
# Begin Source File

SOURCE=.\miniradar.h
# End Source File
# Begin Source File

SOURCE=.\msghook.cpp
# End Source File
# Begin Source File

SOURCE=.\msghook.h
# End Source File
# Begin Source File

SOURCE=.\netgraph.cpp
# End Source File
# Begin Source File

SOURCE=.\netgraph.h
# End Source File
# Begin Source File

SOURCE=.\NT_DDK.h
# End Source File
# Begin Source File

SOURCE=.\opengl32.cpp
# End Source File
# Begin Source File

SOURCE=.\opengl32.h
# End Source File
# Begin Source File

SOURCE=.\parsemsg.cpp
# End Source File
# Begin Source File

SOURCE=.\parsemsg.h
# End Source File
# Begin Source File

SOURCE=.\players.cpp
# End Source File
# Begin Source File

SOURCE=.\players.h
# End Source File
# Begin Source File

SOURCE=.\quake.cpp
# End Source File
# Begin Source File

SOURCE=.\quake.h
# End Source File
# Begin Source File

SOURCE=.\radar.cpp
# End Source File
# Begin Source File

SOURCE=.\radar.h
# End Source File
# Begin Source File

SOURCE=.\spinhack.cpp
# End Source File
# Begin Source File

SOURCE=.\spinhack.h
# End Source File
# Begin Source File

SOURCE=.\spread.cpp
# End Source File
# Begin Source File

SOURCE=.\spread.h
# End Source File
# Begin Source File

SOURCE=.\spy.cpp
# End Source File
# Begin Source File

SOURCE=.\spy.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\time.cpp
# End Source File
# Begin Source File

SOURCE=.\time.h
# End Source File
# Begin Source File

SOURCE=.\tkz.h
# End Source File
# Begin Source File

SOURCE=.\weapon.cpp
# End Source File
# Begin Source File

SOURCE=.\weapon.h
# End Source File
# Begin Source File

SOURCE=.\xEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\xEngine.h
# End Source File
# Begin Source File

SOURCE=.\xorstr.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
