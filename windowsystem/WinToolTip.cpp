///////////////////////////////////////////////////////////////////////////////
///  WinToolTip.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 1:10:18 PM by Christiaan Burchell
///
///  PURPOSE:  Tool tip control
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

// WinToolTip.cpp: implementation of the CWinToolTip class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "WinToolTip.h"
#include "WindowMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CWinToolTip
///
/// CLASS: CWinToolTip
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 1:07:56 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinToolTip::CWinToolTip() : CWinBaceObj()
{
   m_bShow = TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: ~CWinToolTip
///
/// CLASS: CWinToolTip
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 1:07:59 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinToolTip::~CWinToolTip()
{
   GetWindowMgr()->RemoveChild(this);
}



/////////////////////////////////////////////////
///
/// NAME: MouseOver
///
/// CLASS: CWinToolTip
///
/// DESCRIPTION: Handel mouse over
///
/// CREATED:     02/06/2004 1:08:47 PM
///
/// PARAMETERS: 
///             CPoint p
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinToolTip::MouseOver(const CPoint&)
{
   return FALSE;
}



/////////////////////////////////////////////////
///
/// NAME: MouseDown
///
/// CLASS: CWinToolTip
///
/// DESCRIPTION: Handel mouse down
///
/// CREATED:     02/06/2004 1:08:59 PM
///
/// PARAMETERS: 
///             CPoint p
///             int &Id
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinToolTip::MouseDown(const CPoint& , int&)
{
   return FALSE;
}




/////////////////////////////////////////////////
///
/// NAME: Display
///
/// CLASS: CWinToolTip
///
/// DESCRIPTION: Display control
///
/// CREATED:     02/06/2004 1:08:35 PM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinToolTip::Display(C2DDraw *pDisplay)
{
   if(m_bShow)
   {
      CRect r = pDisplay->GetTextRect(GetText());
      r.OffsetRect(GetDrawPoint());
      pDisplay->DrawBlendRect(r,0x7f000000);
      pDisplay->OutTextc(m_BackgroundColor,m_HighlightColor,GetDrawPoint().x,GetDrawPoint().y,TRUE,"%s",GetText());
      CWinBaceObj::Display(pDisplay);
   }
}



/////////////////////////////////////////////////
///
/// NAME: MouseUp
///
/// CLASS: CWinToolTip
///
/// DESCRIPTION: Handle mouse up
///
/// CREATED:     02/06/2004 1:08:28 PM
///
/// PARAMETERS: 
///             CPoint p
///             int &Id
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinToolTip::MouseUp(const CPoint&, int&)
{
   return FALSE;
}



/////////////////////////////////////////////////
///
/// NAME: Show
///
/// CLASS: CWinToolTip
///
/// DESCRIPTION: Enable Tool Tip
///
/// CREATED:     02/06/2004 1:08:18 PM
///
/// PARAMETERS: 
///             BOOL bShow
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinToolTip::Show(BOOL bShow)
{
   m_bShow = bShow;
}



/////////////////////////////////////////////////
///
/// NAME: Create
///
/// CLASS: CWinToolTip
///
/// DESCRIPTION: Create control
///
/// CREATED:     02/06/2004 1:07:52 PM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///             CWinBaceObj *pParent
///             CPoint p
///             CSize s
///             CString str
///
/// RETURN: BOOL 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
BOOL CWinToolTip::Create(C2DDraw *pDisplay, CWinBaceObj *pParent, const CPoint& p, const CSize& s, const TCHAR* str)
{
   if(!CWinBaceObj::Create(pDisplay,pParent,p,s,str))
      return FALSE;
   pParent->RemoveChild(this);

   m_pParent = pParent;

   GetWindowMgr()->AddToolTip(this);
   return TRUE;
}
