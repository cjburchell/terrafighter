///////////////////////////////////////////////////////////////////////////////
///  WinToolTip.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 1:07:43 PM by Christiaan Burchell
///
///  PURPOSE:  tool tip control
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

// WinToolTip.h: interface for the CWinToolTip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINTOOLTIP_H__16294D54_ED70_4133_9A28_A496F74EDBEC__INCLUDED_)
#define AFX_WINTOOLTIP_H__16294D54_ED70_4133_9A28_A496F74EDBEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinBaceObj.h"

class CWinToolTip : public CWinBaceObj  
{
public:
   BOOL Create(C2DDraw *pDisplay, CWinBaceObj *pParent,const CPoint&  p,const CSize& s, const TCHAR* str);
   BOOL m_bShow;
   void Show(BOOL bShow = TRUE);
   virtual BOOL MouseUp(const CPoint& p, int &Id);
   virtual void Display(C2DDraw *pDisplay);
   virtual BOOL MouseOver(const CPoint&  p);
   virtual BOOL MouseDown(const CPoint&  p, int &Id);
   CWinToolTip();
   virtual ~CWinToolTip();

};

#endif // !defined(AFX_WINTOOLTIP_H__16294D54_ED70_4133_9A28_A496F74EDBEC__INCLUDED_)
