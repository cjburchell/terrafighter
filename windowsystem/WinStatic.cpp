///////////////////////////////////////////////////////////////////////////////
///  WinStatic.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 1:10:32 PM by Christiaan Burchell
///
///  PURPOSE:  Static control
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

// WinStatic.cpp: implementation of the CWinStatic class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "WinStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CWinStatic
///
/// CLASS: CWinStatic
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 1:07:17 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinStatic::CWinStatic() : CWinBaceObj()
{

}



/////////////////////////////////////////////////
///
/// NAME: ~CWinStatic
///
/// CLASS: CWinStatic
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 1:07:19 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinStatic::~CWinStatic()
{

}



/////////////////////////////////////////////////
///
/// NAME: Display
///
/// CLASS: CWinStatic
///
/// DESCRIPTION: Display control
///
/// CREATED:     02/06/2004 1:07:24 PM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinStatic::Display(C2DDraw *pDisplay)
{
   pDisplay->OutTextc(m_BackgroundColor,m_ForgoundColor,GetDrawPoint().x,GetDrawPoint().y,FALSE,"%s",GetText());
   CWinBaceObj::Display(pDisplay);
}

