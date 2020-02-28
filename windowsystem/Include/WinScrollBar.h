///////////////////////////////////////////////////////////////////////////////
///  WinScrollBar.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 1:06:03 PM by Christiaan Burchell
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

// WinScrollBar.h: interface for the CWinScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINSCROLLBAR_H__DBCD2B50_FB46_4A3E_8E1B_2A77B6A783AC__INCLUDED_)
#define AFX_WINSCROLLBAR_H__DBCD2B50_FB46_4A3E_8E1B_2A77B6A783AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinBaceObj.h"
#include "WinArrowButton.h"



class CWinScrollBar : public CWinBaceObj  
{
public:
   virtual void Display(C2DDraw *pDisplay);
   CWinScrollBar();
   virtual ~CWinScrollBar();
   void SetScrollRange (int Min, int Max)
   {
      m_RangeMin = Min;
      m_RangeMax = Max;
   }

   void GetScrollRange (int* Min, int* Max) const
   {
      *Min = m_RangeMin;
      *Max = m_RangeMax;
   }

   int GetScrollPos() const {return m_ScrollPos;}
   void SetScrollPos(int Pos){m_ScrollPos = Pos;}

   virtual BOOL Create(C2DDraw *pDisplay, CWinBaceObj *pParent,CPoint p, CSize s, CString str);

   void SetVerticalAlingment(BOOL bVertical = TRUE){m_bVertical = bVertical;}

   virtual void OnButtonPressed(int ButtonID);

private:
   CWinArrowButton m_down;
   CWinArrowButton m_up;
   int m_RangeMin;
   int m_RangeMax;
   int m_ScrollPos;
   BOOL m_bVertical;
};

#endif // !defined(AFX_WINSCROLLBAR_H__DBCD2B50_FB46_4A3E_8E1B_2A77B6A783AC__INCLUDED_)
