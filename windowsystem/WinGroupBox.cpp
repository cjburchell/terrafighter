///////////////////////////////////////////////////////////////////////////////
///  WinGroupBox.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 1:11:19 PM by Christiaan Burchell
///
///  PURPOSE:  Group box control
///
///  COPYRIGHT NOTICE:   Copyright (C) 2004 Redpoint Games 
///
///   This software is provided 'as-is', without any express or implied 
///   warranty.  In no event will the author be held liable for any damages 
///   arising from the use of this software. 
/// 
///   Permission is granted to anyone to use this software for any purpose,
///   excluding commercial applications, and to alter it and redistribute it
///   freely, subject to the following restrictions:
/// 
///   1. The origin of this software must not be misrepresented; you must not
///      claim that you wrote the original software. If you use this software
///      in a product, an acknowledgment in the product documentation would be
///      appreciated but is not required.
///   2. Altered source versions must be plainly marked as such, and must not be
///      misrepresented as being the original software.
///   3. This notice may not be removed or altered from any source distribution.
///   4. The author permission is required to use this software in commercial 
///      applications 
///
///  LAST CHANGED: $Date$
///
///  REVISION HISTORY:
///  $Log$
/// 

// WinGroupBox.cpp: implementation of the CWinGroupBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinGroupBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CWinGroupBox
///
/// CLASS: CWinGroupBox
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 12:31:04 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinGroupBox::CWinGroupBox() : CWinStatic()
{

}



/////////////////////////////////////////////////
///
/// NAME: ~CWinGroupBox
///
/// CLASS: CWinGroupBox
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 12:31:06 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinGroupBox::~CWinGroupBox()
{

}



/////////////////////////////////////////////////
///
/// NAME: Display
///
/// CLASS: CWinGroupBox
///
/// DESCRIPTION: Display the control
///
/// CREATED:     02/06/2004 12:31:15 PM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinGroupBox::Display(C2DDraw *pDisplay)
{
   CRect rect = GetDrawRect();
   DrawBorder(pDisplay,rect);
   CWinStatic::Display(pDisplay);
}
