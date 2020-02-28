///////////////////////////////////////////////////////////////////////////////
///  WinCheckBox.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:18:26 PM by Christiaan Burchell
///
///  PURPOSE:  the check box control
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

// WinCheckBox.cpp: implementation of the CWinCheckBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinCheckBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CWinCheckBox
///
/// CLASS: CWinCheckBox
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 12:17:56 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinCheckBox::CWinCheckBox() : CWinButton()
{
   m_bChecked = FALSE;
}



/////////////////////////////////////////////////
///
/// NAME: ~CWinCheckBox
///
/// CLASS: CWinCheckBox
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 12:17:58 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinCheckBox::~CWinCheckBox()
{

}



/////////////////////////////////////////////////
///
/// NAME: Display
///
/// CLASS: CWinCheckBox
///
/// DESCRIPTION: Display the control
///
/// CREATED:     02/06/2004 12:18:04 PM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinCheckBox::Display(C2DDraw *pDisplay)
{
   SetText(m_bChecked?"X":" ");
   BOOL tmp = m_bPressed;
   m_bPressed = TRUE;
   CWinButton::Display(pDisplay);
   m_bPressed = tmp;
}



/////////////////////////////////////////////////
///
/// NAME: OnMouseUp
///
/// CLASS: CWinCheckBox
///
/// DESCRIPTION: Handel mouse up
///
/// CREATED:     02/06/2004 12:18:11 PM
///
/// PARAMETERS: 
///             CPoint p
///
/// RETURN: int 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
int CWinCheckBox::OnMouseUp(const CPoint& p)
{
   if(m_bPressed)
   {
      OnButtonPressed(GetID());
      m_bPressed = FALSE;
      m_bChecked = !m_bChecked;
   }
   return WIN_BUTTON_ESC;
}