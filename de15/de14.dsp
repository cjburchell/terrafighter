# Microsoft Developer Studio Project File - Name="de14" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=de14 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "de14.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "de14.mak" CFG="de14 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "de14 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "de14 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "de14 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "NDEBUG"
# ADD RSC /l 0x1009 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ddraw.lib Winmm.lib dxguid.lib dxerr8.lib dinput8.lib d3d8.lib D3dx8.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "de14 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "_DEBUG"
# ADD RSC /l 0x1009 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ddraw.lib Winmm.lib dxguid.lib dxerr8.lib dinput8.lib d3d8.lib D3dx8.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "de14 - Win32 Release"
# Name "de14 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Monsters"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EndMonster.cpp
# End Source File
# Begin Source File

SOURCE=.\FireMonst.cpp
# End Source File
# Begin Source File

SOURCE=.\Healer.cpp
# End Source File
# Begin Source File

SOURCE=.\KeySales.cpp
# End Source File
# Begin Source File

SOURCE=.\LightningMonst.cpp
# End Source File
# Begin Source File

SOURCE=.\monst.cpp
# End Source File
# Begin Source File

SOURCE=.\Monster.cpp
# End Source File
# Begin Source File

SOURCE=.\Murchent.cpp
# End Source File
# Begin Source File

SOURCE=.\NPC.cpp
# End Source File
# Begin Source File

SOURCE=.\player.cpp
# End Source File
# Begin Source File

SOURCE=.\Projectile.cpp
# End Source File
# Begin Source File

SOURCE=.\Saver.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite.cpp
# End Source File
# End Group
# Begin Group "Tiles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DEStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\Door.cpp
# End Source File
# Begin Source File

SOURCE=.\Ship.cpp
# End Source File
# Begin Source File

SOURCE=.\Wall.cpp
# End Source File
# Begin Source File

SOURCE=.\Warp.cpp
# End Source File
# Begin Source File

SOURCE=.\Water.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\2DDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\2DFont.cpp
# End Source File
# Begin Source File

SOURCE=.\2DSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\3DDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\3DObject.cpp
# End Source File
# Begin Source File

SOURCE=.\BuySellDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Chest.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dutil.cpp
# End Source File
# Begin Source File

SOURCE=.\ddutil.cpp
# End Source File
# Begin Source File

SOURCE=.\de14.cpp
# End Source File
# Begin Source File

SOURCE=.\de14.rc
# End Source File
# Begin Source File

SOURCE=.\DEObject.cpp
# End Source File
# Begin Source File

SOURCE=.\DEWndowMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\dxutil.cpp
# End Source File
# Begin Source File

SOURCE=.\Hud.cpp
# End Source File
# Begin Source File

SOURCE=.\Icons.cpp
# End Source File
# Begin Source File

SOURCE=.\Input.cpp
# End Source File
# Begin Source File

SOURCE=.\Input2.cpp
# End Source File
# Begin Source File

SOURCE=.\Item.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Items.cpp
# End Source File
# Begin Source File

SOURCE=.\map.cpp
# End Source File
# Begin Source File

SOURCE=.\MapObject.cpp
# End Source File
# Begin Source File

SOURCE=.\MenueDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\mouse.cpp
# End Source File
# Begin Source File

SOURCE=.\path.cpp
# End Source File
# Begin Source File

SOURCE=.\RangeAttackMonster.cpp
# End Source File
# Begin Source File

SOURCE=.\SayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Spell.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\spells.cpp
# End Source File
# Begin Source File

SOURCE=.\stlastar.cpp
# End Source File
# Begin Source File

SOURCE=.\StoreDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Town.cpp
# End Source File
# Begin Source File

SOURCE=.\UseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WinBaceObj.cpp
# End Source File
# Begin Source File

SOURCE=.\WinBitmapButton.cpp
# End Source File
# Begin Source File

SOURCE=.\WinButton.cpp
# End Source File
# Begin Source File

SOURCE=.\WinDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\WinProgress.cpp
# End Source File
# Begin Source File

SOURCE=.\WinStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\WinToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\YesNoDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\2DDisplay.h
# End Source File
# Begin Source File

SOURCE=.\2DFont.h
# End Source File
# Begin Source File

SOURCE=.\2DSurface.h
# End Source File
# Begin Source File

SOURCE=.\3DDisplay.h
# End Source File
# Begin Source File

SOURCE=.\3DObject.h
# End Source File
# Begin Source File

SOURCE=.\BuySellDlg.h
# End Source File
# Begin Source File

SOURCE=.\Chest.h
# End Source File
# Begin Source File

SOURCE=.\d3dutil.h
# End Source File
# Begin Source File

SOURCE=.\ddutil.h
# End Source File
# Begin Source File

SOURCE=.\de14.h
# End Source File
# Begin Source File

SOURCE=.\DEObject.h
# End Source File
# Begin Source File

SOURCE=.\DEStatic.h
# End Source File
# Begin Source File

SOURCE=.\DEWndowMgr.h
# End Source File
# Begin Source File

SOURCE=.\dialog.h
# End Source File
# Begin Source File

SOURCE=.\Door.h
# End Source File
# Begin Source File

SOURCE=.\dxutil.h
# End Source File
# Begin Source File

SOURCE=.\EndMonster.h
# End Source File
# Begin Source File

SOURCE=.\FireMonst.h
# End Source File
# Begin Source File

SOURCE=.\fsa.h
# End Source File
# Begin Source File

SOURCE=.\Healer.h
# End Source File
# Begin Source File

SOURCE=.\Hud.h
# End Source File
# Begin Source File

SOURCE=.\icons.h
# End Source File
# Begin Source File

SOURCE=.\Input.h
# End Source File
# Begin Source File

SOURCE=.\Input2.h
# End Source File
# Begin Source File

SOURCE=.\Item.h
# End Source File
# Begin Source File

SOURCE=.\ItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\Items.h
# End Source File
# Begin Source File

SOURCE=.\KeySales.h
# End Source File
# Begin Source File

SOURCE=.\LightningMonst.h
# End Source File
# Begin Source File

SOURCE=.\map.h
# End Source File
# Begin Source File

SOURCE=.\MapObject.h
# End Source File
# Begin Source File

SOURCE=.\MenueDlg.h
# End Source File
# Begin Source File

SOURCE=.\monst.h
# End Source File
# Begin Source File

SOURCE=.\Monster.h
# End Source File
# Begin Source File

SOURCE=.\mouse.h
# End Source File
# Begin Source File

SOURCE=.\Murchent.h
# End Source File
# Begin Source File

SOURCE=.\mymath.h
# End Source File
# Begin Source File

SOURCE=.\NPC.h
# End Source File
# Begin Source File

SOURCE=.\path.h
# End Source File
# Begin Source File

SOURCE=.\player.h
# End Source File
# Begin Source File

SOURCE=.\Projectile.h
# End Source File
# Begin Source File

SOURCE=.\RangeAttackMonster.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Save.h
# End Source File
# Begin Source File

SOURCE=.\Saver.h
# End Source File
# Begin Source File

SOURCE=.\SayDlg.h
# End Source File
# Begin Source File

SOURCE=.\Ship.h
# End Source File
# Begin Source File

SOURCE=.\Spell.h
# End Source File
# Begin Source File

SOURCE=.\SpellDlg.h
# End Source File
# Begin Source File

SOURCE=.\spells.h
# End Source File
# Begin Source File

SOURCE=.\Sprite.h
# End Source File
# Begin Source File

SOURCE=.\stlastar.h
# End Source File
# Begin Source File

SOURCE=.\StoreDlg.h
# End Source File
# Begin Source File

SOURCE=.\Town.h
# End Source File
# Begin Source File

SOURCE=.\Types.h
# End Source File
# Begin Source File

SOURCE=.\UseDlg.h
# End Source File
# Begin Source File

SOURCE=.\Wall.h
# End Source File
# Begin Source File

SOURCE=.\Warp.h
# End Source File
# Begin Source File

SOURCE=.\Water.h
# End Source File
# Begin Source File

SOURCE=.\WinBaceObj.h
# End Source File
# Begin Source File

SOURCE=.\WinBitmapButton.h
# End Source File
# Begin Source File

SOURCE=.\WinButton.h
# End Source File
# Begin Source File

SOURCE=.\WinDialog.h
# End Source File
# Begin Source File

SOURCE=.\WinProgress.h
# End Source File
# Begin Source File

SOURCE=.\WinStatic.h
# End Source File
# Begin Source File

SOURCE=.\WinToolTip.h
# End Source File
# Begin Source File

SOURCE=.\YesNoDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\de.ico
# End Source File
# End Group
# End Target
# End Project
