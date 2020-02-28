///////////////////////////////////////////////////////////////////////////////
///  WinComboBox.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:18:41 PM by Christiaan Burchell
///
///  PURPOSE:  the combobox control
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

// WinComboBox.h: interface for the CWinComboBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINCOMBOBOX_H__49EECAC9_4444_4FC1_97A3_4FD6A9A2BBE4__INCLUDED_)
#define AFX_WINCOMBOBOX_H__49EECAC9_4444_4FC1_97A3_4FD6A9A2BBE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinBaceObj.h"
#include "WinListBox.h"


class CWinComboBox : public CWinBaceObj  
{
public:
   CWinComboBox();
   virtual ~CWinComboBox();
   virtual BOOL Create(C2DDraw *pDisplay, CWinBaceObj *pParent,CPoint p, CSize s, CString str);
   virtual void Display(C2DDraw *pDisplay);
   virtual void OnButtonPressed(int ButtonID);

   int GetCount(){return m_List.GetCount();}
   int AddString(const TCHAR* str){return m_List.AddString(str);};
   int GetCurSel(){return m_List.GetCurSel();}
   void SetCurSel(int sel){m_List.SetCurSel(sel);}
   void ResetContent(){m_List.ResetContent();}

private:
   CWinArrowButton m_Button;
   CWinListBox m_List;

};

#endif // !defined(AFX_WINCOMBOBOX_H__49EECAC9_4444_4FC1_97A3_4FD6A9A2BBE4__INCLUDED_)
