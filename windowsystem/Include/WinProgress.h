///////////////////////////////////////////////////////////////////////////////
///  WinProgress.h
///
///  PATH: D:\dev2\TerraFighterDev
///
///  CREATED: 02/06/2004 1:04:20 PM by Christiaan Burchell
///
///  PURPOSE:  Progress bar control
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

// WinProgress.h: interface for the CWinProgress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINPROGRESS_H__6C7225D3_07C9_4FE2_88C6_E89F258A6151__INCLUDED_)
#define AFX_WINPROGRESS_H__6C7225D3_07C9_4FE2_88C6_E89F258A6151__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinBaceObj.h"
#include "WinToolTip.h"

class CWinProgress : public CWinBaceObj
{
public:
   void SetToolTipText(const TCHAR* s);
   BOOL Create(C2DDraw *pDisplay, CWinBaceObj *pParent, const CPoint& p, const CSize& s, const TCHAR* str);
   virtual void OnMouseOver(BOOL bIsOver, const CPoint& p);
   D3DCOLOR m_color;
   void SetColor(D3DCOLOR color);
   int m_h;
   void SetProgress(int progress);
   virtual void Display(C2DDraw *pDisplay);
   CWinProgress();
   virtual ~CWinProgress();
protected:
   CWinToolTip m_ToolTip;

};

#endif // !defined(AFX_WINPROGRESS_H__6C7225D3_07C9_4FE2_88C6_E89F258A6151__INCLUDED_)
