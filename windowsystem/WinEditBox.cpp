///////////////////////////////////////////////////////////////////////////////
///  WinEditBox.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 1:11:35 PM by Christiaan Burchell
///
///  PURPOSE:  edit box control
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

// WinEditBox.cpp: implementation of the CWinEditBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinEditBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define EDIT_SIZE 12



/////////////////////////////////////////////////
///
/// NAME: CWinEditBox
///
/// CLASS: CWinEditBox
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 12:30:14 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinEditBox::CWinEditBox()
{

}



/////////////////////////////////////////////////
///
/// NAME: ~CWinEditBox
///
/// CLASS: CWinEditBox
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 12:30:16 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinEditBox::~CWinEditBox()
{

}



/////////////////////////////////////////////////
///
/// NAME: Display
///
/// CLASS: CWinEditBox
///
/// DESCRIPTION: Display the control
///
/// CREATED:     02/06/2004 12:30:24 PM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinEditBox::Display(C2DDraw *pDisplay)
{
   CRect rect = GetDrawRect();
   rect.bottom = rect.top + EDIT_SIZE;

   DrawBorder(pDisplay,rect);

   CString TmpStr = GetText();
   if(IsActive())
      TmpStr+="|";
   pDisplay->OutTextc(m_BackgroundColor,m_ForgoundColor,GetDrawPoint().x+2,GetDrawPoint().y+1,FALSE,"%s",TmpStr);
   CWinBaceObj::Display(pDisplay);
}



/////////////////////////////////////////////////
///
/// NAME: OnCharDown
///
/// CLASS: CWinEditBox
///
/// DESCRIPTION: handel char presses
///
/// CREATED:     02/06/2004 12:30:33 PM
///
/// PARAMETERS: 
///             char c
///             DWORD keycode
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinEditBox::OnCharDown(char c, DWORD keycode)
{
   CString TmpStr = GetText();
   if(c == 0x8) // delete key
   {
      TmpStr.Delete(TmpStr.GetLength()-1);
      SetText(TmpStr);
   }
   else if(c == '\r')
   {
      OnButtonPressed(GetID());
   }
   else
   {
      if(::isgraph(c) || ::isspace(c))
         TmpStr+=c;
      SetText(TmpStr);
   }

   CWinBaceObj::OnCharDown(c,keycode);
}
