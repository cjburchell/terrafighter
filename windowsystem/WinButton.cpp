///////////////////////////////////////////////////////////////////////////////
///  WinButton.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:15:12 PM by Christiaan Burchell
///
///  PURPOSE:  The Button Control
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

// WinButton.cpp: implementation of the CWinButton class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "WinButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
///
/// NAME: CWinButton
///
/// CLASS: CWinButton
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 12:15:07 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinButton::CWinButton() : CWinBaceObj()
{
   m_bPressed = FALSE;
   m_isMouseOver = FALSE;
}



/////////////////////////////////////////////////
///
/// NAME: ~CWinButton
///
/// CLASS: CWinButton
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 12:16:22 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinButton::~CWinButton()
{

}



/////////////////////////////////////////////////
///
/// NAME: OnMouseDown
///
/// CLASS: CWinButton
///
/// DESCRIPTION: Handels the mouse down
///
/// CREATED:     02/06/2004 12:16:36 PM
///
/// RETURN: int 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
int CWinButton::OnMouseDown()
{
   if(IsEnabled())
      m_bPressed = TRUE;
   else
      m_bPressed = FALSE;
   return CWinBaceObj::OnMouseDown();
}



/////////////////////////////////////////////////
///
/// NAME: Display
///
/// CLASS: CWinButton
///
/// DESCRIPTION: Displays the window
///
/// CREATED:     02/06/2004 12:16:48 PM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinButton::Display(C2DDraw *pDisplay)
{
   CRect rect = GetDrawRect();
   if(ShowBackground())
      pDisplay->DrawBlendRect(rect,m_BackgroundColor);

   if(m_bPressed)
   {
      DrawBorder(pDisplay,rect);
   }
   else
   {
      pDisplay->DrawLine(rect.TopLeft(),CPoint(rect.left,rect.bottom),m_BorderColor2);
      pDisplay->DrawLine(rect.TopLeft(),CPoint(rect.right,rect.top),m_BorderColor2);


      pDisplay->DrawLine(CPoint(rect.right,rect.bottom-1),CPoint(rect.left,rect.bottom-1),m_BorderColor3);
      pDisplay->DrawLine(CPoint(rect.right-1,rect.bottom-1),CPoint(rect.right-1,rect.top),m_BorderColor3);

      pDisplay->DrawLine(rect.BottomRight(),CPoint(rect.left-1,rect.bottom),m_BorderColor);
      pDisplay->DrawLine(rect.BottomRight(),CPoint(rect.right,rect.top-1),m_BorderColor);
   }

   CPoint p; 
   CRect r = pDisplay->GetTextRect(GetText());
   r.OffsetRect(rect.TopLeft());
   p = rect.CenterPoint() - r.CenterPoint();
   r.OffsetRect(p);
   p = r.TopLeft();

   if(IsEnabled())
   {
      if(!m_isMouseOver)
         pDisplay->OutTextc(m_BackgroundColor,m_ForgoundColor,p.x,p.y, FALSE, "%s",GetText());
      else
         pDisplay->OutTextc(m_BackgroundColor,m_HighlightColor,p.x,p.y, FALSE, "%s",GetText());
   }
   else
      pDisplay->OutTextc(m_BackgroundColor,m_DisabledColor,p.x,p.y, FALSE, "%s",GetText());

   CWinBaceObj::Display(pDisplay);
}



/////////////////////////////////////////////////
///
/// NAME: OnMouseOver
///
/// CLASS: CWinButton
///
/// DESCRIPTION: Handels the mouse over
///
/// CREATED:     02/06/2004 12:17:02 PM
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
void CWinButton::OnMouseOver(BOOL bIsOver, const CPoint& p)
{
   if(IsEnabled())
   {
      m_isMouseOver = bIsOver;
      if(!bIsOver)
         m_bPressed = FALSE;
   }
   else
   {
      m_bPressed = FALSE;
      m_isMouseOver = FALSE;
   }
}



/////////////////////////////////////////////////
///
/// NAME: OnMouseUp
///
/// CLASS: CWinButton
///
/// DESCRIPTION: handels mose up
///
/// CREATED:     02/06/2004 12:17:11 PM
///
/// PARAMETERS: 
///             CPoint p
///
/// RETURN: int 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
int CWinButton::OnMouseUp(const CPoint& p)
{
   if(IsEnabled())
   {
      if(m_bPressed)
      {
         OnButtonPressed(GetID());
         m_bPressed = FALSE;
         return GetID();
      }
      else
         return WIN_BUTTON_ESC;
   }
   else
   {
      m_bPressed = FALSE;
      m_isMouseOver = FALSE;
      return WIN_BUTTON_ESC;
   }
}
