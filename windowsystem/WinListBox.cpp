///////////////////////////////////////////////////////////////////////////////
///  WinListBox.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:32:06 PM by Christiaan Burchell
///
///  PURPOSE:  the list box control
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

// WinListBox.cpp: implementation of the CWinListBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <WinListBox.h>
#include <commonres.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////
///
/// NAME: CWinListBox
///
/// CLASS: CWinListBox
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 12:32:09 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinListBox::CWinListBox() : CWinBaceObj()
{
   m_CurrentSel = -1;
   m_DisplayCount = 0;
}



/////////////////////////////////////////////////
///
/// NAME: ~CWinListBox
///
/// CLASS: CWinListBox
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 12:32:11 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinListBox::~CWinListBox()
{

}



/////////////////////////////////////////////////
///
/// NAME: Create
///
/// CLASS: CWinListBox
///
/// DESCRIPTION: Creates the list box
///
/// CREATED:     02/06/2004 12:32:18 PM
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
BOOL CWinListBox::Create(C2DDraw *pDisplay, CWinBaceObj *pParent, const CPoint& p, const CSize& s, const TCHAR* str)
{
   if(CWinBaceObj::Create(pDisplay, pParent,p, s, str) == FALSE)
      return FALSE;

   CSize ScrollSize;
   ScrollSize.cx = 11;
   ScrollSize.cy = s.cy;
   CPoint ScrollPos(s.cx-11,0); 

   m_Scroll.Create(pDisplay,this,ScrollPos,ScrollSize,"Scroll");
   m_Scroll.SetID(IDC_LIST_SCROLL);
   m_Scroll.SetScrollRange(0,0);


   CRect rect = GetDrawRect();


   CPoint TextPos = rect.TopLeft();
   TextPos.Offset(3,3);
   m_DisplayCount = 0;
   while(TRUE)
   {
      CRect TextRect = pDisplay->GetTextRect("Test");
      TextRect.OffsetRect(TextPos);
      if(rect.PtInRect(CPoint(TextRect.left,TextRect.bottom)))
      {
         m_DisplayCount++;
      }
      else
      {
         break;
      }
      TextPos.Offset(0,TextRect.Size().cy);
   }

   return TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: Display
///
/// CLASS: CWinListBox
///
/// DESCRIPTION: Displays the control
///
/// CREATED:     02/06/2004 12:32:29 PM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinListBox::Display(C2DDraw *pDisplay)
{
   CRect rect = GetDrawRect();
   if(ShowBackground())
      pDisplay->DrawBlendRect(rect,m_EditAreaColor);


   DrawBorder(pDisplay,rect);

   int listSize = m_Items.size();
   CPoint TextPos = rect.TopLeft();
   TextPos.Offset(3,3);
   for(int i = m_Scroll.GetScrollPos(); i<listSize;i++)
   {
      CRect TextRect = pDisplay->GetTextRect(m_Items[i].m_String);
      TextRect.OffsetRect(TextPos);
      if(rect.PtInRect(CPoint(TextRect.left,TextRect.bottom)))
      {
         if(m_CurrentSel == i)
         {
            pDisplay->DrawBlendRect(TextRect,m_SelectColor);
            pDisplay->OutTextc(m_SelectColor,m_HighlightColor,TextPos.x,TextPos.y,FALSE,"%s",m_Items[i].m_String);
         }
         else
         {
            pDisplay->OutTextc(m_EditAreaColor,m_Items[i].m_Color,TextPos.x,TextPos.y,FALSE,"%s",m_Items[i].m_String);
         }
      }
      TextPos.Offset(0,TextRect.Size().cy);
   }

   CWinBaceObj::Display(pDisplay);
}



/////////////////////////////////////////////////
///
/// NAME: OnButtonPressed
///
/// CLASS: CWinListBox
///
/// DESCRIPTION: Handels button press
///
/// CREATED:     02/06/2004 12:32:40 PM
///
/// PARAMETERS: 
///             int ButtonID
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinListBox::OnButtonPressed(int ButtonID)
{
   if(ButtonID != IDC_LIST_SCROLL)
      CWinBaceObj::OnButtonPressed(GetID());
}



/////////////////////////////////////////////////
///
/// NAME: AddString
///
/// CLASS: CWinListBox
///
/// DESCRIPTION: Adds a string
///
/// CREATED:     02/06/2004 12:32:48 PM
///
/// PARAMETERS: 
///             CString str
///             D3DCOLOR Color
///
/// RETURN: int 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
int CWinListBox::AddString(const TCHAR* str,D3DCOLOR Color)
{
   int index = GetCount();
   CWinLBItem Item;
   Item.m_String = str;
   Item.m_Color = Color;
   m_Items.push_back(Item);
   m_Scroll.SetScrollRange(0,index);
   return index;
}



/////////////////////////////////////////////////
///
/// NAME: AddString
///
/// CLASS: CWinListBox
///
/// DESCRIPTION: Adds a string
///
/// CREATED:     02/06/2004 12:33:00 PM
///
/// PARAMETERS: 
///             CString str
///
/// RETURN: int 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
int CWinListBox::AddString(const TCHAR* str)
{
   return 	AddString(str,m_ForgoundColor);
}



/////////////////////////////////////////////////
///
/// NAME: OnMouseUp
///
/// CLASS: CWinListBox
///
/// DESCRIPTION: Handels mouse up
///
/// CREATED:     02/06/2004 12:33:09 PM
///
/// PARAMETERS: 
///             CPoint p
///
/// RETURN: int 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
int CWinListBox::OnMouseUp(const CPoint& p)
{
   CRect rect = GetDrawRect();
   int listSize = m_Items.size();
   CPoint TextPos = rect.TopLeft();
   TextPos.Offset(3,3);
   for(int i = m_Scroll.GetScrollPos(); i<listSize;i++)
   {
      CRect TextRect = GetDisplay()->GetTextRect(m_Items[i].m_String);
      TextRect.OffsetRect(TextPos);
      if(TextRect.PtInRect(p))
      {
         SetCurSel(i);
         OnButtonPressed(GetID());
         break;
      }
      TextPos.Offset(0,TextRect.Size().cy);
   }
   return 0;
}



/////////////////////////////////////////////////
///
/// NAME: SetVisable
///
/// CLASS: CWinListBox
///
/// DESCRIPTION: Set the index given visable
///
/// CREATED:     02/06/2004 12:33:59 PM
///
/// PARAMETERS: 
///             int index
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinListBox::SetVisable(int index)
{
   int ScrollIndex = index - m_DisplayCount;

   if(m_DisplayCount<=index)
      ScrollIndex += 1;

   if(ScrollIndex < 0)
      ScrollIndex = 0;

   m_Scroll.SetScrollPos(ScrollIndex);
}
