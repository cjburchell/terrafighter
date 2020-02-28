///////////////////////////////////////////////////////////////////////////////
///  WinProgress.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 1:10:54 PM by Christiaan Burchell
///
///  PURPOSE:  Progress control
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

// WinProgress.cpp: implementation of the CWinProgress class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "WinProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CWinProgress
///
/// CLASS: CWinProgress
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 1:04:27 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinProgress::CWinProgress() : CWinBaceObj()
{
   m_h = 0;
}



/////////////////////////////////////////////////
///
/// NAME: ~CWinProgress
///
/// CLASS: CWinProgress
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 1:04:28 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinProgress::~CWinProgress()
{

}



/////////////////////////////////////////////////
///
/// NAME: Display
///
/// CLASS: CWinProgress
///
/// DESCRIPTION: Display control
///
/// CREATED:     02/06/2004 1:04:33 PM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinProgress::Display(C2DDraw *pDisplay)
{
   CRect rect = GetDrawRect();
   CRect temprect = rect;
   if(ShowBackground())
      pDisplay->DrawBlendRect(rect,m_BackgroundColor);

   temprect.top += GetSize().cy - m_h;
   pDisplay->DrawFilledRect(temprect,m_color);

   DrawBorder(pDisplay,rect);

   CWinBaceObj::Display(pDisplay);
}



/////////////////////////////////////////////////
///
/// NAME: SetProgress
///
/// CLASS: CWinProgress
///
/// DESCRIPTION: Set progress
///
/// CREATED:     02/06/2004 1:04:40 PM
///
/// PARAMETERS: 
///             int progress
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinProgress::SetProgress(int progress)
{
   m_h = static_cast<int>((static_cast<double>(progress)/100.0)*static_cast<double>(GetSize().cy));
}



/////////////////////////////////////////////////
///
/// NAME: SetColor
///
/// CLASS: CWinProgress
///
/// DESCRIPTION: Set colour of the progress bar
///
/// CREATED:     02/06/2004 1:05:06 PM
///
/// PARAMETERS: 
///             D3DCOLOR color
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinProgress::SetColor(D3DCOLOR color)
{
   m_color = color;
}



/////////////////////////////////////////////////
///
/// NAME: OnMouseOver
///
/// CLASS: CWinProgress
///
/// DESCRIPTION: Display tool tip when mouse is over
///
/// CREATED:     02/06/2004 1:05:21 PM
///
/// PARAMETERS: 
///             BOOL bIsOver
///             CPoint p
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinProgress::OnMouseOver(BOOL bIsOver, const CPoint&)
{
   m_ToolTip.Show(bIsOver);
}



/////////////////////////////////////////////////
///
/// NAME: Create
///
/// CLASS: CWinProgress
///
/// DESCRIPTION: Create the control
///
/// CREATED:     02/06/2004 1:05:31 PM
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
BOOL CWinProgress::Create(C2DDraw *pDisplay, CWinBaceObj *pParent, const CPoint& p, const CSize& s, const TCHAR* str)
{
   if(!CWinBaceObj::Create(pDisplay,pParent,p,s,str))
      return FALSE;
   m_ToolTip.Create(pDisplay,this,CPoint(1,1),CSize(0,0),str);
   m_ToolTip.Show(FALSE);
   return TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: SetToolTipText
///
/// CLASS: CWinProgress
///
/// DESCRIPTION: Set the tool tip text
///
/// CREATED:     02/06/2004 1:05:42 PM
///
/// PARAMETERS: 
///             CString s
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinProgress::SetToolTipText(const TCHAR* s)
{
   m_ToolTip.SetText(s);
}
