# Microsoft Developer Studio Project File - Name="TerraFighter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TerraFighter - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TerraFighter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TerraFighter.mak" CFG="TerraFighter - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TerraFighter - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TerraFighter - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TerraFighter - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "NDEBUG"
# ADD RSC /l 0x1009 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 dxerr8.lib dinput8.lib dsound.lib dxguid.lib d3dx8dt.lib d3d8.lib winmm.lib D3dxof.lib /nologo /subsystem:windows /pdb:none /machine:I386 /fixed:no

!ELSEIF  "$(CFG)" == "TerraFighter - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "_DEBUG"
# ADD RSC /l 0x1009 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dxerr8.lib dinput8.lib dsound.lib dxguid.lib d3dx8dt.lib d3d8.lib winmm.lib d3dxof.lib /nologo /subsystem:windows /profile /debug /machine:I386

!ENDIF 

# Begin Target

# Name "TerraFighter - Win32 Release"
# Name "TerraFighter - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\CommonDev\2DDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\3DTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\AddressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Animation.cpp
# End Source File
# Begin Source File

SOURCE=.\BigExplosion.cpp
# End Source File
# Begin Source File

SOURCE=.\BlasterShot.cpp
# End Source File
# Begin Source File

SOURCE=.\Bush.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonDev\d3dapp.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonDev\d3dfile.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonDev\d3dfont.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonDev\d3dutil.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayObject.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonDev\dsutil.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonDev\dxutil.cpp
# End Source File
# Begin Source File

SOURCE=.\Enemy.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Exaust.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonDev\FOVClipper.cpp
# End Source File
# Begin Source File

SOURCE=.\Input.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\LensFlare.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\CommonDev\LevelData.cpp
# End Source File
# Begin Source File

SOURCE=.\LobbyWaitDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MenueDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiplayerConnectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiplayerCreateGameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiplayerGamesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\netconnect.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\NetworkConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ParticleSystem.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Planet.cpp
# End Source File
# Begin Source File

SOURCE=.\PowerUp.cpp
# End Source File
# Begin Source File

SOURCE=.\SayDlg.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SelectDeviceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectLevelDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectShipDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SessionInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ShipObject.cpp

!IF  "$(CFG)" == "TerraFighter - Win32 Release"

!ELSEIF  "$(CFG)" == "TerraFighter - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ShotSystem.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\CommonDev\SkyBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Sprite.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StaticMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticObj.cpp
# End Source File
# Begin Source File

SOURCE=.\Station.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Surface.cpp
# End Source File
# Begin Source File

SOURCE=.\TerraFighter.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TerraFighter.rc
# End Source File
# Begin Source File

SOURCE=.\TerraFighterApp.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\Tree.cpp
# End Source File
# Begin Source File

SOURCE=.\Trees.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Vegetation.cpp
# End Source File
# Begin Source File

SOURCE=.\Vehicle.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Water.cpp
# End Source File
# Begin Source File

SOURCE=.\WayPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Weapon.cpp
# End Source File
# Begin Source File

SOURCE=.\WinArrowButton.cpp
# End Source File
# Begin Source File

SOURCE=.\WinBaceObj.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WinBitmapButton.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WinBitmapCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\WinButton.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WinCheckBox.cpp
# End Source File
# Begin Source File

SOURCE=.\WinComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\WinDialog.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WindowMgr.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WinEditBox.cpp
# End Source File
# Begin Source File

SOURCE=.\WinGroupBox.cpp
# End Source File
# Begin Source File

SOURCE=.\WinListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\WinProgress.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WinScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\WinStatic.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WinToolTip.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\YesNoDlg.cpp
# ADD CPP /Yu"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\CommonDev\Include\2DDraw.h
# End Source File
# Begin Source File

SOURCE=.\3DTarget.h
# End Source File
# Begin Source File

SOURCE=.\AddressDlg.h
# End Source File
# Begin Source File

SOURCE=.\Animation.h
# End Source File
# Begin Source File

SOURCE=.\BigExplosion.h
# End Source File
# Begin Source File

SOURCE=.\BlasterShot.h
# End Source File
# Begin Source File

SOURCE=.\Bush.h
# End Source File
# Begin Source File

SOURCE=.\ChatDlg.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\d3dapp.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\d3dfile.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\d3dfont.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\d3dres.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\d3dutil.h
# End Source File
# Begin Source File

SOURCE=.\DisplayObject.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\dsutil.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\dxutil.h
# End Source File
# Begin Source File

SOURCE=.\Enemy.h
# End Source File
# Begin Source File

SOURCE=.\Exaust.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\FOVClipper.h
# End Source File
# Begin Source File

SOURCE=.\Input.h
# End Source File
# Begin Source File

SOURCE=.\LensFlare.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\LevelData.h
# End Source File
# Begin Source File

SOURCE=.\LobbyWaitDlg.h
# End Source File
# Begin Source File

SOURCE=.\MenueDlg.h
# End Source File
# Begin Source File

SOURCE=.\MultiplayerConnectDlg.h
# End Source File
# Begin Source File

SOURCE=.\MultiplayerCreateGameDlg.h
# End Source File
# Begin Source File

SOURCE=.\MultiplayerGamesDlg.h
# End Source File
# Begin Source File

SOURCE=.\netconnect.h
# End Source File
# Begin Source File

SOURCE=.\NetMessages.h
# End Source File
# Begin Source File

SOURCE=.\NetworkConnection.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\Octtree.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\ParticleSystem.h
# End Source File
# Begin Source File

SOURCE=.\Planet.h
# End Source File
# Begin Source File

SOURCE=.\PowerUp.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\QuadTree.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SayDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectDeviceDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectLevelDlg.h
# End Source File
# Begin Source File

SOURCE=.\SelectShipDlg.h
# End Source File
# Begin Source File

SOURCE=.\SessionInfo.h
# End Source File
# Begin Source File

SOURCE=.\ShipObject.h
# End Source File
# Begin Source File

SOURCE=.\ShotSystem.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\SkyBox.h
# End Source File
# Begin Source File

SOURCE=.\SkyBox.h
# End Source File
# Begin Source File

SOURCE=.\SortedList.h
# End Source File
# Begin Source File

SOURCE=.\Sprite.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\SpriteData.h
# End Source File
# Begin Source File

SOURCE=.\StaticMesh.h
# End Source File
# Begin Source File

SOURCE=.\StaticObj.h
# End Source File
# Begin Source File

SOURCE=.\Station.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\Surface.h
# End Source File
# Begin Source File

SOURCE=.\Surface.h
# End Source File
# Begin Source File

SOURCE=.\TerraFighterApp.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\Terrain.h
# End Source File
# Begin Source File

SOURCE=.\Tree.h
# End Source File
# Begin Source File

SOURCE=.\Vegetation.h
# End Source File
# Begin Source File

SOURCE=.\Vehicle.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\vertextypes.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\Water.h
# End Source File
# Begin Source File

SOURCE=.\WayPoint.h
# End Source File
# Begin Source File

SOURCE=.\Weapon.h
# End Source File
# Begin Source File

SOURCE=.\WinArrowButton.h
# End Source File
# Begin Source File

SOURCE=.\WinBaceObj.h
# End Source File
# Begin Source File

SOURCE=.\WinBitmapButton.h
# End Source File
# Begin Source File

SOURCE=.\WinBitmapCtrl.h
# End Source File
# Begin Source File

SOURCE=.\WinButton.h
# End Source File
# Begin Source File

SOURCE=.\WinCheckBox.h
# End Source File
# Begin Source File

SOURCE=.\WinComboBox.h
# End Source File
# Begin Source File

SOURCE=.\WinDialog.h
# End Source File
# Begin Source File

SOURCE=.\WindowMgr.h
# End Source File
# Begin Source File

SOURCE=.\WinEditBox.h
# End Source File
# Begin Source File

SOURCE=.\WinGroupBox.h
# End Source File
# Begin Source File

SOURCE=.\WinListBox.h
# End Source File
# Begin Source File

SOURCE=.\WinProgress.h
# End Source File
# Begin Source File

SOURCE=.\WinScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\WinStatic.h
# End Source File
# Begin Source File

SOURCE=.\WinToolTip.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\XmlHelpers.h
# End Source File
# Begin Source File

SOURCE=.\YesNoDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\directx.ico
# End Source File
# End Group
# End Target
# End Project
