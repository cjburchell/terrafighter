///////////////////////////////////////////////////////////////////////////////
///  WinScrollBar.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 1:06:14 PM by Christiaan Burchell
///
///  PURPOSE:  Scroll bar control
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

// WinScrollBar.cpp: implementation of the CWinScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <WinScrollBar.h>
#include <commonres.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CWinScrollBar
///
/// CLASS: CWinScrollBar
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 1:06:17 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinScrollBar::CWinScrollBar() : CWinBaceObj()
{
   m_RangeMin = 0;
   m_RangeMax = 10;
   m_ScrollPos = 0;
   m_bVertical = TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: ~CWinScrollBar
///
/// CLASS: CWinScrollBar
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 1:06:19 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinScrollBar::~CWinScrollBar()
{

}



/////////////////////////////////////////////////
///
/// NAME: Create
///
/// CLASS: CWinScrollBar
///
/// DESCRIPTION: Create control
///
/// CREATED:     02/06/2004 1:06:29 PM
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
BOOL CWinScrollBar::Create(C2DDraw *pDisplay, CWinBaceObj *pParent,CPoint p, CSize s, CString str)
{
   if(CWinBaceObj::Create(pDisplay, pParent,p, s, str) == FALSE)
      return FALSE;

   CSize ButtonSize;
   ButtonSize.cx = s.cx-3;
   ButtonSize.cy = s.cx-2;

   CPoint ButtonPos(2,1); 

   m_up.Create(pDisplay,this,ButtonPos,ButtonSize,"u");
   m_up.SetID(IDC_SCROLL_UP);
   m_up.SetType(WINARROW_UP);

   ButtonPos.Offset(0,s.cy-s.cx);
   m_down.Create(pDisplay,this,ButtonPos,ButtonSize,"d");
   m_down.SetID(IDC_SCROLL_DOWN);
   m_down.SetType(WINARROW_DOWN);

   return TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: Display
///
/// CLASS: CWinScrollBar
///
/// DESCRIPTION: Display control
///
/// CREATED:     02/06/2004 1:06:35 PM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinScrollBar::Display(C2DDraw *pDisplay)
{
   CRect rect = GetDrawRect();

   if(ShowBackground())
      pDisplay->DrawBlendRect(rect,m_EditAreaColor);

   DrawBorder(pDisplay,rect);

   CSize s = rect.Size();
   int area = s.cy - (s.cx + s.cx);
   int nSliderPoints = m_RangeMax-m_RangeMin+1;

   CSize SliderSize;
   SliderSize.cx = s.cx;
   SliderSize.cy = area/nSliderPoints;
   if(SliderSize.cy<5)
      SliderSize.cy = 5;

   CPoint SliderPos = rect.TopLeft();
   SliderPos.Offset(0,s.cx+((area/nSliderPoints) * (m_ScrollPos-m_RangeMin)));

   CRect SliderRect(SliderPos,SliderSize);
   if(ShowBackground())
      pDisplay->DrawBlendRect(SliderRect,m_BackgroundColor);

   DrawBorder(pDisplay,SliderRect);

   CWinBaceObj::Display(pDisplay);
}



/////////////////////////////////////////////////
///
/// NAME: OnButtonPressed
///
/// CLASS: CWinScrollBar
///
/// DESCRIPTION: Handle button presses
///
/// CREATED:     02/06/2004 1:06:53 PM
///
/// PARAMETERS: 
///             int ButtonID
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinScrollBar::OnButtonPressed(int ButtonID)
{
   if(ButtonID == IDC_SCROLL_UP)
   {
      m_ScrollPos--;
      if(m_ScrollPos<m_RangeMin)
         m_ScrollPos = m_RangeMin;
   }
   else if(ButtonID == IDC_SCROLL_DOWN)
   {
      m_ScrollPos++;
      if(m_ScrollPos>m_RangeMax)
         m_ScrollPos = m_RangeMax;
   }
   CWinBaceObj::OnButtonPressed(GetID());
}
