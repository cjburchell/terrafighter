///////////////////////////////////////////////////////////////////////////////
///  WinListBox.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 12:31:39 PM by Christiaan Burchell
///
///  PURPOSE:  The list box control
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

// WinListBox.h: interface for the CWinListBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINLISTBOX_H__AE8B70F6_A058_490C_B34E_55BC818DC233__INCLUDED_)
#define AFX_WINLISTBOX_H__AE8B70F6_A058_490C_B34E_55BC818DC233__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinBaceObj.h"
#include "WinScrollBar.h"
#include <vector>

class CWinLBItem
{
public:
   CWinLBItem()
   {
      m_Data = NULL;
      m_String = _T("");
      m_Color = 0;

   }
   DWORD m_Data;
   CString m_String;
   D3DCOLOR m_Color; 
};


class CWinListBox : public CWinBaceObj  
{
public:

   CWinListBox();
   virtual ~CWinListBox();
   virtual BOOL Create(C2DDraw *pDisplay, CWinBaceObj *pParent, const CPoint& p, const CSize& s, const TCHAR* str);
   virtual void Display(C2DDraw *pDisplay);
   virtual void OnButtonPressed(int ButtonID);

   int GetCount() const {return m_Items.size();}
   int AddString(const TCHAR* str);
   int AddString(const TCHAR* str,D3DCOLOR Color);
   int GetCurSel() const {return m_CurrentSel;}
   void SetCurSel(int sel){m_CurrentSel = sel;}

   virtual int OnMouseUp(const CPoint&  p);

   const TCHAR* GetCurString(){return m_Items[m_CurrentSel].m_String;}
   void ResetContent(){m_CurrentSel = -1; m_Items.clear();}

   void SetVisable(int index);

   int AddItemData(const TCHAR* str,DWORD Data)
   {
      int index = AddString(str);
      m_Items[index].m_Data = Data;
      return index;
   }

   DWORD GetItemData(UINT index)
   {
      if(m_Items.size() < index)
         return 0;
      return m_Items[index].m_Data;
   }

   void SetItemData(UINT index,DWORD Data)
   {
      if(m_Items.size() < index)
         return;
      m_Items[index].m_Data = Data;
   }


   int FindString(const TCHAR* str)
   {
      int listSize = m_Items.size();
      for(int i=0;i<listSize;i++)
      {
         if(m_Items[i].m_String == str)
            return i;
      }
      return -1;
   }


private:
   CWinScrollBar m_Scroll;
   int m_CurrentSel;
   int m_DisplayCount;

   std::vector<CWinLBItem> m_Items;

};

#endif // !defined(AFX_WINLISTBOX_H__AE8B70F6_A058_490C_B34E_55BC818DC233__INCLUDED_)
