# Microsoft Developer Studio Project File - Name="TerraEd" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TerraEd - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TerraEd.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TerraEd.mak" CFG="TerraEd - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TerraEd - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TerraEd - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TerraEd - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x1009 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "TerraEd - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x1009 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x1009 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dxerr8.lib dinput8.lib dsound.lib dxguid.lib d3dx8dt.lib d3d8.lib winmm.lib d3dxof.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TerraEd - Win32 Release"
# Name "TerraEd - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\CommonDev\2DDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\AddTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ColourButton.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.cpp
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

SOURCE=.\DeleteFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonDev\dxutil.cpp
# End Source File
# Begin Source File

SOURCE=.\EditEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\EditLevel.cpp
# End Source File
# Begin Source File

SOURCE=.\EditObject.cpp
# End Source File
# Begin Source File

SOURCE=.\EditObjectData.cpp
# End Source File
# Begin Source File

SOURCE=.\EditSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\EditSpriteData.cpp
# End Source File
# Begin Source File

SOURCE=.\EditTextrue.cpp
# End Source File
# Begin Source File

SOURCE=.\FillDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonDev\FOVClipper.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonDev\LevelData.cpp
# End Source File
# Begin Source File

SOURCE=.\LevelTreeBar.cpp
# End Source File
# Begin Source File

SOURCE=.\PlanetObjectDataEx.cpp
# End Source File
# Begin Source File

SOURCE=.\RandomObjDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelObjDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonDev\SkyBox.cpp
# End Source File
# Begin Source File

SOURCE=.\SpriteDataEx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Surface.cpp
# End Source File
# Begin Source File

SOURCE=.\TerraEd.cpp
# End Source File
# Begin Source File

SOURCE=.\TerraEd.rc
# End Source File
# Begin Source File

SOURCE=.\TerraEdDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\TerraEdView.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Terrain.cpp
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Water.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddTexture.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ColourButton.h
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\DeleteFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditEvent.h
# End Source File
# Begin Source File

SOURCE=.\EditLevel.h
# End Source File
# Begin Source File

SOURCE=.\EditObject.h
# End Source File
# Begin Source File

SOURCE=.\EditObjectData.h
# End Source File
# Begin Source File

SOURCE=.\EditSprite.h
# End Source File
# Begin Source File

SOURCE=.\EditSpriteData.h
# End Source File
# Begin Source File

SOURCE=.\EditTextrue.h
# End Source File
# Begin Source File

SOURCE=.\FillDialog.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\LevelData.h
# End Source File
# Begin Source File

SOURCE=.\LevelTreeBar.h
# End Source File
# Begin Source File

SOURCE=.\PlanetObjectDataEx.h
# End Source File
# Begin Source File

SOURCE=.\RandomObjDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SelObjDlg.h
# End Source File
# Begin Source File

SOURCE=.\SpriteDataEx.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TerraEd.h
# End Source File
# Begin Source File

SOURCE=.\TerraEdDoc.h
# End Source File
# Begin Source File

SOURCE=.\TerraEdView.h
# End Source File
# Begin Source File

SOURCE=..\CommonDev\Include\XmlHelpers.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\OdPan.cur
# End Source File
# Begin Source File

SOURCE=.\OdPanReady.cur
# End Source File
# Begin Source File

SOURCE=.\OdRotate.cur
# End Source File
# Begin Source File

SOURCE=.\OdZoom.cur
# End Source File
# Begin Source File

SOURCE=.\res\TerraEd.ico
# End Source File
# Begin Source File

SOURCE=.\res\TerraEd.rc2
# End Source File
# Begin Source File

SOURCE=.\res\TerraEdDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
