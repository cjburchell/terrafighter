///////////////////////////////////////////////////////////////////////////////
///  WinComboBox.cpp
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 1:12:03 PM by Christiaan Burchell
///
///  PURPOSE:  Combobox control
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

// WinComboBox.cpp: implementation of the CWinComboBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <WinComboBox.h>
#include <commonres.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define COMBO_SIZE 12




/////////////////////////////////////////////////
///
/// NAME: CWinComboBox
///
/// CLASS: CWinComboBox
///
/// DESCRIPTION: Class Constructor
///
/// CREATED:     02/06/2004 12:18:54 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinComboBox::CWinComboBox() : CWinBaceObj()
{

}



/////////////////////////////////////////////////
///
/// NAME: ~CWinComboBox
///
/// CLASS: CWinComboBox
///
/// DESCRIPTION: Class Destructor
///
/// CREATED:     02/06/2004 12:18:55 PM
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
CWinComboBox::~CWinComboBox()
{

}



/////////////////////////////////////////////////
///
/// NAME: Create
///
/// CLASS: CWinComboBox
///
/// DESCRIPTION: Creates the control
///
/// CREATED:     02/06/2004 12:19:04 PM
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
BOOL CWinComboBox::Create(C2DDraw *pDisplay, CWinBaceObj *pParent,CPoint p, CSize s, CString str)
{
   if(CWinBaceObj::Create(pDisplay, pParent,p, s, str) == FALSE)
      return FALSE;

   CSize ButtonSize;
   ButtonSize.cx = 11 - 1;
   ButtonSize.cy = COMBO_SIZE - 2;
   CPoint ButtonPos(s.cx-11 - 1,2); 
   m_Button.Create(pDisplay,this,ButtonPos,ButtonSize,"d");
   m_Button.SetID(IDC_COMBO_BUTTON);
   m_Button.SetType(WINARROW_DOWN);

   CSize ListSize;

   ListSize.cx = s.cx;
   ListSize.cy = s.cy-COMBO_SIZE;

   CPoint ListPos(0,COMBO_SIZE); 

   m_List.Create(pDisplay,this,ListPos,ListSize,"d");
   m_List.SetID(IDC_COMBO_LIST);
   m_List.ShowWindow(FALSE);
   m_List.SetShowBackground();

   return TRUE;
}



/////////////////////////////////////////////////
///
/// NAME: Display
///
/// CLASS: CWinComboBox
///
/// DESCRIPTION: Displays the control
///
/// CREATED:     02/06/2004 12:19:12 PM
///
/// PARAMETERS: 
///             C2DDraw *pDisplay
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinComboBox::Display(C2DDraw *pDisplay)
{
   CRect rect = GetDrawRect();
   rect.bottom = rect.top + COMBO_SIZE;
   if(ShowBackground())
      pDisplay->DrawBlendRect(rect,m_EditAreaColor);

   DrawBorder(pDisplay,rect);

   if(m_List.GetCurSel() != -1)
      pDisplay->OutTextc(m_BackgroundColor,m_ForgoundColor,GetDrawPoint().x+2,GetDrawPoint().y+1,FALSE,"%s",m_List.GetCurString());

   CWinBaceObj::Display(pDisplay);
}



/////////////////////////////////////////////////
///
/// NAME: OnButtonPressed
///
/// CLASS: CWinComboBox
///
/// DESCRIPTION: handels the combo button pressed
///
/// CREATED:     02/06/2004 12:19:33 PM
///
/// PARAMETERS: 
///             int ButtonID
///
/// RETURN: void 
///
/// SIDE EFFECTS: 
///
/////////////////////////////////////////////////
void CWinComboBox::OnButtonPressed(int ButtonID)
{
   if(ButtonID == IDC_COMBO_BUTTON)
   {
      m_List.ShowWindow(!m_List.IsShown());
   }
   else if(ButtonID == IDC_COMBO_LIST)
   {
      m_List.ShowWindow(FALSE);
      CWinBaceObj::OnButtonPressed(GetID());
   }
}
